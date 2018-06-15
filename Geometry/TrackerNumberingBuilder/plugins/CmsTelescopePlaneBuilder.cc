#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePlaneBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/ActiveSensorBuilder.h"

#include <bitset>

CmsTelescopePlaneBuilder::CmsTelescopePlaneBuilder() {}

void CmsTelescopePlaneBuilder::buildComponent( DDFilteredView& fv, GeometricDet* plane, std::string attribute ) {
  ActiveSensorBuilder myActiveSensorBuilder;

  GeometricDet* myModule = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  switch( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv ))) {
  case GeometricDet::Phase1PixelModule:
    // TEST
    std::cout << "Found a Phase1PixelModule:"
	      << "myModule DetId = " << myModule->geographicalID().rawId() 
	      << ", x = " << myModule->translation().X() 
	      << ", y = " << myModule->translation().Y()
	      << ", z = " << myModule->translation().Z()
	      << ", phi = "  << myModule->phi() * 180. / M_PI << std::endl;
    // END TEST
    myActiveSensorBuilder.build( fv, myModule, attribute);      
    break;
  default:
    edm::LogError( "CmsTelescopePlaneBuilder" ) << " ERROR - Could not find a Phase1PixelModule, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
  }
  
  plane->addComponent(myModule);
}


void CmsTelescopePlaneBuilder::sortNS( DDFilteredView& fv, GeometricDet* plane ) {  
  GeometricDet::ConstGeometricDetContainer& allModules = plane->components();
  std::stable_sort( allModules.begin(), allModules.end(), LessY());  // TO DO: use LessR() instead??

  for (uint32_t i = 0; i < allModules.size(); i++) {
    plane->component(i)->setGeographicalID(i+1);
  }

  if (allModules.empty() ){
    edm::LogError("CmsTelescopePlaneBuilder") << "Found no Phase1PixelModule within the Plane.";
  } 
}



