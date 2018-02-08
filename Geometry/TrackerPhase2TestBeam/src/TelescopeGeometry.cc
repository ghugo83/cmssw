#include <typeinfo>

#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeometry.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <algorithm>
#include <iostream>
#include <map>

namespace {
  /*GeomDetEnumerators::SubDetector geometricDetToGeomDet(GeometricDet::GDEnumType gdenum) {
    // TO DO: Add here telescope arm, DUT container, and remove all the rest.
    // Obviously this class will be, after adapation, renamed into Telescope Geometry.
    // That way, we will have a Digitizer for the telescope :)
    // But first, let's see if things can slightly make sense!

    // provide a map between the GeometricDet enumerators and the GeomDet enumerators of the possible tracker subdetectors
    if(gdenum == GeometricDet::GDEnumType::PixelBarrel ) return GeomDetEnumerators::SubDetector::PixelBarrel;
    if(gdenum == GeometricDet::GDEnumType::PixelEndCap) return GeomDetEnumerators::SubDetector::PixelEndcap;
    if(gdenum == GeometricDet::GDEnumType::TIB) return GeomDetEnumerators::SubDetector::TIB;
    if(gdenum == GeometricDet::GDEnumType::TID) return GeomDetEnumerators::SubDetector::TID;
    if(gdenum == GeometricDet::GDEnumType::TOB) return GeomDetEnumerators::SubDetector::TOB;
    if(gdenum == GeometricDet::GDEnumType::TEC) return GeomDetEnumerators::SubDetector::TEC;
    if(gdenum == GeometricDet::GDEnumType::PixelPhase1Barrel) return GeomDetEnumerators::SubDetector::P1PXB;
    if(gdenum == GeometricDet::GDEnumType::PixelPhase1EndCap) return GeomDetEnumerators::SubDetector::P1PXEC;
    if(gdenum == GeometricDet::GDEnumType::PixelPhase2Barrel) return GeomDetEnumerators::SubDetector::P2PXB;
    if(gdenum == GeometricDet::GDEnumType::PixelPhase2EndCap) return GeomDetEnumerators::SubDetector::P2PXEC;
    if(gdenum == GeometricDet::GDEnumType::OTPhase2Barrel) return GeomDetEnumerators::SubDetector::P2OTB;
    if(gdenum == GeometricDet::GDEnumType::OTPhase2EndCap) return GeomDetEnumerators::SubDetector::P2OTEC;
    return GeomDetEnumerators::SubDetector::invalidDet;
  }*/
  
  class DetIdComparator {
  public:
    bool operator()(GeometricDet const* gd1, GeometricDet const * gd2) const {
      uint32_t det1 = gd1->geographicalId();
      uint32_t det2 = gd2->geographicalId();
      return det1 < det2;
    }  
  };
}

TelescopeGeometry::TelescopeGeometry(GeometricDet const* gd)
   : theTrackerDet(gd) { 

  /*
    for(unsigned int i=0;i<6;++i) {
    theSubDetTypeMap[i] = GeomDetEnumerators::invalidDet;
    theNumberOfLayers[i] = 0;
    }
  */
  // Telescope arm, (-Z) side
  theSubDetTypeMap[0] = GeomDetEnumerators::SubDetector::P1PXEC;  // See Geometry/CommonDetUnit/interface/GeomDetEnumerators.h (and its amazing code style lol)
  theNumberOfLayers[0] = 4;

  // Telescope DUT containers (contains only 1 DUT here)
  theSubDetTypeMap[1] = GeomDetEnumerators::SubDetector::P2OTB;
  theNumberOfLayers[1] = 1;

  // Telescope arm, (+Z) side
  theSubDetTypeMap[2] = GeomDetEnumerators::SubDetector::P1PXEC;  // See Geometry/CommonDetUnit/interface/GeomDetEnumerators.h (and its amazing code style lol)
  theNumberOfLayers[2] = 4;


  /*
  GeometricDet::ConstGeometricDetContainer subdetgd = gd->components();
  LogDebug("BuildingSubDetTypeMap") << "GeometriDet and GeomDetEnumerators enumerator values of the subdetectors";
  for(unsigned int i=0;i<subdetgd.size();++i) {
    assert(subdetgd[i]->geographicalId().subdetId()>0 && subdetgd[i]->geographicalId().subdetId()<7);
    theSubDetTypeMap[subdetgd[i]->geographicalId().subdetId()-1]= geometricDetToGeomDet(subdetgd[i]->type());
    theNumberOfLayers[subdetgd[i]->geographicalId().subdetId()-1]= subdetgd[i]->components().size();
    LogTrace("BuildingSubDetTypeMap") << "subdet " << i 
				      << " Geometric Det type " << subdetgd[i]->type()
				      << " Geom Det type " << theSubDetTypeMap[subdetgd[i]->geographicalId().subdetId()-1]
				      << " detid " <<  subdetgd[i]->geographicalId()
				      << " subdetid " <<  subdetgd[i]->geographicalId().subdetId()
				      << " number of layers " << subdetgd[i]->components().size();
  }
  LogDebug("SubDetTypeMapContent") << "Content of theSubDetTypeMap";
  for(unsigned int i=1;i<7;++i) {
    LogTrace("SubDetTypeMapContent") << " detid subdet "<< i << " Geom Det type " << geomDetSubDetector(i); 
  }
  LogDebug("NumberOfLayers") << "Content of theNumberOfLayers";
  for(unsigned int i=1;i<7;++i) {
    LogTrace("NumberOfLayers") << " detid subdet "<< i << " number of layers " << numberOfLayers(i); 
  }
  */

  std::vector<const GeometricDet*> deepcomp;
  gd->deepComponents(deepcomp);
   
  sort(deepcomp.begin(), deepcomp.end(), DetIdComparator());

  LogDebug("ThicknessAndType") << " Total Number of Detectors " << deepcomp.size() ;
  LogDebug("ThicknessAndType") << "Dump of sensors names and bounds";
  for(auto det : deepcomp) {
    fillTestMap(det); 
    LogDebug("ThicknessAndType") << det->geographicalId() << " " << det->name().fullname() << " " << det->bounds()->thickness();
  }
  LogDebug("DetTypeList") << " Content of DetTypetList : size " << theDetTypetList.size();
  for (auto iVal : theDetTypetList) {
    LogDebug("DetTypeList") << " DetId " << std::get<0>(iVal)
			    << " Type " << static_cast<std::underlying_type<TelescopeGeometry::ModuleType>::type>(std::get<1>(iVal))
			    << " Thickness " << std::get<2>(iVal);
  }  
}

TelescopeGeometry::~TelescopeGeometry() {
    for (auto d : theDets) delete const_cast<GeomDet*>(d);
    for (auto d : theDetTypes) delete const_cast<GeomDetType*>(d);
}

void TelescopeGeometry::finalize() {
    theDetTypes.shrink_to_fit();  // owns the DetTypes
    theDetUnits.shrink_to_fit();  // they're all also into 'theDets', so we assume 'theDets' owns them
    theDets.shrink_to_fit();     // owns *ONLY* the GeomDet * corresponding to GluedDets.
    theDetUnitIds.shrink_to_fit();
    theDetIds.shrink_to_fit();
  
    thePXBDets.shrink_to_fit(); // not owned: they're also in 'theDets'
    thePXFDets.shrink_to_fit(); // not owned: they're also in 'theDets'
    theTIBDets.shrink_to_fit(); // not owned: they're also in 'theDets'
    theTIDDets.shrink_to_fit(); // not owned: they're also in 'theDets'
    theTOBDets.shrink_to_fit(); // not owned: they're also in 'theDets'
    theTECDets.shrink_to_fit(); // not owned: they're also in 'theDets'
}

void TelescopeGeometry::addType(GeomDetType const * p) {
  theDetTypes.emplace_back(p);  // add to vector
}

void TelescopeGeometry::addDetUnit(GeomDet const * p) {
  // set index
  const_cast<GeomDet *>(p)->setIndex(theDetUnits.size());
  theDetUnits.emplace_back(p);  // add to vector
  theMapUnit.insert(std::make_pair(p->geographicalId().rawId(),p));
}

void TelescopeGeometry::addDetUnitId(DetId p){
  theDetUnitIds.emplace_back(p);
}

void TelescopeGeometry::addDet(GeomDet const * p) {
  // set index
  const_cast<GeomDet *>(p)->setGdetIndex(theDets.size());
  theDets.emplace_back(p);  // add to vector
  theMap.insert(std::make_pair(p->geographicalId().rawId(),p));
  DetId id(p->geographicalId());
  switch(id.subdetId()){
  case PixelSubdetector::PixelBarrel:
    thePXBDets.emplace_back(p);
    break;
  case PixelSubdetector::PixelEndcap:
    thePXFDets.emplace_back(p);
    break;
  case StripSubdetector::TIB:
    theTIBDets.emplace_back(p);
    break;
  case StripSubdetector::TID:
    theTIDDets.emplace_back(p);
    break;
  case StripSubdetector::TOB:
    theTOBDets.emplace_back(p);
    break;
  case StripSubdetector::TEC:
    theTECDets.emplace_back(p);
    break;
  default:
    edm::LogError("TelescopeGeometry")<<"ERROR - I was expecting a Tracker Subdetector, I got a "<<id.subdetId();
  }
}

void TelescopeGeometry::addDetId(DetId p){
  theDetIds.emplace_back(p);
}


const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsPXB() const
{
  return thePXBDets;
}

const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsPXF() const
{
  return thePXFDets;
}

const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsTIB() const
{
  return theTIBDets;
}

const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsTID() const
{
  return theTIDDets;
}

const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsTOB() const
{
  return theTOBDets;
}

const TelescopeGeometry::DetContainer&
TelescopeGeometry::detsTEC() const
{
  return theTECDets;
}

const TrackerGeomDet * 
TelescopeGeometry::idToDetUnit(DetId s)const
{
  mapIdToDetUnit::const_iterator p=theMapUnit.find(s.rawId());
  if (p != theMapUnit.end()) {
    return static_cast<const TrackerGeomDet *>(p->second);
  } else {
    throw cms::Exception("WrongTrackerSubDet") << "Invalid DetID: no GeomDetUnit associated with raw ID "
					       << s.rawId() << " of subdet ID " << s.subdetId();
  }
}

const TrackerGeomDet* 
TelescopeGeometry::idToDet(DetId s)const
{
  mapIdToDet::const_iterator p=theMap.find(s.rawId());
  if (p != theMap.end()) {
    return static_cast<const TrackerGeomDet *>(p->second);
  } else {
    throw cms::Exception("WrongTrackerSubDet") << "Invalid DetID: no GeomDetUnit associated with raw ID "
					       << s.rawId() << " of subdet ID " << s.subdetId();
  }
}

const GeomDetEnumerators::SubDetector 
TelescopeGeometry::geomDetSubDetector(int subdet) const {
  if(subdet>=1 && subdet<=6) {
    return theSubDetTypeMap[subdet-1];
  } else {
    throw cms::Exception("WrongTrackerSubDet") << "Subdetector " << subdet;
  }
}

unsigned int
TelescopeGeometry::numberOfLayers(int subdet) const {
  if(subdet>=1 && subdet<=6) {
    return theNumberOfLayers[subdet-1];
  } else {
    throw cms::Exception("WrongTrackerSubDet") << "Subdetector " << subdet;
  }
}

bool
TelescopeGeometry::isThere(GeomDetEnumerators::SubDetector subdet) const {
  for(unsigned int i=1;i<7;++i) {
    if(subdet == geomDetSubDetector(i)) return true;
  }
  return false;
}

void TelescopeGeometry::fillTestMap(const GeometricDet* gd) {
    
  std::string temp = gd->name();
  std::string name = temp.substr(temp.find(":")+1); 
  DetId detid = gd->geographicalId();
  float thickness = gd->bounds()->thickness();
  std::string nameTag;  
  TelescopeGeometry::ModuleType mtype = moduleType(name);
  if (theDetTypetList.empty()) {
    theDetTypetList.emplace_back(detid, mtype, thickness);
  } else {
    auto  & t = (*(theDetTypetList.end()-1));
    if (std::get<1>(t) != mtype) theDetTypetList.emplace_back(detid, mtype, thickness);
    else {
      if  ( detid > std::get<0>(t) ) std::get<0>(t) = detid;
    }
  }
}

TelescopeGeometry::ModuleType TelescopeGeometry::getDetectorType(DetId detid) const {
  for (auto iVal : theDetTypetList) {
    DetId detid_max = std::get<0>(iVal);
    TelescopeGeometry::ModuleType mtype =  std::get<1>(iVal);     
    if (detid.rawId() <=  detid_max.rawId()) return mtype;
  }
  return TelescopeGeometry::ModuleType::UNKNOWN;
}

float TelescopeGeometry::getDetectorThickness(DetId detid) const {
  for (auto iVal : theDetTypetList) {
    DetId detid_max = std::get<0>(iVal);
    if (detid.rawId() <=  detid_max.rawId()) 
      return std::get<2>(iVal);
  }
  return -1.0;
}

TelescopeGeometry::ModuleType TelescopeGeometry::moduleType(const std::string& name) const {
  if ( name.find("Pixel") != std::string::npos ){
    if ( name.find("BarrelActive") != std::string::npos) return ModuleType::Ph1PXB;
    else if ( name.find("ForwardSensor") != std::string::npos) return ModuleType::Ph1PXF;
    else if ( name.find("BModule") != std::string::npos && name.find("InnerPixelActive") != std::string::npos ) return ModuleType::Ph2PXB;
    else if ( name.find("EModule") != std::string::npos && name.find("InnerPixelActive") != std::string::npos ) return ModuleType::Ph2PXF;
  } else if ( name.find("TIB") != std::string::npos) {
    if ( name.find("0") != std::string::npos) return ModuleType::IB1;
    else return ModuleType::IB2;
  } else if ( name.find("TOB") != std::string::npos) {
    if ( name.find("0") != std::string::npos) return ModuleType::OB1;
    else return ModuleType::OB2;
  } else if ( name.find("TID") != std::string::npos) {
    if ( name.find("0") != std::string::npos) return ModuleType::W1A; 
    else if ( name.find("1") != std::string::npos) return ModuleType::W2A;
    else if ( name.find("2") != std::string::npos) return ModuleType::W3A;
  } else if ( name.find("TEC") != std::string::npos) { 
    if ( name.find("0") != std::string::npos) return ModuleType::W1B;
    else if ( name.find("1") != std::string::npos) return ModuleType::W2B;
    else if ( name.find("2") != std::string::npos) return ModuleType::W3B;
    else if ( name.find("3") != std::string::npos) return ModuleType::W4;
    else if ( name.find("4") != std::string::npos) return ModuleType::W5;
    else if ( name.find("5") != std::string::npos) return ModuleType::W6;
    else if ( name.find("6") != std::string::npos) return ModuleType::W7;
  } 
  if ( name.find("BModule") != std::string::npos || name.find("EModule") != std::string::npos ) { 
    if (name.find("PSMacroPixel") != std::string::npos) return ModuleType::Ph2PSP;
    else if (name.find("PSStrip") != std::string::npos) return ModuleType::Ph2PSS;
    else if (name.find("2S") != std::string::npos) return ModuleType::Ph2SS;
  }
  return ModuleType::UNKNOWN;  
}
