#ifndef Geometry_TrackerPhase2TestBeam_PlaneBuilder_H
# define Geometry_TrackerPhase2TestBeam_PlaneBuilder_H

//# include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
//# include "FWCore/ParameterSet/interface/types.h"
//# include <string>


//#include "DetectorDescription/Core/interface/DDFilteredView.h"
//#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
//#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
//#include "DataFormats/DetId/interface/DetId.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerPhase2TestBeam/interface/Phase1PixelModuleBuilder.h"

//#include <bitset>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class PlaneBuilder : public CmsTrackerLevelBuilder
{
public:
  PlaneBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
