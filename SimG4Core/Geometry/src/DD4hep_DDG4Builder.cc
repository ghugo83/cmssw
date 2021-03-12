#include "FWCore/Utilities/interface/Exception.h"

#include "SimG4Core/Geometry/interface/DD4hep_DDG4Builder.h"
#include "SimG4Core/Geometry/interface/SensitiveDetectorCatalog.h"

#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/DDCMS/interface/DDDetector.h"
#include <DDG4/Geant4Converter.h>
#include <DDG4/Geant4GeometryInfo.h>
#include <DDG4/Geant4Mapping.h>
#include <DD4hep/Detector.h>
#include <DD4hep/Filter.h>

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ReflectionFactory.hh"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <algorithm>

using namespace cms;
using namespace dd4hep;
using namespace dd4hep::sim;

DDG4Builder::DDG4Builder(const cms::DDCompactView *cpv, dd4hep::sim::Geant4GeometryMaps::VolumeMap &lvmap, std::unordered_map<G4LogicalVolume*, G4LogicalVolume*>& reflectedG4LogicalVolumes, bool check)
  : compactView_(cpv), map_(lvmap), reflectedG4LogicalVolumes_(reflectedG4LogicalVolumes), check_(check) {}

G4VPhysicalVolume *DDG4Builder::BuildGeometry(SensitiveDetectorCatalog &catalog) {
  G4ReflectionFactory *refFact = G4ReflectionFactory::Instance();
  refFact->SetScalePrecision(100. * refFact->GetScalePrecision());

  const cms::DDDetector *det = compactView_->detector();
  DetElement world = det->description()->world();
  const Detector &detector = *det->description();

  // GET FULL G4 GEOMETRY
  Geant4Converter g4Geo(detector);
  g4Geo.debugMaterials = false;
  Geant4GeometryInfo *geometry = g4Geo.create(world).detach();
  map_ = geometry->g4Volumes;

  // FIND & STORE ALL G4 LOGICAL VOLUMES DEFINED AS SENSITIVE IN CMSSW XMLS
  std::vector<std::pair<G4LogicalVolume *, const dd4hep::SpecPar *>> dd4hepVec;
  const dd4hep::SpecParRegistry &specPars = det->specpars();
  dd4hep::SpecParRefs specs;
  specPars.filter(specs, "SensitiveDetector");
  for (auto const &it : map_) {
    for (auto const &fit : specs) {
      for (auto const &pit : fit.second->paths) {
        if (dd4hep::dd::compareEqualName(dd4hep::dd::noNamespace(dd4hep::dd::realTopName(pit)),
                                         dd4hep::dd::noNamespace(it.first.name()))) {
          dd4hepVec.emplace_back(&*it.second, &*fit.second);
        }
      }
    }
  }


  for (auto const &it : map_) {
    G4LogicalVolume* nonReflectedG4LogicalVolume = it.second;
    const G4String& G4LogicalVolumeName = nonReflectedG4LogicalVolume->GetName();
    const G4String& G4LogicalVolumeReflectedName = G4LogicalVolumeName + "_refl";

    G4LogicalVolumeStore* allG4LogicalVolumes = G4LogicalVolumeStore::GetInstance();

    auto reflectedG4LogicalVolumeIt = std::find_if(allG4LogicalVolumes->begin(), allG4LogicalVolumes->end(), [&](G4LogicalVolume* aG4LogicalVolume) {
	return (aG4LogicalVolume->GetName() == G4LogicalVolumeReflectedName);
      });

    if (reflectedG4LogicalVolumeIt != allG4LogicalVolumes->end()) {
      reflectedG4LogicalVolumes_[nonReflectedG4LogicalVolume] = *reflectedG4LogicalVolumeIt;
    }
  }



  // ADD ALL SELECTED G4 LOGICAL VOLUMES TO SENSITIVE DETECTORS CATALOGUE
  for (auto const &it : dd4hepVec) {
    // Sensitive detector info
    const G4String &sensitiveDetectorG4Name = it.first->GetName();
    auto sClassName = it.second->strValue("SensitiveDetector");
    auto sROUName = it.second->strValue("ReadOutName");
    // Add to catalogue
    catalog.insert({sClassName.data(), sClassName.size()}, {sROUName.data(), sROUName.size()}, sensitiveDetectorG4Name);

    edm::LogVerbatim("SimG4CoreApplication") << " DDG4SensitiveConverter: Sensitive " << sensitiveDetectorG4Name
                                             << " Class Name " << sClassName << " ROU Name " << sROUName;

    // Reflected sensors also need to be added to the senstive detectors catalogue!
    // Similar treatment here with DD4hep, as what was done for old DD.
    const G4String &sensitiveDetectorG4ReflectedName = sensitiveDetectorG4Name + "_refl";
    //const G4LogicalVolumeStore *const allG4LogicalVolumes = G4LogicalVolumeStore::GetInstance();
    /*G4LogicalVolume* const reflectedG4LogicalVolume = *std::find_if(allG4LogicalVolumes->begin(), allG4LogicalVolumes->end(), [&](G4LogicalVolume *const aG4LogicalVolume) {
	return (aG4LogicalVolume->GetName() == sensitiveDetectorG4ReflectedName);
      });
      const bool hasG4ReflectedVolume = (reflectedG4LogicalVolume != nullptr);*/

    const bool hasG4ReflectedVolume2 = (reflectedG4LogicalVolumes_.find(it.first) != reflectedG4LogicalVolumes_.end());

    //std::cout << "(hasG4ReflectedVolume == hasG4ReflectedVolume2) = " << (hasG4ReflectedVolume == hasG4ReflectedVolume2) << std::endl;

    if (hasG4ReflectedVolume2) {
      // Add reflected sensitive detector to catalogue
      std::cout << sensitiveDetectorG4ReflectedName << std::endl;
      catalog.insert(
          {sClassName.data(), sClassName.size()}, {sROUName.data(), sROUName.size()}, sensitiveDetectorG4ReflectedName);

      edm::LogVerbatim("SimG4CoreApplication")
          << " DDG4SensitiveConverter: Sensitive " << sensitiveDetectorG4ReflectedName << " Class Name " << sClassName
          << " ROU Name " << sROUName;
    }
  }

  return geometry->world();
}
