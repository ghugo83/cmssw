#include "SimG4Core/Geometry/interface/DDDWorld.h"
#include "SimG4Core/Geometry/interface/DDG4Builder.h"
#include "SimG4Core/Geometry/interface/G4LogicalVolumeToDDLogicalPartMap.h"
#include "SimG4Core/Geometry/interface/DDG4ProductionCuts.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "SimG4Core/Geometry/interface/DD4hep_DDG4Builder.h"
#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/DDCMS/interface/DDDetector.h"
#include "DDG4/Geant4Converter.h"
#include "DDG4/Geant4GeometryInfo.h"
#include "DDG4/Geant4Mapping.h"
#include "DD4hep/Detector.h"
#include "DD4hep/Printout.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4PVPlacement.hh"

using namespace edm;
using namespace dd4hep;
using namespace dd4hep::sim;

DDDWorld::DDDWorld(const DDCompactView *pDD,
                   const cms::DDCompactView *pDD4hep,
                   SensitiveDetectorCatalog &catalog,
                   int verb,
                   bool cuts,
                   bool pcut) {
  LogVerbatim("SimG4CoreApplication") << "+++ DDDWorld: initialisation of Geant4 geometry";
  if (pDD4hep) {
    // DD4Hep
    const cms::DDDetector *det = pDD4hep->detector();
    dd4hep::sim::Geant4GeometryMaps::VolumeMap lvMap;
    cms::DDG4Builder theBuilder(pDD4hep, lvMap, false);

    auto startSens = std::chrono::high_resolution_clock::now();
    m_world = theBuilder.BuildGeometry(catalog);
    auto stopSens = std::chrono::high_resolution_clock::now();
    auto diffSens = stopSens - startSens;
    auto timeSens = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(diffSens).count()) / 1e6;
    std::cout << "Processed Sens catalog in " << std::scientific << timeSens << " seconds." << std::endl;

    catalog.printMe();

    LogVerbatim("SimG4CoreApplication") << "DDDWorld: worldLV: " << m_world->GetName();
    if (cuts) {
      auto start = std::chrono::high_resolution_clock::now();

      DDG4ProductionCuts pcuts(&det->specpars(), &lvMap, verb, pcut);

      auto stop = std::chrono::high_resolution_clock::now();
      auto diff = stop - start;
      auto time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(diff).count()) / 1e6;
      std::cout << "Processed Production cuts in " << std::scientific << time << " seconds." << std::endl;
    }

  } else {
    // old DD code
    G4LogicalVolumeToDDLogicalPartMap lvMap;
    DDG4Builder theBuilder(pDD, lvMap, false);

    auto startSens = std::chrono::high_resolution_clock::now();
    G4LogicalVolume *world = theBuilder.BuildGeometry(catalog);

    auto stopSens = std::chrono::high_resolution_clock::now();
    auto diffSens = stopSens - startSens;
    auto timeSens = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(diffSens).count()) / 1e6;
    std::cout << "Processed Sens catalog in " << std::scientific << timeSens << " seconds." << std::endl;

    catalog.printMe();

    LogVerbatim("SimG4CoreApplication") << "DDDWorld: worldLV: " << world->GetName();
    m_world = new G4PVPlacement(nullptr, G4ThreeVector(), world, "DDDWorld", nullptr, false, 0);
    if (cuts) {
      auto start = std::chrono::high_resolution_clock::now();

      DDG4ProductionCuts pcuts(&lvMap, verb, pcut);

      auto stop = std::chrono::high_resolution_clock::now();
      auto diff = stop - start;
      auto time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(diff).count()) / 1e6;
      std::cout << "Processed Production cuts in " << std::scientific << time << " seconds." << std::endl;
    }
  }
  LogVerbatim("SimG4CoreApplication") << "DDDWorld: initialisation of Geant4 geometry is done.";
}

DDDWorld::DDDWorld(const DDCompactView *cpv,
                   G4LogicalVolumeToDDLogicalPartMap &lvmap,
                   SensitiveDetectorCatalog &catalog,
                   bool check) {
  LogVerbatim("SimG4CoreApplication") << "DDDWorld: initialization of Geant4 geometry";
  DDG4Builder theBuilder(cpv, lvmap, check);

  G4LogicalVolume *world = theBuilder.BuildGeometry(catalog);

  m_world = new G4PVPlacement(nullptr, G4ThreeVector(), world, "DDDWorld", nullptr, false, 0);
  LogVerbatim("SimG4CoreApplication") << "DDDWorld: initialization of Geant4 geometry is done.";
}

DDDWorld::~DDDWorld() {}
