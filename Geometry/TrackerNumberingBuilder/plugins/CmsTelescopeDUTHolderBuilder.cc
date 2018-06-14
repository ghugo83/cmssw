#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopeDUTHolderBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"

#include <bitset>  // TO DO: clean 

CmsTelescopeDUTHolderBuilder::CmsTelescopeDUTHolderBuilder() {}

void CmsTelescopeDUTHolderBuilder::buildComponent( DDFilteredView& fv, GeometricDet* dutHolder, std::string attribute ) {
  CmsDetConstruction myCmsDetBuilder;

  GeometricDet* myDUT = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::OTPhase2Stack:
    // TEST
    std::cout << "Found a DUT:"
	      << " myDUT DetId = " <<  myDUT->geographicalID().rawId() 
	      << ", x = " <<  myDUT->translation().X() 
	      << ", y = " <<  myDUT->translation().Y()
	      << ", z = " <<  myDUT->translation().Z()
	      << ", phi = "  <<  myDUT->phi() * 180. / M_PI << std::endl;
    // END TEST
    myCmsDetBuilder.build( fv, myDUT, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopeDUTHolderBuilder" ) << " ERROR - Could not find a OTPhase2Stack, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  dutHolder->addComponent( myDUT );
}



void CmsTelescopeDUTHolderBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allDUTHolders = parent->components();
  std::stable_sort( allDUTHolders.begin(), allDUTHolders.end(), LessZ());
}




