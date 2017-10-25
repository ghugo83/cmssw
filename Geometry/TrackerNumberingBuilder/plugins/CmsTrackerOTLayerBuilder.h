#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerOTLayerBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerOTLayerBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>
/**
 * Class related to the geometry building of Phase 2 Outer Tracker layers.
 */
class CmsTrackerOTLayerBuilder : public CmsTrackerLevelBuilder {
  
 private:
  void sortNS(DDFilteredView& , GeometricDet*) override;
  void buildComponent(DDFilteredView& , GeometricDet*, std::string) override;

};

#endif
