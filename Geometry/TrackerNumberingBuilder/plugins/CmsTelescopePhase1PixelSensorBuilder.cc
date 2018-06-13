#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelSensorBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"

#include <bitset>

CmsTelescopePhase1PixelSensorBuilder::CmsTelescopePhase1PixelSensorBuilder() {}

void CmsTelescopePhase1PixelSensorBuilder::buildComponent( DDFilteredView& fv, GeometricDet* module, std::string attribute ) {
  CmsDetConstruction myCmsDetBuilder;

  GeometricDet* myPhase1PixelSensor = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::DetUnit:
    // TEST
    /*std::cout << "myPhase1PixelSensor DetId = " << myPhase1PixelSensor->geographicalID().rawId() 
	      << ", x = " << myPhase1PixelSensor->translation().X() 
	      << ", y = " << myPhase1PixelSensor->translation().Y()
	      << ", z = " << myPhase1PixelSensor->translation().Z()
	      << ", phi = "  << myPhase1PixelSensor->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myCmsDetBuilder.build( fv, myPhase1PixelSensor, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopePhase1PixelSensorBuilder" ) << " ERROR - Could not find a DetUnit, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  module->addComponent(myPhase1PixelSensor);
}

 // TO DO: Phase 1 Pixel Sensor should be added to Phase 1 Pixel Module


void CmsTelescopePhase1PixelSensorBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allSensors = parent->components();
  std::stable_sort( allSensors.begin(), allSensors.end(), LessZ()); 
}




