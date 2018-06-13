#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelModuleBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelSensorBuilder.h"

#include <bitset>

CmsTelescopePhase1PixelModuleBuilder::CmsTelescopePhase1PixelModuleBuilder() {}

void CmsTelescopePhase1PixelModuleBuilder::buildComponent( DDFilteredView& fv, GeometricDet* plane, std::string attribute ) {
  CmsTelescopePhase1PixelSensorBuilder myPhase1PixelSensorBuilder;

  GeometricDet* myPhase1PixelModule = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::Phase1PixelModule:
    // TEST
    /*std::cout << "myPhase1PixelModule DetId = " << myPhase1PixelModule->geographicalID().rawId() 
	      << ", x = " << myPhase1PixelModule->translation().X() 
	      << ", y = " << myPhase1PixelModule->translation().Y()
	      << ", z = " << myPhase1PixelModule->translation().Z()
	      << ", phi = "  << myPhase1PixelModule->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myPhase1PixelSensorBuilder.build( fv, myPhase1PixelModule, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopePhase1PixelModuleBuilder" ) << " ERROR - Could not find a Phase1PixelModule, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  plane->addComponent(myPhase1PixelModule);
}


void CmsTelescopePhase1PixelModuleBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allPhase1PixelModules = parent->components();
  std::stable_sort( allPhase1PixelModules.begin(), allPhase1PixelModules.end(), LessY());  // TO DO: use LessR() instead??
}




