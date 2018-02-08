#include "Geometry/TrackerPhase2TestBeam/interface/DUTBuilder.h"


DUTBuilder::DUTBuilder() {}

void DUTBuilder::buildComponent( DDFilteredView& fv, GeometricDet* myDUTHolder, std::string attribute ) {
  ActiveSensorBuilder myActiveSensorBuilder;

  GeometricDet* dut = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::DUT:
    myActiveSensorBuilder.build( fv, dut, attribute);      
    break;
  default:
    edm::LogError( "DUTBuilder" ) << " ERROR - Could not find a DUT, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  myDUTHolder->addComponent( dut );
}


void DUTBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& myDUTs = parent->components();
  std::stable_sort( myDUTs.begin(), myDUTs.end(), LessZ());
}




