#ifndef Geometry_TrackerPhase2TestBeam_DUTHolderBuilder_H
# define Geometry_TrackerPhase2TestBeam_DUTHolderBuilder_H

#include "Geometry/TrackerPhase2TestBeam/interface/DUTBuilder.h"

//#include <bitset>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class DUTHolderBuilder : public CmsTrackerLevelBuilder
{
public:
  DUTHolderBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
