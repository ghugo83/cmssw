#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"

#include "DataFormats/TrackerCommon/interface/TelescopeTopology.h"
#include "Geometry/Records/interface/TelescopeTopologyRcd.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeometry.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/Records/interface/TelescopeDigiGeometryRecord.h"
#include "Geometry/TrackerPhase2TestBeam/plugins/TelescopeTopologyEP.h"

#include <climits>
#include <iostream>

class TelescopeTopologyAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit TelescopeTopologyAnalyzer( const edm::ParameterSet& ) {};
  ~TelescopeTopologyAnalyzer() override {};
  
  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}
};

void TelescopeTopologyAnalyzer::analyze( const edm::Event &iEvent, const edm::EventSetup& iSetup) {
  //Retrieve telescope topology from geometry
  edm::ESHandle<TelescopeTopology> tTopoHandle;
  iSetup.get<TelescopeTopologyRcd>().get(tTopoHandle);
  const TelescopeTopology* const tTopo = tTopoHandle.product();

  typedef std::vector<DetId>                 DetIdContainer;

  edm::ESHandle<TelescopeGeometry> geo;
  iSetup.get<TelescopeDigiGeometryRecord>().get(geo);

  DetIdContainer allIds=geo->detIds();
 
  for( DetIdContainer::const_iterator id = allIds.begin(), detUnitIdEnd = allIds.end(); id != detUnitIdEnd; ++id ) {

    if ( id->det() == DetId::Telescope ) {

      std::cout << "tTopo->side(*id) = " << tTopo->side(*id) 
		<< " tTopo->plane(*id) = " << tTopo->plane(*id) 
		<< " tTopo->module(*id) = " << tTopo->module(*id) 
		<< std::endl;


      const GeomDet* geom = geo->idToDet(*id);
      std::cout << "  (phi, z, r) : ("
		<< geom->surface().position().phi()  << " , "
		<< geom->surface().position().z()    << " , "
		<< geom->surface().position().perp() << ")" << std::endl;

    }

  }

}

DEFINE_FWK_MODULE(TelescopeTopologyAnalyzer);

