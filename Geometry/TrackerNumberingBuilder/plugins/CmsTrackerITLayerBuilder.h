#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerITLayerBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerITLayerBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>
/**
 * Class which contructs Phase 2 Inner Tracker Barrel layers.
 */
class CmsTrackerITLayerBuilder : public CmsTrackerLevelBuilder {
  
 private:
  void sortNS(DDFilteredView& , GeometricDet*) override;
  void buildComponent(DDFilteredView& , GeometricDet*, std::string) override;

};

#endif
