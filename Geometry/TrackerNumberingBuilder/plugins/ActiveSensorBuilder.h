#ifndef Geometry_TrackerNumberingBuilder_ActiveSensorBuilder_H
# define Geometry_TrackerNumberingBuilder_ActiveSensorBuilder_H

#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"

#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/types.h"

#include <string>
#include <bitset>


/**
 * 
 */
class ActiveSensorBuilder : public CmsTrackerLevelBuilder {
public:
  ActiveSensorBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
};

#endif
