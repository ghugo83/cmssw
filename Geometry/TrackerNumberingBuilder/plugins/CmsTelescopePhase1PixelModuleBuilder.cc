#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelModuleBuilder.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTelescopePhase1PixelWaferBuilder.h"

#include <bitset>

CmsTelescopePhase1PixelModuleBuilder::CmsTelescopePhase1PixelModuleBuilder() {}

void CmsTelescopePhase1PixelModuleBuilder::buildComponent( DDFilteredView& fv, GeometricDet* module, std::string attribute ) {
  CmsTelescopePhase1PixelWaferBuilder myPhase1PixelWaferBuilder;

  GeometricDet* myWafer = new GeometricDet( &fv, theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )));
  if ( theCmsTrackerStringToEnum.type( ExtractStringFromDDD::getString( attribute, &fv )) == GeometricDet::Phase1PixelWafer) {
    // TEST
    std::cout << "Found a Phase1PixelWafer:"
	      << "myPhase1PixelWafer DetId = " << myWafer->geographicalID().rawId() 
	      << ", x = " << myWafer->translation().X() 
	      << ", y = " << myWafer->translation().Y()
	      << ", z = " << myWafer->translation().Z()
	      << ", phi = "  << myWafer->phi() * 180. / M_PI << std::endl;
    // END TEST
    myPhase1PixelWaferBuilder.build( fv, myWafer, attribute);      
    // break;
    //default:
    //edm::LogError( "CmsTelescopePhase1PixelModuleBuilder" ) << " ERROR - Could not find a Phase1PixelWafer, but found a " << ExtractStringFromDDD::getString( attribute, &fv );
    //}
  
    module->addComponent(myWafer);
  }
}



void CmsTelescopePhase1PixelModuleBuilder::sortNS( DDFilteredView& fv, GeometricDet* parent ) {  
  GeometricDet::ConstGeometricDetContainer& allModules = parent->components();
  std::stable_sort( allModules.begin(), allModules.end(), LessY());  // TO DO: one one wafer per module!!!
}




