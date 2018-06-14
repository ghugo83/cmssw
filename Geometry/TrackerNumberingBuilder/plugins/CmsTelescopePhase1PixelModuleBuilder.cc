#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelModuleBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"

#include <bitset>

CmsTelescopePhase1PixelModuleBuilder::CmsTelescopePhase1PixelModuleBuilder() {}

void CmsTelescopePhase1PixelModuleBuilder::buildComponent( DDFilteredView& fv, GeometricDet* module, std::string attribute ) {
CmsDetConstruction myCmsDetBuilder;

  GeometricDet* myPhase1PixelSensor = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::DetUnit:
    // TEST
    std::cout << "Found a Phase1PixelSensor:"
	      << "myPhase1PixelSensor DetId = " << myPhase1PixelSensor->geographicalID().rawId() 
	      << ", x = " << myPhase1PixelSensor->translation().X() 
	      << ", y = " << myPhase1PixelSensor->translation().Y()
	      << ", z = " << myPhase1PixelSensor->translation().Z()
	      << ", phi = "  << myPhase1PixelSensor->phi() * 180. / M_PI << std::endl;
    // END TEST
    myCmsDetBuilder.build( fv, myPhase1PixelSensor, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopePhase1PixelModuleBuilder" ) << " ERROR - Could not find a DetUnit, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  module->addComponent(myPhase1PixelSensor);
}



void CmsTelescopePhase1PixelModuleBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allPhase1PixelModules = parent->components();
  std::stable_sort( allPhase1PixelModules.begin(), allPhase1PixelModules.end(), LessY());  // TO DO: use LessR() instead??
}




