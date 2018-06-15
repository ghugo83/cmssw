#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopeBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopeDUTHolderBuilder.h"
#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePlaneBuilder.h"

#include <bitset>  // TO DO: clean 

CmsTelescopeBuilder::CmsTelescopeBuilder() {}

void CmsTelescopeBuilder::buildComponent( DDFilteredView& fv, GeometricDet* dutContainerOrArm, std::string attribute ) {
  CmsTelescopeDUTHolderBuilder myDUTHolderBuilder;
  CmsTelescopePlaneBuilder myPlaneBuilder;

  GeometricDet* myDUTHolderOrPlane = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
    // DUT holder
  case GeometricDet::DUTHolder:
    // TEST
    std::cout << "Found a DUT Holder:"
	      << " myDUTHolder DetId = " <<  myDUTHolderOrPlane->geographicalID().rawId() 
	      << ", x = " <<  myDUTHolderOrPlane->translation().X() 
	      << ", y = " <<  myDUTHolderOrPlane->translation().Y()
	      << ", z = " <<  myDUTHolderOrPlane->translation().Z()
	      << ", phi = "  <<  myDUTHolderOrPlane->phi() * 180. / M_PI << std::endl;
    // END TEST
    myDUTHolderBuilder.build( fv, myDUTHolderOrPlane, attribute);      
    break;
    // Telescope plane
  case GeometricDet::Plane:
    // TEST  
    std::cout << "Found a Plane:"      
	      << " plane DetId = " << myDUTHolderOrPlane->geographicalID().rawId() 
      << ", x = " << myDUTHolderOrPlane->translation().X() 
      << ", y = " << myDUTHolderOrPlane->translation().Y()
      << ", z = " << myDUTHolderOrPlane->translation().Z()
      << ", phi = "  << myDUTHolderOrPlane->phi() * 180. / M_PI << std::endl;
    // END TEST
    myPlaneBuilder.build( fv, myDUTHolderOrPlane, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopeBuilder" ) << " ERROR - Could not find a DUTHolder or a Plane, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  dutContainerOrArm->addComponent( myDUTHolderOrPlane );
}


void CmsTelescopeBuilder::sortNS( DDFilteredView& fv, GeometricDet* telescopeSubStructure) {  
  GeometricDet::ConstGeometricDetContainer& allDutHoldersOrPlanes = telescopeSubStructure->components();
  std::stable_sort( allDutHoldersOrPlanes.begin(), allDutHoldersOrPlanes.end(), LessModZ());

  for (uint32_t i = 0; i < allDutHoldersOrPlanes.size(); i++) {
    telescopeSubStructure->component(i)->setGeographicalID(i+1);
  }

  if (allDutHoldersOrPlanes.empty() ){
    edm::LogError("CmsTelescopeBuilder") << "Found no DUTHolder or Plane within the TelescopeSubStructure.";
  } 
}
