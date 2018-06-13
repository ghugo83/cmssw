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

void CmsTelescopeBuilder::buildComponent( DDFilteredView& fv, GeometricDet* allTelescopeContainers, std::string attribute ) {
  CmsTelescopeDUTHolderBuilder myDUTHolderBuilder;
  CmsTelescopePlaneBuilder myPlaneBuilder;

  GeometricDet* myTelescopeContainer = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
    // DUT holder
  case GeometricDet::DUTHolder:
    myDUTHolderBuilder.build( fv, myTelescopeContainer, attribute);      
    break;
    // Telescope plane
  case GeometricDet::Plane:
    // TEST        
    /*std::cout << "arm DetId = " << telescopeContainer->geographicalID().rawId() 
      << ", x = " << telescopeContainer->translation().X() 
      << ", y = " << telescopeContainer->translation().Y()
      << ", z = " << telescopeContainer->translation().Z()
      << ", phi = "  << telescopeContainer->phi() * 180. / M_PI << std::endl;*/
    // END TEST
    myPlaneBuilder.build( fv, myTelescopeContainer, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopeBuilder" ) << " ERROR - Could not find a DUTHolder or a Plane, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  allTelescopeContainers->addComponent( myTelescopeContainer );
}


void CmsTelescopeBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allTelescopeContainers = parent->components();
  std::stable_sort( allTelescopeContainers.begin(), allTelescopeContainers.end(), LessZ());
}
