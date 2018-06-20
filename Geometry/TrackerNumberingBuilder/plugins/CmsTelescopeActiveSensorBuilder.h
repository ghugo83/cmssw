#ifndef Geometry_TrackerNumberingBuilder_CmsTelescopeActiveSensorBuilder_H
# define Geometry_TrackerNumberingBuilder_CmsTelescopeActiveSensorBuilder_H

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
class CmsTelescopeActiveSensorBuilder : public CmsTrackerLevelBuilder {
public:
  CmsTelescopeActiveSensorBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
};

#endif
