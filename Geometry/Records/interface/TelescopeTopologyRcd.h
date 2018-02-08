#ifndef Geometry_Records_TelescopeTopologyRcd
#define Geometry_Records_TelescopeTopologyRcd

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "boost/mpl/vector.hpp"

class TelescopeTopologyRcd :
public edm::eventsetup::DependentRecordImplementation<TelescopeTopologyRcd,
  boost::mpl::vector<IdealGeometryRecord, PTelescopeParametersRcd> > {};

#endif
