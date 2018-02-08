#ifndef PTelescopeParametersRcd_H
#define PTelescopeParametersRcd_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "boost/mpl/vector.hpp"

class PTelescopeParametersRcd : public edm::eventsetup::DependentRecordImplementation<PTelescopeParametersRcd,
  boost::mpl::vector<IdealGeometryRecord> > {};

#endif // PTelescopeParameters_H
