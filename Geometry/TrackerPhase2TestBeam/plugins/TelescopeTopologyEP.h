#ifndef GEOMETRY_TRACKERPHASE2TESTBEAM_TELESCOPETOPOLOGYEP_H
#define GEOMETRY_TRACKERPHASE2TESTBEAM_TELESCOPETOPOLOGYEP_H

#include "memory"
#include "FWCore/Framework/interface/ESProducer.h"


#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDVectorGetter.h"
#include "DetectorDescription/Core/interface/DDutils.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include "DataFormats/TrackerCommon/interface/TelescopeTopology.h"
#include "Geometry/Records/interface/TelescopeTopologyRcd.h"

#include "Geometry/TrackerPhase2TestBeam/interface/PTelescopeParameters.h"
//#include "DataFormats/TrackerCommon/interface/PTelescopeParameters.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeParametersFromDD.h"


namespace edm {
  class ConfigurationDescriptions;
}

class TelescopeTopologyEP : public edm::ESProducer
{
public:
  TelescopeTopologyEP( const edm::ParameterSet & );
  ~TelescopeTopologyEP( void ) override;

  typedef std::shared_ptr<TelescopeTopology> ReturnType;

  static void fillDescriptions( edm::ConfigurationDescriptions & descriptions );
    
  ReturnType produce( const TelescopeTopologyRcd & );

private:
  void fillScheme(const PTelescopeParameters& ptp);
    
  const unsigned int numTelescopeHierarchyLevels_ = 5;
  TelescopeTopology::TelescopeScheme telescopeScheme_;
};

#endif
