// -*- C++ -*-
// Package:    SiPixelESProducers
// Class:      SiPixelDetInfoFileWriter
// Original Author:  V.Chiochia (adapted from the Strip version by G.Bruno)
//         Created:  Mon May 20 10:04:31 CET 2007

#include "CalibTracker/SiPixelESProducers/interface/SiPixelDetInfoFileWriter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"






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
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
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


SiPixelDetInfoFileWriter::SiPixelDetInfoFileWriter(const edm::ParameterSet& iConfig) {

  
  edm::LogInfo("SiPixelDetInfoFileWriter::SiPixelDetInfoFileWriter");

  filePath_ = iConfig.getUntrackedParameter<std::string>("FilePath",std::string("SiPixelDetInfo.dat"));

}


SiPixelDetInfoFileWriter::~SiPixelDetInfoFileWriter(){

   edm::LogInfo("SiPixelDetInfoFileWriter::~SiPixelDetInfoFileWriter");
}



void SiPixelDetInfoFileWriter::beginRun(const edm::Run &run , const edm::EventSetup &iSetup){

  outputFile_.open(filePath_.c_str());

  if (outputFile_.is_open()){
  
  
    // get the GeometricDet
      edm::ESHandle<GeometricDet> rDD;
      iSetup.get<IdealGeometryRecord>().get( rDD ); 

     std::vector<const GeometricDet*> modules =  (*rDD).deepComponents();
      for(unsigned int i=0; i<modules.size();i++){  
        modules[i]->displayDet();
      }

  
  

    edm::ESHandle<TrackerGeometry> pDD;

    iSetup.get<TrackerDigiGeometryRecord>().get( pDD );

    edm::LogInfo("SiPixelDetInfoFileWriter::beginJob - got geometry  ")<<std::endl;    
    edm::LogInfo("SiPixelDetInfoFileWriter") <<" There are "<<pDD->detUnits().size() <<" detectors"<<std::endl;
    
    int nPixelDets = 0;

    for( const auto& it : pDD->detUnits()) {
  
      const PixelGeomDetUnit* mit = dynamic_cast<PixelGeomDetUnit const *>(it);

      if(mit!=nullptr){
	nPixelDets++;
      //const PixelTopology & topol = mit->specificTopology();       
      // Get the module sizes.
      //int nrows = topol.nrows();      // rows in x
      //int ncols = topol.ncolumns();   // cols in y      
      //uint32_t detid=(mit->geographicalId()).rawId();
      
      
      //outputFile_ << detid << " "<< ncols << " " << nrows << "\n";
      outputFile_ <<  "  "  << "\n"; 
      
      }
    }    
    outputFile_.close();
    edm::LogInfo("SiPixelDetInfoFileWriter::beginJob - Loop finished. ")<< nPixelDets << " Pixel DetUnits found " << std::endl;
  }
  
  else {

    edm::LogError("SiPixelDetInfoFileWriter::beginJob - Unable to open file")<<endl;
    return;
  
  }

}


void SiPixelDetInfoFileWriter::beginJob() {

}

void SiPixelDetInfoFileWriter::analyze(const edm::Event &, const edm::EventSetup &) {

}
