#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerITRingBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerITRingBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>
/**
 * Class which contructs Phase 2 Inner Tracker Barrel tilted rings (if any). 
 */
class CmsTrackerITRingBuilder : public CmsTrackerLevelBuilder {
  
 private:
  void sortNS(DDFilteredView& , GeometricDet*) override;
  void buildComponent(DDFilteredView& , GeometricDet*, std::string) override;

};

#endif
