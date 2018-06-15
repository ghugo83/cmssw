#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopeDUTHolderBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "Geometry/TrackerNumberingBuilder/plugins/CmsDetConstruction.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ActiveSensorBuilder.h"

#include <bitset>  // TO DO: clean 

CmsTelescopeDUTHolderBuilder::CmsTelescopeDUTHolderBuilder() {}

void CmsTelescopeDUTHolderBuilder::buildComponent( DDFilteredView& fv, GeometricDet* dutHolder, std::string attribute ) {
  //CmsDetConstruction myCmsDetBuilder;
  ActiveSensorBuilder myActiveSensorBuilder;

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
    myActiveSensorBuilder.build( fv, myDUT, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopeDUTHolderBuilder" ) << " ERROR - Could not find a OTPhase2Stack, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  dutHolder->addComponent( myDUT );
}



void CmsTelescopeDUTHolderBuilder::sortNS( DDFilteredView& fv, GeometricDet* dutHolder ) {  
  GeometricDet::ConstGeometricDetContainer& allDUTs = dutHolder->components();
  std::stable_sort( allDUTs.begin(), allDUTs.end(), LessZ());

  for (uint32_t i = 0; i < allDUTs.size(); i++) {
    dutHolder->component(i)->setGeographicalID(i+1);                                                       
  }

  if (allDUTs.empty() ){
    edm::LogError("CmsTelescopeDUTHolderBuilder") << "Found no DUT within the DUTHolder.";
  } 
}




