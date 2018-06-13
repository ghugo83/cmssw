#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePlaneBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelModuleBuilder.h"

#include <bitset>

CmsTelescopePlaneBuilder::CmsTelescopePlaneBuilder() {}

void CmsTelescopePlaneBuilder::buildComponent( DDFilteredView& fv, GeometricDet* arm, std::string attribute ) {
  CmsTelescopePhase1PixelModuleBuilder myPhase1PixelModuleBuilder;

  GeometricDet* myPlane = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::Plane:
    // TEST
    /*std::cout << "myPlane DetId = " << myPlane->geographicalID().rawId() 
	      << ", x = " << myPlane->translation().X() 
	      << ", y = " << myPlane->translation().Y()
	      << ", z = " << myPlane->translation().Z()
	      << ", phi = "  << myPlane->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myPhase1PixelModuleBuilder.build( fv, myPlane, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopePlaneBuilder" ) << " ERROR - Could not find a Plane, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  arm->addComponent(myPlane);
}


void CmsTelescopePlaneBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allPlanes= parent->components();
  std::stable_sort( allPlanes.begin(), allPlanes.end(), LessModZ());
}



