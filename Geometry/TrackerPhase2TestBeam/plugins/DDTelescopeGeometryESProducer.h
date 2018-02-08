#ifndef Geometry_TrackerPhase2TestBeam_DDTelescopeGeometryESProducer_H
#define Geometry_TrackerPhase2TestBeam_DDTelescopeGeometryESProducer_H

#include "FWCore/Framework/interface/ESProducer.h"


#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeometryBuilder.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDVectorGetter.h"
#include "DetectorDescription/Core/interface/DDutils.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include <memory>

namespace edm {
  class ConfigurationDescriptions;
  class ParameterSet;
}

class GeometricDet;
class IdealGeometryRecord;

class DDTelescopeGeometryESProducer : public edm::ESProducer
{
public:
  DDTelescopeGeometryESProducer( const edm::ParameterSet & p );
  ~DDTelescopeGeometryESProducer( void ) override; 
  std::unique_ptr<GeometricDet> produce( const IdealGeometryRecord & );

  static void fillDescriptions( edm::ConfigurationDescriptions & descriptions );
  
private:
  bool fromDDD_;
};

#endif





