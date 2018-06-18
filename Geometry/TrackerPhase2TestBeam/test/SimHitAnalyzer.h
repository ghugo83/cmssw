#ifndef SIM_HIT_ANALYZER
#define SIM_HIT_ANALYZER

#include <unordered_map>
#include <string>

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TH1D.h"
#include "TH2D.h"

using namespace std;

class SimHitAnalyzer : public edm::EDAnalyzer
{
  public:
    SimHitAnalyzer (const edm::ParameterSet &);
    ~SimHitAnalyzer ();

    void analyze (const edm::Event &, const edm::EventSetup &);

  private:
    edm::Service<TFileService> fs_;
    unordered_map<string, TH1D *> oneDHists_;
    unordered_map<string, TH2D *> twoDHists_;

    edm::InputTag simHitsBarrelHighTof_;
    edm::InputTag simHitsBarrelLowTof_;
    edm::InputTag simHitsEndcapHighTof_;
     edm::InputTag simHitsEndcapLowTof_;

    edm::EDGetTokenT<vector<PSimHit> > simHitsBarrelHighTofToken_;
    edm::EDGetTokenT<vector<PSimHit> > simHitsBarrelLowTofToken_;
    edm::EDGetTokenT<vector<PSimHit> > simHitsEndcapHighTofToken_;
    edm::EDGetTokenT<vector<PSimHit> > simHitsEndcapLowTofToken_;
};

#endif
