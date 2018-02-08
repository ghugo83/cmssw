#ifndef Geometry_TrackerPhase2TestBeam_TelescopeDigiGeometryESModule_H
#define Geometry_TrackerPhase2TestBeam_TelescopeDigiGeometryESModule_H

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/TelescopeDigiGeometryRecord.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeometry.h"
#include <memory>

#include <string>


#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeomBuilderFromGeometricDet.h"

#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"


//#include "Geometry/Records/interface/PTrackerParametersRcd.h"
//#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"


#include "Geometry/TrackerPhase2TestBeam/interface/PTelescopeParameters.h"
//#include "DataFormats/TrackerCommon/interface/PTelescopeParameters.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeParametersFromDD.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "DataFormats/TrackerCommon/interface/TelescopeTopology.h"
#include "Geometry/Records/interface/TelescopeTopologyRcd.h"


// Alignments
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/Alignment/interface/AlignmentErrorsExtended.h"
#include "CondFormats/Alignment/interface/AlignmentSurfaceDeformations.h"
#include "CondFormats/Alignment/interface/DetectorGlobalPosition.h"
#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentErrorExtendedRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerSurfaceDeformationRcd.h"
#include "Geometry/CommonTopologies/interface/GeometryAligner.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include <memory>


namespace edm {
  class ConfigurationDescriptions;
}

class  TelescopeDigiGeometryESModule: public edm::ESProducer{
 public:
  TelescopeDigiGeometryESModule(const edm::ParameterSet & p);
  ~TelescopeDigiGeometryESModule() override; 
  std::shared_ptr<TelescopeGeometry> produce(const TelescopeDigiGeometryRecord &);

  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
  
 private:
  /// Called when geometry description changes
  std::shared_ptr<TelescopeGeometry> _telescope;
  const std::string alignmentsLabel_;
  const std::string myLabel_;
  bool applyAlignment_; // Switch to apply alignment corrections
  bool fromDDD_;
};


#endif




