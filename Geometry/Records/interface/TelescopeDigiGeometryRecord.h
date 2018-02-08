#ifndef RECORDS_TELESCOPEDIGIGEOMETRYRECORD_H
#define RECORDS_TELESCOPEDIGIGEOMETRYRECORD_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentErrorExtendedRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerSurfaceDeformationRcd.h"
#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/TelescopeTopologyRcd.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "boost/mpl/vector.hpp"

class TelescopeDigiGeometryRecord : 
  public edm::eventsetup::DependentRecordImplementation<TelescopeDigiGeometryRecord,
                boost::mpl::vector<IdealGeometryRecord,
                TrackerAlignmentRcd, 
                TrackerAlignmentErrorExtendedRcd,
                TrackerSurfaceDeformationRcd,
                GlobalPositionRcd,
                TelescopeTopologyRcd,
                PTelescopeParametersRcd> > {};

#endif /* RECORDS_TELESCOPEDIGIGEOMETRYRECORD_H */
