#include "Geometry/TrackerPhase2TestBeam/interface/Phase1PixelModuleBuilder.h"


Phase1PixelModuleBuilder::Phase1PixelModuleBuilder() {}

void Phase1PixelModuleBuilder::buildComponent( DDFilteredView& fv, GeometricDet* plane, std::string attribute ) {
  ActiveSensorBuilder myActiveSensorBuilder;

  GeometricDet* phase1PixelModule = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::Phase1PixelModule:
    // TEST
    /*std::cout << "phase1PixelModule DetId = " << phase1PixelModule->geographicalID().rawId() 
	      << ", x = " << phase1PixelModule->translation().X() 
	      << ", y = " << phase1PixelModule->translation().Y()
	      << ", z = " << phase1PixelModule->translation().Z()
	      << ", phi = "  << phase1PixelModule->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myActiveSensorBuilder.build( fv, phase1PixelModule, attribute);      
    break;
  default:
    edm::LogError( "Phase1PixelModuleBuilder" ) << " ERROR - Could not find a Phase1PixelModule, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  plane->addComponent(phase1PixelModule);
}


void Phase1PixelModuleBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& myPhase1PixelModules = parent->components();
  std::stable_sort( myPhase1PixelModules.begin(), myPhase1PixelModules.end(), LessY());
}




