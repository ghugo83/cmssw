#include "Geometry/TrackerPhase2TestBeam/interface/DUTHolderBuilder.h"


DUTHolderBuilder::DUTHolderBuilder() {}

void DUTHolderBuilder::buildComponent( DDFilteredView& fv, GeometricDet* myDUTContainer, std::string attribute ) {
  DUTBuilder myDUTBuilder;

  GeometricDet* dutHolder = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::DUTHolder:
    myDUTBuilder.build( fv, dutHolder, attribute);      
    break;
  default:
    edm::LogError( "DUTHolderBuilder" ) << " ERROR - Could not find a DUTHolder, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  myDUTContainer->addComponent( dutHolder );
}


void DUTHolderBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& myDUTHolders = parent->components();
  std::stable_sort( myDUTHolders.begin(), myDUTHolders.end(), LessZ());
}




