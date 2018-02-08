#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeParametersFromDD.h"


bool TelescopeParametersFromDD::build( const DDCompactView* cvp, PTelescopeParameters& ptp) {

  ptp.detIdShifts = dbl_to_int( DDVectorGetter::get( "telescopeDetIdShifts" ));
  ptp.detIdMasks = dbl_to_int( DDVectorGetter::get( "telescopeDetIdMasks" ));

  return true;
}
