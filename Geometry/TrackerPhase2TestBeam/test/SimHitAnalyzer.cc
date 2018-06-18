#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "Geometry/TrackerPhase2TestBeam/test/SimHitAnalyzer.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 



SimHitAnalyzer::SimHitAnalyzer (const edm::ParameterSet &cfg) :
  simHitsBarrelHighTof_ (cfg.getParameter<edm::InputTag> ("simHitsBarrelHighTof")),
  simHitsBarrelLowTof_ (cfg.getParameter<edm::InputTag> ("simHitsBarrelLowTof")),
  simHitsEndcapHighTof_ (cfg.getParameter<edm::InputTag> ("simHitsEndcapHighTof")),
  simHitsEndcapLowTof_ (cfg.getParameter<edm::InputTag> ("simHitsEndcapLowTof")) {
  TH1::SetDefaultSumw2 ();

  twoDHists_["rhoPhi"] = fs_->make<TH2D> ("rhoPhi", ";x [cm];y [cm]", 500, -8.0, 8.0, 500, -8.0, 8.0);
  twoDHists_["ZY"] = fs_->make<TH2D> ("ZY", ";z [cm]; y [cm]", 5000, -40.0, 40.0, 5000, -8.0, 8.0);  // (ZY) plot
  //twoDHists_["rhoZ"] = fs_->make<TH2D> ("rhoZ", ";z [cm];#rho [cm]", 500, 70.0, 230.0, 500, 0.0, 10.0);

  simHitsBarrelHighTofToken_ = consumes<vector<PSimHit> > (simHitsBarrelHighTof_);
  simHitsBarrelLowTofToken_ = consumes<vector<PSimHit> > (simHitsBarrelLowTof_);
  simHitsEndcapHighTofToken_ = consumes<vector<PSimHit> > (simHitsEndcapHighTof_);
  simHitsEndcapLowTofToken_ = consumes<vector<PSimHit> > (simHitsEndcapLowTof_);
}


SimHitAnalyzer::~SimHitAnalyzer () {}


void SimHitAnalyzer::analyze (const edm::Event &event, const edm::EventSetup &setup) {
  edm::Handle<vector<PSimHit> > simHitsBarrelHighTof;
  event.getByToken (simHitsBarrelHighTofToken_, simHitsBarrelHighTof);
  edm::Handle<vector<PSimHit> > simHitsBarrelLowTof;
  event.getByToken (simHitsBarrelLowTofToken_, simHitsBarrelLowTof);
  edm::Handle<vector<PSimHit> > simHitsEndcapHighTof;
  event.getByToken (simHitsEndcapHighTofToken_, simHitsEndcapHighTof);
  edm::Handle<vector<PSimHit> > simHitsEndcapLowTof;
  event.getByToken (simHitsEndcapLowTofToken_, simHitsEndcapLowTof);




  
  edm::ESHandle<TrackerGeometry> theTrackerGeometry;
  setup.get<TrackerDigiGeometryRecord> ().get (theTrackerGeometry);
  const TrackerGeometry &theTracker (*theTrackerGeometry);





  for (const auto &simHit : *simHitsBarrelHighTof) {
    DetId theDetUnitId (simHit.detUnitId ());
    const GeomDet *theDet = theTracker.idToDet (theDetUnitId);

    const Global3DPoint& entry = theDet->surface().toGlobal(simHit.entryPoint());
    const Global3DPoint& exit = theDet->surface().toGlobal(simHit.exitPoint());
    std::cout << "SimHitAnalyzer::analyze simHitsBarrelHighTof  theDetUnitId.rawId() = " << theDetUnitId.rawId()
	      << " simHit.trackId() = " << simHit.trackId()
	      << " simHit.timeOfFlight() (ns) = " << simHit.timeOfFlight()
	      << " simHit.processType() = " << simHit.processType()
	      << " simHit.particleType() = " << simHit.particleType()
	      << " simHit.entryPoint() (cm) = LOCAL: " << simHit.entryPoint() << " GLOBAL: (" << entry.x() << "," << entry.y() << "," << entry.z() << ")"
	      << " simHit.exitPoint() (cm) = LOCAL: " << simHit.exitPoint() << " GLOBAL: (" << exit.x() << "," << exit.y() << "," << exit.z() << ")"
	      << " simHit.energyLoss() (GeV) = " << simHit.energyLoss()
	      << std::endl;
      
      double x, y, z;
      x = theDet->surface ().toGlobal (simHit.localPosition ()).x ();
      y = theDet->surface ().toGlobal (simHit.localPosition ()).y ();
      z = theDet->surface ().toGlobal (simHit.localPosition ()).z ();

      twoDHists_.at ("rhoPhi")->Fill (x, y);
      //twoDHists_.at ("rhoZ")->Fill (z, hypot (x, y));
      twoDHists_.at ("ZY")->Fill (z, y);
    }


  for (const auto &simHit : *simHitsBarrelLowTof) {
    DetId theDetUnitId (simHit.detUnitId ());
    const GeomDet *theDet = theTracker.idToDet (theDetUnitId);

    const Global3DPoint& entry = theDet->surface().toGlobal(simHit.entryPoint());
    const Global3DPoint& exit = theDet->surface().toGlobal(simHit.exitPoint());
    std::cout << "SimHitAnalyzer::analyze simHitsBarrelLowTof  theDetUnitId.rawId() = " << theDetUnitId.rawId()
	      << " simHit.trackId() = " << simHit.trackId()
	      << " simHit.timeOfFlight() (ns) = " << simHit.timeOfFlight()
	      << " simHit.processType() = " << simHit.processType()
	      << " simHit.particleType() = " << simHit.particleType()
	      << " simHit.entryPoint() (cm) = LOCAL: " << simHit.entryPoint() << " GLOBAL: (" << entry.x() << "," << entry.y() << "," << entry.z() << ")"
	      << " simHit.exitPoint() (cm) = LOCAL: " << simHit.exitPoint() << " GLOBAL: (" << exit.x() << "," << exit.y() << "," << exit.z() << ")"
	      << " simHit.energyLoss() (GeV) = " << simHit.energyLoss() 
	      << std::endl;

    double x, y, z;
      x = theDet->surface ().toGlobal (simHit.localPosition ()).x ();
      y = theDet->surface ().toGlobal (simHit.localPosition ()).y ();
      z = theDet->surface ().toGlobal (simHit.localPosition ()).z ();

      twoDHists_.at ("rhoPhi")->Fill (x, y);
      //twoDHists_.at ("rhoZ")->Fill (z, hypot (x, y));
      twoDHists_.at ("ZY")->Fill (z, y);
    }


  for (const auto &simHit : *simHitsEndcapHighTof) {
    DetId theDetUnitId (simHit.detUnitId ());
    const GeomDet *theDet = theTracker.idToDet (theDetUnitId);

    const Global3DPoint& entry = theDet->surface().toGlobal(simHit.entryPoint());
    const Global3DPoint& exit = theDet->surface().toGlobal(simHit.exitPoint());
    std::cout << "SimHitAnalyzer::analyze simHitsEndcapHighTof  theDetUnitId.rawId() = " << theDetUnitId.rawId()
	      << " simHit.trackId() = " << simHit.trackId()
	      << " simHit.timeOfFlight() (ns) = " << simHit.timeOfFlight()
	      << " simHit.processType() = " << simHit.processType()
	      << " simHit.particleType() = " << simHit.particleType() 
	      << " simHit.entryPoint() (cm) = LOCAL: " << simHit.entryPoint() << " GLOBAL: (" << entry.x() << "," << entry.y() << "," << entry.z() << ")"
	      << " simHit.exitPoint() (cm) = LOCAL: " << simHit.exitPoint() << " GLOBAL: (" << exit.x() << "," << exit.y() << "," << exit.z() << ")"  
	      << " simHit.energyLoss() (GeV) = " << simHit.energyLoss() 
	      << std::endl;

    //double x, y, z;
    double y, z;
    //x = theDet->surface ().toGlobal (simHit.localPosition ()).x ();
    y = theDet->surface ().toGlobal (simHit.localPosition ()).y ();
    z = theDet->surface ().toGlobal (simHit.localPosition ()).z ();

    twoDHists_.at ("ZY")->Fill (z, y);
  }


  for (const auto &simHit : *simHitsEndcapLowTof) {
    DetId theDetUnitId (simHit.detUnitId ());
    const GeomDet *theDet = theTracker.idToDet (theDetUnitId);

    const Global3DPoint& entry = theDet->surface().toGlobal(simHit.entryPoint());
    const Global3DPoint& exit = theDet->surface().toGlobal(simHit.exitPoint());
    std::cout << "SimHitAnalyzer::analyze simHitsEndcapLowTof  theDetUnitId.rawId() = " << theDetUnitId.rawId()
	      << " simHit.trackId() = " << simHit.trackId()
	      << " simHit.timeOfFlight() (ns) = " << simHit.timeOfFlight()
	      << " simHit.processType() = " << simHit.processType()
	      << " simHit.particleType() = " << simHit.particleType()
	      << " simHit.entryPoint() (cm) = LOCAL: " << simHit.entryPoint() << " GLOBAL: (" << entry.x() << "," << entry.y() << "," << entry.z() << ")"
	      << " simHit.exitPoint() (cm) = LOCAL: " << simHit.exitPoint() << " GLOBAL: (" << exit.x() << "," << exit.y() << "," << exit.z() << ")"
	      << " simHit.energyLoss() (GeV) = " << simHit.energyLoss() 
	      << std::endl;

    //double x, y, z;
    double y, z;
    //x = theDet->surface ().toGlobal (simHit.localPosition ()).x ();
    y = theDet->surface ().toGlobal (simHit.localPosition ()).y ();
    z = theDet->surface ().toGlobal (simHit.localPosition ()).z ();

    twoDHists_.at ("ZY")->Fill (z, y);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SimHitAnalyzer);
