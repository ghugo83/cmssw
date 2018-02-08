#ifndef Geometry_TrackerPhase2TestBeam_TelescopeParametersESModule_H
#define Geometry_TrackerPhase2TestBeam_TelescopeParametersESModule_H

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include <memory>


#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeParametersFromDD.h"
#include "Geometry/TrackerPhase2TestBeam/interface/PTelescopeParameters.h"
//#include "DataFormats/TrackerCommon/interface/PTelescopeParameters.h"


namespace edm {
  class ConfigurationDescriptions;
}


class  TelescopeParametersESModule: public edm::ESProducer
{
 public:
  TelescopeParametersESModule( const edm::ParameterSet & );
  ~TelescopeParametersESModule( void ) override;

  typedef std::shared_ptr<PTelescopeParameters> ReturnType;

  static void fillDescriptions( edm::ConfigurationDescriptions & );
  
  ReturnType produce( const PTelescopeParametersRcd & );
};

#endif
