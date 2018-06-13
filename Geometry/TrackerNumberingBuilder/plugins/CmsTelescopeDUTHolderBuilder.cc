#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopeDUTHolderBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"

#include <bitset>  // TO DO: clean 

CmsTelescopeDUTHolderBuilder::CmsTelescopeDUTHolderBuilder() {}

void CmsTelescopeDUTHolderBuilder::buildComponent( DDFilteredView& fv, GeometricDet* allDUTHolders, std::string attribute ) {
  CmsDetConstruction myCmsDetBuilder;

  GeometricDet* myDUTHolder = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::DetUnit:
    myCmsDetBuilder.build( fv, myDUTHolder, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopeDUTHolderBuilder" ) << " ERROR - Could not find a DUTHolder, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  allDUTHolders->addComponent( myDUTHolder );
}


void CmsTelescopeDUTHolderBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allDUTHolders = parent->components();
  std::stable_sort( allDUTHolders.begin(), allDUTHolders.end(), LessZ());
}




