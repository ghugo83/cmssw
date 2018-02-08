#include "Geometry/TrackerPhase2TestBeam/interface/PlaneBuilder.h"


PlaneBuilder::PlaneBuilder() {}

void PlaneBuilder::buildComponent( DDFilteredView& fv, GeometricDet* arm, std::string attribute ) {
  Phase1PixelModuleBuilder myPhase1PixelModuleBuilder;

  GeometricDet* plane = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::Plane:
    // TEST
    /*std::cout << "plane DetId = " << plane->geographicalID().rawId() 
	      << ", x = " << plane->translation().X() 
	      << ", y = " << plane->translation().Y()
	      << ", z = " << plane->translation().Z()
	      << ", phi = "  << plane->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myPhase1PixelModuleBuilder.build( fv, plane, attribute);      
    break;
  default:
    edm::LogError( "PlaneBuilder" ) << " ERROR - Could not find a Phase1PixelModule, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  arm->addComponent(plane);
}


void PlaneBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& myPlanes= parent->components();
  std::stable_sort( myPlanes.begin(), myPlanes.end(), LessModZ());
}



