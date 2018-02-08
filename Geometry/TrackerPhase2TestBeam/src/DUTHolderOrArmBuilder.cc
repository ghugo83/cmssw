#include "Geometry/TrackerPhase2TestBeam/interface/DUTHolderOrArmBuilder.h"


#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.cc"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.cc"



DUTHolderOrArmBuilder::DUTHolderOrArmBuilder() {}

void DUTHolderOrArmBuilder::buildComponent( DDFilteredView& fv, GeometricDet* telescope, std::string attribute ) {

  DUTHolderBuilder myDUTHolderBuilder; // TO DO: why not having the build directly at construction time?
  PlaneBuilder myPlaneBuilder;
  GeometricDet* dutHolderOrArm = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
    // DUT holder
  case GeometricDet::DUTContainer:
    myDUTHolderBuilder.build(fv, dutHolderOrArm, attribute);      
    break;
    // Telescope arm
  case GeometricDet::Arm:
    // TEST        
    /*std::cout << "arm DetId = " << dutHolderOrArm->geographicalID().rawId() 
	      << ", x = " << dutHolderOrArm->translation().X() 
	      << ", y = " << dutHolderOrArm->translation().Y()
	      << ", z = " << dutHolderOrArm->translation().Z()
	      << ", phi = "  << dutHolderOrArm->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myPlaneBuilder.build( fv, dutHolderOrArm, attribute);      
    break;
  default:
    edm::LogError( "DUTHolderOrArmBuilder" ) << " ERROR - I was expecting a DUTContainer or an Arm, I got a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  telescope->addComponent(dutHolderOrArm);
}


void DUTHolderOrArmBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& myDutHolderOrArms = parent->components();
  std::stable_sort( myDutHolderOrArms.begin(), myDutHolderOrArms.end(), LessZ());
}





