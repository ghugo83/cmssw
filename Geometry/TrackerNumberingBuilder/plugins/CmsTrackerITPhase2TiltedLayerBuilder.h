#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerITPhase2TiltedLayerBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerITPhase2TiltedLayerBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>
/**
 * Class related to the geometry building of Phase 2 Inner Tracker layers.
 */
class CmsTrackerITPhase2TiltedLayerBuilder : public CmsTrackerLevelBuilder {
  
 private:
  void sortNS(DDFilteredView& , GeometricDet*) override;
  void buildComponent(DDFilteredView& , GeometricDet*, std::string) override;

};

#endif
