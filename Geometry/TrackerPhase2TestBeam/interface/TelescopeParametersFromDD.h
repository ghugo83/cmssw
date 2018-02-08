#ifndef Geometry_TrackerPhase2TestBeam_TelescopeParametersFromDD_h
#define Geometry_TrackerPhase2TestBeam_TelescopeParametersFromDD_h

#include "Geometry/TrackerPhase2TestBeam/interface/PTelescopeParameters.h"
//#include "DataFormats/TrackerCommon/interface/PTelescopeParameters.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDVectorGetter.h"
#include "DetectorDescription/Core/interface/DDutils.h"
#include <vector>

class DDCompactView;
class PTelescopeParameters;

class TelescopeParametersFromDD {
 public:
  TelescopeParametersFromDD() {};
  virtual ~TelescopeParametersFromDD() {};

  bool build( const DDCompactView*, PTelescopeParameters& );
};

#endif
