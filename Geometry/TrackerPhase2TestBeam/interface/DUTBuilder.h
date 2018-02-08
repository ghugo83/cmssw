#ifndef Geometry_TrackerPhase2TestBeam_DUTBuilder_H
# define Geometry_TrackerPhase2TestBeam_DUTBuilder_H

#include "Geometry/TrackerPhase2TestBeam/interface/ActiveSensorBuilder.h"

//#include <bitset>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class DUTBuilder : public CmsTrackerLevelBuilder
{
public:
  DUTBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
