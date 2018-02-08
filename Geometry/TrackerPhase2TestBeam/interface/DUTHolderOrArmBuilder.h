#ifndef Geometry_TrackerPhase2TestBeam_DUTHolderOrArmBuilder_H
#define Geometry_TrackerPhase2TestBeam_DUTHolderOrArmBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
//#include "FWCore/ParameterSet/interface/types.h"
//#include <string>


#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
//#include "DetectorDescription/Core/interface/DDFilteredView.h"

//#include "DataFormats/DetId/interface/DetId.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerPhase2TestBeam/interface/DUTHolderBuilder.h"
#include "Geometry/TrackerPhase2TestBeam/interface/PlaneBuilder.h"

//#include <bitset>





/**
 * Abstract Class to construct a Level in the hierarchy
 */
class DUTHolderOrArmBuilder : public CmsTrackerLevelBuilder
{
public:
  DUTHolderOrArmBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
