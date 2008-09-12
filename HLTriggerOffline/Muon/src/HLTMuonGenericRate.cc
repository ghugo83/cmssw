 /** \class HLTMuonGenericRate
 *  Get L1/HLT efficiency/rate plots
 *
 *  \author M. Vander Donckt, J. Klukas  (copied from J. Alcaraz)
 */

#include "HLTriggerOffline/Muon/interface/HLTMuonGenericRate.h"
#include "HLTriggerOffline/Muon/interface/AnglesUtil.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// Collaborating Class Header
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace edm;
using namespace reco;
using namespace trigger;
using namespace l1extra;

typedef std::vector< edm::ParameterSet > Parameters;

/// Constructor
HLTMuonGenericRate::HLTMuonGenericRate(const ParameterSet& pset, 
				       int triggerIndex)
{

  Parameters triggerLists  = pset.getParameter<Parameters>
                             ("TriggerCollection");
  ParameterSet thisTrigger = triggerLists[triggerIndex];

  theL1CollectionLabel     = thisTrigger.getParameter<InputTag>
                             ("L1CollectionLabel");
  theHLTCollectionLabels   = thisTrigger.getParameter< std::vector<InputTag> >
                             ("HLTCollectionLabels");
  theL1ReferenceThreshold  = thisTrigger.getParameter<double>
                             ("L1ReferenceThreshold");    
  theHLTReferenceThreshold = thisTrigger.getParameter<double>
                             ("HLTReferenceThreshold");    
  theNumberOfObjects       = thisTrigger.getParameter<unsigned int>
                             ("NumberOfObjects");
  if ( useMuonFromGenerator ) 
    theGenLabel            = pset.getUntrackedParameter<InputTag>("GenLabel");
  if ( useMuonFromReco )
    theRecoLabel           = pset.getUntrackedParameter<InputTag>("RecoLabel");

  useMuonFromGenerator = pset.getParameter<bool>("UseMuonFromGenerator");
  useMuonFromReco      = pset.getParameter<bool>("UseMuonFromReco");

  thePtMin     = pset.getUntrackedParameter<double>      ("PtMin",  0.);
  thePtMax     = pset.getUntrackedParameter<double>      ("PtMax", 40.);
  theNbins     = pset.getUntrackedParameter<unsigned int>("Nbins", 40 );
  theMinPtCut  = pset.getUntrackedParameter<double>      ("MinPtCut",  5.0);
  theMaxEtaCut = pset.getUntrackedParameter<double>      ("MaxEtaCut", 2.1);

  motherParticleId = pset.getUntrackedParameter<int> ("MotherParticleId", 0);
  theNSigmas = pset.getUntrackedParameter<std::vector<double> >("NSigmas90");

  theNumberOfEvents   = 0;
  theNumberOfL1Events = 0;

  dbe_ = 0 ;
  if ( pset.getUntrackedParameter<bool>("DQMStore", false) ) {
    dbe_ = Service<DQMStore>().operator->();
    dbe_->setVerbose(0);
  }

  if ( pset.getUntrackedParameter<bool>("disableROOToutput", false) ) 
    theRootFileName="";
  else 
    theRootFileName = pset.getUntrackedParameter<std::string>("RootFileName");

  nL1Orphans  = 0;
  nHltOrphans = 0;

  if ( theL1CollectionLabel.label() == "hltSingleMuIsoL1Filtered" )
    makeNtuple = true;
  else makeNtuple = false;
  if ( makeNtuple ) {
    theFile    = new TFile  ("file.root","RECREATE");
    theNtuple  = new TNtuple("nt","data","genPt:genEta:genPhi:pt1:eta1:phi1:pt2:eta2:phi2:pt3:eta3:phi3:pt4:eta4:phi4:pt5:eta5:phi5");
  }

}



/// Destructor
HLTMuonGenericRate::~HLTMuonGenericRate()
{
  if ( makeNtuple ) {
    cout << "nL1Orphans: "  << nL1Orphans  << 
          ", nHltOrphans: " << nHltOrphans << endl;
    theFile->cd();
    theNtuple->Write();
    theFile->Write();
    theFile->Close();
  }
}



void HLTMuonGenericRate::analyze( const Event & iEvent )
{
  thisEventWeight = 1;
  NumberOfEvents->Fill(++theNumberOfEvents); // Sets ME<int> NumberOfEvents
  LogTrace( "HLTMuonVal" ) << "In analyze for L1 trigger " << 
    theL1CollectionLabel << " Event:" << theNumberOfEvents;  


  //////////////////////////////////////////////////////////////////////////
  // Get all generated and reconstructed muons, fill eta and phi histograms,
  // and save the highest pt from each collection

  double genMuonPt = -1;
  double recMuonPt = -1;

  genMatches.clear();
  recMatches.clear();  

  if (useMuonFromGenerator) {
    Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel("genParticles", genParticles);
    for ( size_t i = 0; i < genParticles->size(); i++ ) {
      const reco::GenParticle *genParticle = &(*genParticles)[i];
      const Candidate *mother = findMother(genParticle);
      int    momId  = ( mother ) ? mother->pdgId() : 0;
      int    id     = genParticle->pdgId();
      int    status = genParticle->status();
      double pt     = genParticle->pt();
      double eta    = genParticle->eta();
      if ( abs(id) == 13  && status == 1 && 
	   ( motherParticleId == 0 || abs(momId) == motherParticleId ) )
      {
	MatchStruct newMatchStruct;
	newMatchStruct.genCand = genParticle;
	genMatches.push_back(newMatchStruct);
	if (pt > theMinPtCut &&  pt > genMuonPt && fabs(eta) < theMaxEtaCut)
	  genMuonPt = pt;
      }
    }
  }

  Handle<reco::TrackCollection> muTracks;
  if (useMuonFromReco) {
    iEvent.getByLabel(theRecoLabel.label(), muTracks);    
    reco::TrackCollection::const_iterator muon;
    if  ( muTracks.failedToGet() ) {
      LogDebug("HLTMuonVal") << "No reco tracks to compare to";
      useMuonFromReco = false;
    } else {
      for ( muon = muTracks->begin(); muon != muTracks->end(); ++muon ) {
	if ( muon->pt() > theMinPtCut  && fabs(muon->eta()) < theMaxEtaCut ) {
	  float pt = muon->pt();
	  MatchStruct newMatchStruct;
	  newMatchStruct.recCand = &*muon;
	  recMatches.push_back(newMatchStruct);
	  if ( pt > recMuonPt ) recMuonPt = pt;
  } } } }
  
  if ( genMuonPt > 0 ) hPtPassGen[0]->Fill(genMuonPt, thisEventWeight);
  if ( recMuonPt > 0 ) hPtPassRec[0]->Fill(recMuonPt, thisEventWeight);

  LogTrace("HLTMuonVal") << "genMuonPt: " << genMuonPt << ", "  
                         << "recMuonPt: " << recMuonPt;

  //////////////////////////////////////////////////////////////////////////
  // Get the L1 and HLT trigger collections

  edm::Handle<trigger::TriggerEventWithRefs> triggerObj;
  iEvent.getByLabel("hltTriggerSummaryRAW", triggerObj); 

  if( !triggerObj.isValid() ) { 
    LogDebug("HLTMuonVal") << "RAW-type HLT results not found, skipping event";
    return;
  }

  // Output filter information
  const size_type numFilterObjects(triggerObj->size());
  LogTrace("HLTMuonVal") 
    << "Used Processname: " << triggerObj->usedProcessName() << "\n"
    << "Number of TriggerFilterObjects: " << numFilterObjects << "\n"
    << "The TriggerFilterObjects: #, tag";
  for ( size_type i = 0; i != numFilterObjects; ++i ) LogTrace("HLTMuonVal") 
    << i << " " << triggerObj->filterTag(i).encode();

  InputTag label; // Used as a shortcut for the current CollectionLabel

  // Get the L1 candidates //
  vector<L1MuonParticleRef> l1Cands;
  label = theL1CollectionLabel;
  LogTrace("HLTMuonVal") << "TriggerObject Size=" << triggerObj->size();
  if ( triggerObj->filterIndex(label) >= triggerObj->size() ) {
    LogTrace("HLTMuonVal") << "No L1 Collection with label " << label;
    return;
  }
  triggerObj->getObjects( triggerObj->filterIndex(label), 81, l1Cands );
  NumberOfL1Events->Fill( ++theNumberOfL1Events );

  // Get the HLT candidates //
  unsigned int numHltLabels = theHLTCollectionLabels.size();
  vector< vector<RecoChargedCandidateRef> > hltCands(numHltLabels);
  for ( unsigned int i = 0; i < numHltLabels; i++ ) {
    label = theHLTCollectionLabels[i];
    if ( triggerObj->filterIndex(label) >= triggerObj->size() )
      LogTrace("HLTMuonVal") <<"No HLT Collection with label "<< label.label();
    else
      triggerObj->getObjects(triggerObj->filterIndex(label),93,hltCands[i]);
  }

  for ( size_t i = 0; i < genMatches.size(); i++ ) {
    genMatches[i].l1Cand = 0;
    genMatches[i].hltCands.resize(numHltLabels);
    for ( size_t j = 0; j < numHltLabels; j++ ) 
      genMatches[i].hltCands[j] = 0;
  }
  for ( size_t i = 0; i < recMatches.size(); i++ ) {
    recMatches[i].l1Cand = 0;
    recMatches[i].hltCands.resize(numHltLabels);
    for ( size_t j = 0; j < numHltLabels; j++ )
      recMatches[i].hltCands[j] = 0;
  }

  //////////////////////////////////////////////////////////////////////////
  // Loop through L1 candidates, matching to gen particles 

  // cout << "Looking at L1" << endl;

  unsigned int numL1Cands = 0;

  for ( size_t i = 0; i < l1Cands.size(); i++ ) {

    L1MuonParticleRef l1Cand = L1MuonParticleRef( l1Cands[i] );
    double eta   = l1Cand->eta();
    double phi   = l1Cand->phi();
    double ptLUT = l1Cand->pt();  // L1 pt is taken from a lookup table
    double pt    = ptLUT + 0.001; // In case ptLUT, which is discrete, exactly
                                  // equals theL1ReferenceThreshold

    if ( pt > theL1ReferenceThreshold ) {
      double maxDeltaR = 0.4;
      numL1Cands++;
      if ( useMuonFromGenerator ){
	int match = findGenMatch( eta, phi, maxDeltaR );
	if ( match != -1 ) genMatches[match].l1Cand = &*l1Cand;
	else nL1Orphans++;
      }
      if ( useMuonFromReco ){
	int match = findRecMatch( eta, phi, maxDeltaR );
	if ( match != -1 ) recMatches[match].l1Cand = &*l1Cand;
      }
    }
  }

  if ( numL1Cands >= theNumberOfObjects ){
    if ( genMuonPt > 0 ) hPtPassGen[1]->Fill(genMuonPt, thisEventWeight);
    if ( recMuonPt > 0 ) hPtPassRec[1]->Fill(recMuonPt, thisEventWeight);
    //    hSteps->Fill(1.);  
  }


  //////////////////////////////////////////////////////////////////////////
  // Find HLT candidates for each module passing a pt threshold, fill
  // histograms, and match them to generated and reconstructed muons

  // cout << "Looking at HLT" << endl;

  for ( size_t  i = 0; i < numHltLabels; i++ ) { 
    unsigned int numFound = 0;
    for ( size_t candNum = 0; candNum < hltCands[i].size(); candNum++ ) {

      int triggerLevel = ( i < ( numHltLabels / 2 ) ) ? 2 : 3;
      double maxDeltaR = ( triggerLevel == 2 ) ? 0.05 : 0.015;

      RecoChargedCandidateRef hltCand = hltCands[i][candNum];
      double eta = hltCand->eta();
      double phi = hltCand->phi();
      double pt  = hltCand->pt();

      if ( pt > theHLTReferenceThreshold ) {
	numFound++;
	if ( useMuonFromGenerator ){
	  int match = findGenMatch( eta, phi, maxDeltaR );
	  if ( match != -1 ) genMatches[match].hltCands[i] = &*hltCand;
	  else nHltOrphans++;
	}
	if ( useMuonFromReco ){
	  int match  = findRecMatch( eta, phi, maxDeltaR );
	  if ( match != -1 ) recMatches[match].hltCands[i] = &*hltCand;
	}
      }
    }
    if ( numFound >= theNumberOfObjects ){
      if ( genMuonPt > 0 ) hPtPassGen[i+2]->Fill(genMuonPt, thisEventWeight);
      if ( recMuonPt > 0 ) hPtPassRec[i+2]->Fill(recMuonPt, thisEventWeight);
      //    hSteps->Fill(1.);  
    }

  }


  //////////////////////////////////////////////////////////////////////////
  // Fill histograms

  if ( makeNtuple ) {
    // cout << "Looking at hltSingleMuIso" << endl;
    for ( size_t i = 0; i < genMatches.size(); i++ ) {
      for ( int k = 0; k < 18; k++ ) ntParams[k] = -1;
      ntParams[0] =  genMatches[i].genCand->pt();
      ntParams[1] =  genMatches[i].genCand->eta();
      ntParams[2] =  genMatches[i].genCand->phi();
      if ( genMatches[i].l1Cand ) {
	ntParams[3] = genMatches[i].l1Cand->pt();
	ntParams[4] = genMatches[i].l1Cand->eta();
	ntParams[5] = genMatches[i].l1Cand->phi();
      }
      // cout << numHltLabels << endl;
      for ( size_t j = 0; j < genMatches[i].hltCands.size(); j++ ) {
	if ( genMatches[i].hltCands[j] ) {
	  // cout << (j*3+8) << ": " << genMatches[i].hltCands[j] << endl;
	  ntParams[(j*3+6)] = genMatches[i].hltCands[j]->pt();
	  ntParams[(j*3+7)] = genMatches[i].hltCands[j]->eta();
	  ntParams[(j*3+8)] = genMatches[i].hltCands[j]->phi();
	}
      }
      // cout << "About to fill the ntuple" << endl;
      theNtuple->Fill(ntParams);
    }
  }

  // cout << "Filling histograms" << endl;

  for ( size_t i = 0; i < genMatches.size(); i++  ) {
    hEtaPassGen[0]->Fill(genMatches[i].genCand->eta());
    hPhiPassGen[0]->Fill(genMatches[i].genCand->phi());
    if ( genMatches[i].l1Cand ) {
      hEtaPassGen[1]->Fill(genMatches[i].genCand->eta());
      hPhiPassGen[1]->Fill(genMatches[i].genCand->phi());
    }
    // cout << "Before hltCands" << endl;
    for ( size_t j = 0; j < genMatches[i].hltCands.size(); j++ ) {
      if ( genMatches[i].hltCands[j] ) {
	hEtaPassGen[j+2]->Fill(genMatches[i].genCand->eta());
	hPhiPassGen[j+2]->Fill(genMatches[i].genCand->phi());
  } } }

  // cout << "Filling rec histos" << endl;

  for ( size_t i = 0; i < recMatches.size(); i++  ) {
    hEtaPassRec[0]->Fill(recMatches[i].recCand->eta());
    hPhiPassRec[0]->Fill(recMatches[i].recCand->phi());
    if ( recMatches[i].l1Cand ) {
      hEtaPassRec[1]->Fill(recMatches[i].recCand->eta());
      hPhiPassRec[1]->Fill(recMatches[i].recCand->phi());
    }
    for ( size_t j = 0; j < genMatches[i].hltCands.size(); j++ ) {
      if ( recMatches[i].hltCands[j] ) {
	hEtaPassRec[j+2]->Fill(recMatches[i].recCand->eta());
	hPhiPassRec[j+2]->Fill(recMatches[i].recCand->phi());
  } } }

  // cout << "Done filling histograms" << endl;

}



const reco::Candidate* HLTMuonGenericRate::findMother( const reco::Candidate* p ) {
  const reco::Candidate* mother = p->mother();
  if ( mother ) {
    if ( mother->pdgId() == p->pdgId() ) return findMother(mother);
    else return mother;
  }
  else return 0;
}



int HLTMuonGenericRate::findGenMatch(double eta, double phi, double maxDeltaR )
{
  double bestDeltaR = maxDeltaR;
  int bestMatch = -1;
  for ( size_t i = 0; i < genMatches.size(); i++ ) {
    double dR = kinem::delta_R( eta, phi, 
				genMatches[i].genCand->eta(), 
				genMatches[i].genCand->phi() );
    if ( dR  < bestDeltaR ) {
      bestMatch  =  i;
      bestDeltaR = dR;
    }
  }
  return bestMatch;
}



int HLTMuonGenericRate::findRecMatch(double eta, double phi,  double maxDeltaR )
{
  double bestDeltaR = maxDeltaR;
  int bestMatch = -1;
  for ( size_t i = 0; i < recMatches.size(); i++ ) {
    double dR = kinem::delta_R( eta, phi, 
			        recMatches[i].recCand->eta(), 
				recMatches[i].recCand->phi() );
    if ( dR  < bestDeltaR ) {
      bestMatch  =  i;
      bestDeltaR = dR;
    }
  }
  return bestMatch;
}



void 
HLTMuonGenericRate::BookHistograms() 
{
  TString dirLabel, myLabel, newFolder, histName, histTitle;
  vector<TH1F*> h;
  if (dbe_) {
    dbe_->cd();
    dbe_->setCurrentFolder("HLT/Muon");

    TString theL1CollectionString( theL1CollectionLabel.encode().c_str() );

    dirLabel = theL1CollectionString;
    dirLabel.Resize( dirLabel.Index("L1") ); // Truncate starting at "L1"
    myLabel = theL1CollectionString;
    myLabel.Resize( myLabel.Index(":") );

    newFolder = "HLT/Muon/RateEfficiencies/" + dirLabel;
    dbe_->setCurrentFolder( newFolder.Data() );

    NumberOfEvents   = dbe_->bookInt("NumberOfEvents");
    NumberOfL1Events = dbe_->bookInt("NumberOfL1Events");

    dbe_->cd();
    newFolder = "HLT/Muon/Distributions/" + dirLabel;
    dbe_->setCurrentFolder( newFolder.Data() );

    if (useMuonFromGenerator){
      hPtPassGen.push_back( BookIt( "genPtPass_All", "Highest Gen Muon Pt" + myLabel,  theNbins, thePtMin, thePtMax) );
      hPtPassGen.push_back( BookIt( "genPtPass_" + myLabel, "Highest Gen Muon pt >= 1 L1 Candidate, label=" + myLabel,  theNbins, thePtMin, thePtMax) );
      hEtaPassGen.push_back( BookIt( "genEtaPass_All", "#eta of Gen Muons",  50, -2.1, 2.1) );
      hEtaPassGen.push_back( BookIt( "genEtaPass_" + myLabel, "#eta of Gen Muons Matched to L1, label=" + myLabel,  50, -2.1, 2.1) );
      hPhiPassGen.push_back( BookIt( "genPhiPass_All", "#phi of Gen Muons",  50, -3.15, 3.15) );
      hPhiPassGen.push_back( BookIt( "genPhiPass_" + myLabel, "#phi of Gen Muons Matched to L1, label=" + myLabel,  50, -3.15, 3.15) );
    }
    if (useMuonFromReco){
      hPtPassRec.push_back( BookIt( "recPtPass_All", "pt of Reco Muons" + myLabel,  theNbins, thePtMin, thePtMax) );
      hPtPassRec.push_back( BookIt( "recPtPass_" + myLabel, "pt of Reco Muons Matched to L1, label=" + myLabel,  theNbins, thePtMin, thePtMax) );
      hEtaPassRec.push_back( BookIt( "recEtaPass_All", "#eta of Reco Muons",  50, -2.1, 2.1) );
      hEtaPassRec.push_back( BookIt( "recEtaPass_" + myLabel, "#eta of Reco Muons Matched to L1, label=" + myLabel,  50, -2.1, 2.1) );
      hPhiPassRec.push_back( BookIt( "recPhiPass_All", "#phi of Reco Muons",  50, -3.15, 3.15) );
      hPhiPassRec.push_back( BookIt( "recPhiPass_" + myLabel, "#phi of Reco Muons Matched to L1, label=" + myLabel,  50, -3.15, 3.15) );
    }

    for (unsigned int i = 0; i < theHLTCollectionLabels.size(); i++) {
      dbe_->cd();
      newFolder = "HLT/Muon/Distributions/" + dirLabel;
      dbe_->setCurrentFolder( newFolder.Data() );
      myLabel = theHLTCollectionLabels[i].encode().c_str();
      myLabel.Resize( myLabel.Index(":") );
      if (useMuonFromGenerator) {
	hPtPassGen.push_back( BookIt( "genPtPass_" + myLabel, "Highest Gen Muon pt with >= 1 Candidate, label=" + myLabel, theNbins, thePtMin, thePtMax) );   
	hEtaPassGen.push_back( BookIt( "genEtaPass_" + myLabel, "#eta of Gen Muons Matched to HLT, label=" + myLabel,  50, -2.1, 2.1) );
	hPhiPassGen.push_back( BookIt( "genPhiPass_" + myLabel, "#phi of Gen Muons Matched to HLT, label=" + myLabel,  50, -3.15, 3.15) );
      }
      if (useMuonFromReco) {
	hPtPassRec.push_back( BookIt( "recPtPass_" + myLabel, "pt of Reco Muons Matched to HLT, label=" + myLabel, theNbins, thePtMin, thePtMax) );     
	hEtaPassRec.push_back( BookIt( "recEtaPass_" + myLabel, "#eta of Reco Muons Matched to HLT, label=" + myLabel,  50, -2.1, 2.1) );
	hPhiPassRec.push_back( BookIt( "recPhiPass_" + myLabel, "#phi of Reco Muons Matched to HLT, label=" + myLabel,  50, -3.15, 3.15) );
      }
    }
  }
}



MonitorElement* 
HLTMuonGenericRate::BookIt( TString name, TString title, 
			    int Nbins, float Min, float Max) 
{
  LogDebug("HLTMuonVal") << "Directory " << dbe_->pwd() << " Name " << 
                            name << " Title:" << title;
  TH1F *h = new TH1F( name, title, Nbins, Min, Max );
  h->Sumw2();
  return dbe_->book1D( name.Data(), h );
  delete h;
}



void 
HLTMuonGenericRate::WriteHistograms() 
{
  if ( theRootFileName.size() != 0 && dbe_ ) dbe_->save(theRootFileName);
   return;
}

