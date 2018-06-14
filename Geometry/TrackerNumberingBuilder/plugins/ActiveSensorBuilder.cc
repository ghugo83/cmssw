#include "Geometry/TrackerNumberingBuilder/plugins/ActiveSensorBuilder.h"


ActiveSensorBuilder::ActiveSensorBuilder() {}

void ActiveSensorBuilder::buildComponent( DDFilteredView& fv, GeometricDet* parent, std::string attribute ) {

  GeometricDet* activeSensor  = new GeometricDet(&fv, theCmsTrackerStringToEnum.type(ExtractStringFromDDD::getString(attribute,&fv)));
  static const std::string isLower = "DUTInnerSensor";
  static const std::string isUpper = "DUTOuterSensor";
  static const std::string isPixel = "Phase1PixelSensor";

  uint32_t temp = 0;
  if (ExtractStringFromDDD::getString(isLower, &fv) == "true") {
    temp = 1;   
  } 

  else if (ExtractStringFromDDD::getString(isUpper,&fv) == "true") {
    temp = 2;
  } 

  else if (ExtractStringFromDDD::getString(isPixel,&fv) == "true") {
    temp = 0;
  }
  
  else {
    edm::LogError("ActiveSensorBuilder") << "Child is nor DUTInnerSensor nor DUTOuterSensor nor Phase1PixelSensor.";
  }
 
  activeSensor->setGeographicalID(DetId(temp));

  parent->addComponent(activeSensor);
}



