#ifndef Geometry_TrackerPhase2TestBeam_DetIdBuilder_H
# define Geometry_TrackerPhase2TestBeam_DetIdBuilder_H

# include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerStringToEnum.h"
# include "FWCore/ParameterSet/interface/types.h"
# include <ostream>
#include <vector>
#include <array>


#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>


class GeometricDet;

/**
 * Class to build a geographicalId.
 */

class DetIdBuilder {
 public:
  DetIdBuilder(std::vector<int> detidShifts);
  void build(GeometricDet* telescope);

 private:
  void iterate(uint32_t parentId, GeometricDet* volume, int siblingCounter, int hierarchyLevel);

  std::vector<int> detIdShifts_;
  int numHierarchyLevels_;

  CmsTrackerStringToEnum theCmsTrackerStringToEnum_;
};

#endif
