// -*- C++ -*-
// Package:    SiPixelESProducers
// Class:      DetIdsAnalyzer
// Original Author:  V.Chiochia (adapted from the Strip version by G.Bruno)
//         Created:  Mon May 20 10:04:31 CET 2007

#include "Geometry/TrackerNumberingBuilder/plugins/DetIdsAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"




// TO DO: clean the includes

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DataFormats/GeometrySurface/interface/BoundSurface.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerDebugNavigator.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerStringToEnum.h"
#include "DetectorDescription/Core/interface/DDRoot.h"
#include "DetectorDescription/Core/interface/DDExpandedView.h"
#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDMaterial.h"

// output
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <bitset>





using namespace cms;
using namespace std;


DetIdsAnalyzer::DetIdsAnalyzer(const edm::ParameterSet& iConfig) {

  edm::LogInfo("DetIdsAnalyzer::DetIdsAnalyzer");

}



DetIdsAnalyzer::~DetIdsAnalyzer(){

   edm::LogInfo("DetIdsAnalyzer::~DetIdsAnalyzer");
}



void DetIdsAnalyzer::beginRun(const edm::Run &run , const edm::EventSetup &iSetup){

    // get the GeometricDet
    edm::ESHandle<GeometricDet> rDD;
    iSetup.get<IdealGeometryRecord>().get( rDD ); 

    std::vector<const GeometricDet*> sensors =  (*rDD).deepComponents();
    

    for (const GeometricDet* mySensor : sensors) {

      const double x = mySensor->translation().X();
      const double y = mySensor->translation().Y();
      const double z = mySensor->translation().Z();
      //mySensor->rho()
        
      std::cout << "sensor DetId = " << mySensor->geographicalID().rawId() << ", x = " << x << ", y = " << y << ", z = " << z  << ", phi = "  << mySensor->phi() * 180. / M_PI << std::endl;
    }
    
}


void DetIdsAnalyzer::beginJob() { }


void DetIdsAnalyzer::analyze(const edm::Event &, const edm::EventSetup &) { }


DEFINE_FWK_MODULE(DetIdsAnalyzer);
