// Package:    DQMTelescope/SimpleTracking
// Class:      SimpleTracking
//
// class SimpleTracking SimpleTracking.cc DQMTelescope/SimpleTracking/plugins/SimpleTracking.cc

// Original Author:  Jeremy Andrea
//      Updated by:  Nikkie Deelen
//         Created:  03.08.2018

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "RecoLocalTracker/Records/interface/TkPixelCPERecord.h"

#include "DQMTelescope/PixelTelescope/plugins/TelescopeTracks.h"

#include <cstring>
#include <string> 
#include <TH2F.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
#include <TH3.h>
#include <TPolyLine3D.h>
#include <Math/Vector3D.h>
#include <TMath.h>
#include <TGraph2D.h>
#include <TVirtualFitter.h>
#include <TGraph2D.h>
#include <TGraph2DErrors.h>
//
// class declaration
//



using reco::TrackCollection ;
using namespace ROOT::Math;






class SimpleTracking : public edm::one::EDAnalyzer<edm::one::SharedResources> {
  public:

    explicit SimpleTracking ( const edm::ParameterSet& ) ;
    ~SimpleTracking ( ) ;

    static void fillDescriptions ( edm::ConfigurationDescriptions& descriptions ) ;
   
  private:

    virtual void beginJob ( ) override ;
    virtual void analyze ( const edm::Event&, const edm::EventSetup& ) override ;
    virtual void endJob ( ) override ;

    // ----------member data ---------------------------
    edm::EDGetTokenT< TrackCollection >                        tracksToken_ ;
    edm::EDGetTokenT< edm::DetSetVector< PixelDigi > >         pixeldigiToken_ ;
    edm::EDGetTokenT< edmNew::DetSetVector< SiPixelCluster > > pixelclusterToken_ ;
    edm::EDGetTokenT< edmNew::DetSetVector< SiPixelRecHit > >  pixelhitToken_ ;      

    edm::Service<TFileService> fs ;
     
    std::map< uint32_t, TH1F* > DQM_ClusterCharge ;
    std::map< uint32_t, TH1F* > DQM_ClusterSize_X ;    
    std::map< uint32_t, TH1F* > DQM_ClusterSize_Y ;    
    std::map< uint32_t, TH1F* > DQM_ClusterSize_XY ;
    std::map< uint32_t, TH1F* > DQM_NumbOfClusters_per_Event;
    std::map< uint32_t, TH2F* > DQM_ClusterPosition ;
    //std::map< uint32_t, TH1F* > DQM_Hits_per_Pixel_per_Event ;

    // Correlation plots for the telescope
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation_X ;
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation_Y ;
    
    std::map< uint32_t, TH1F* > DQM_TrackPull_X ;    
    std::map< uint32_t, TH1F* > DQM_TrackPull_Y ;
    
    
    TH2F* testTrack;
    TH2F* testModule;
    TH2F* testModule2;
    TH2F* testModuleLayer;
    // 3D Tree 
    TTree* cluster3DTree ;

    Int_t      tree_runNumber ;
    Int_t      tree_lumiSection ;
    Int_t      tree_event ;
    Int_t      tree_detId ;
    Int_t      tree_cluster ;
    Double_t   tree_x ;
    //Double_t   tree_sizeX ;
    Double_t   tree_y ;
    //Double_t   tree_sizeY ;
    Double_t   tree_z ;
    TString    tree_modName ;
    Long64_t   tree_maxEntries = 1000000 ;
    
    
    // 3D Tree 
    TTree* TrackTree ;
    Int_t      tree_trackevent ;
    Double_t   tree_trackParam0;
    Double_t   tree_trackParam1;
    Double_t   tree_trackParam2;
    Double_t   tree_trackParam3;
    
    
    

    // detId versus moduleName
    std::map<int , TString> detId_to_moduleName ;
    std::map<TString , int> moduleName_to_detID ;
    
    
    std::map<TString , std::vector<double> > moduleName_to_position ;
    
    
    // 3D Tree 
    //TTree* simpleTracks ;
    
    
    std::vector<double> getTracks(std::vector<TVector3>, std::vector<TVector3>);
    void line(double , double *, double &, double &, double &) ; 
    //double distance2(double ,double ,double , double *);
    //void  SumDistance2(int &, double *, double & sum, double * par, int ) ; 
    
    
    bool checkCompatibility_X(float x1, float x2){if( fabs(x1-x2) > 0.5) return false; else return true;};
    bool checkCompatibility_Y(float y1, float y2){if( fabs(y1-y2) > 0.5) return false; else return true;};
    
    std::pair<int, int> getNextLayer(int);
    void doSeeding(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > , const TrackerGeometry *, 
    const PixelClusterParameterEstimator &, edm::ESHandle<TrackerGeometry> );
   
    std::vector<TelescopeTracks> theTeleTrackCollection;
    
    std::vector<std::pair<int, int> > LayersDefinition;
    
    void doPatternReco(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > , const TrackerGeometry *, 
    const PixelClusterParameterEstimator &, edm::ESHandle<TrackerGeometry> );
    
    
} ;

/////////////////////
// Class functions //
/////////////////////

SimpleTracking::SimpleTracking( const edm::ParameterSet& iConfig ) : tracksToken_( consumes<TrackCollection>( iConfig.getUntrackedParameter<edm::InputTag>( "tracks" ) ) ) {

  // detId vs Module name
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344200196, "M3090") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344201220, "M3124") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344462340, "M3082") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344463364, "M3175") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344724484, "M3009") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344725508, "M3057") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344986628, "M3027") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(344987652, "M3074") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(352588804, "M3192") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(352589828, "M3204") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(352850948, "M3226") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(352851972, "M3265") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(353113092, "M3023") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(353114116, "M3239") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(353375236, "M3164") ) ;
  detId_to_moduleName.insert( std::pair<uint32_t, TString>(353376260, "M3173") ) ;

   
  // Module name to detID
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3090", 344200196) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3124", 344201220) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3082", 344462340) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3175", 344463364) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3009", 344724484) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3057", 344725508) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3027", 344986628) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3074", 344987652) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3192", 352588804) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3204", 352589828) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3226", 352850948) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3265", 352851972) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3023", 353113092) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3239", 353114116) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3164", 353375236) ) ;
  moduleName_to_detID.insert( std::pair<TString, uint32_t>("M3173", 353376260) ) ;

  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3173"], moduleName_to_detID["M3164"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3239"], moduleName_to_detID["M3023"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3265"], moduleName_to_detID["M3226"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3204"], moduleName_to_detID["M3192"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3090"], moduleName_to_detID["M3124"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3082"], moduleName_to_detID["M3175"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3009"], moduleName_to_detID["M3057"]) );
  LayersDefinition.push_back(std::pair<int, int> (moduleName_to_detID["M3027"], moduleName_to_detID["M3074"]) );
  
  
   
  // x, y, z global position, 2 angles, skew (rotation around y), tilt (rotation around x).
  /*std::vector<double> pos_temp;
  pos_temp.push_back(0.);
  pos_temp.push_back(0.);
  pos_temp.push_back( 37);
  pos_temp.push_back( 20.);
  pos_temp.push_back( 30.);
  moduleName_to_position["M3027"]= pos_temp; 
  
  "M3074"  */
  
  


  TFileDirectory sub1 = fs->mkdir(  "run100000" ); // This does not make sense yet

  cluster3DTree = sub1.make<TTree>("cluster3DTree", "3D Cluster Tree");
  TrackTree = sub1.make<TTree>("TrackTree", "parameters of reco tracks");
  //simpleTracks = sub1.make<TTree>("simpleTracks",   "Tracks from simple tracking");
  
  TFileDirectory sub2 = sub1.mkdir( "dqmPlots" ) ;
  TFileDirectory sub3 = sub1.mkdir( "correlationPlots" ) ;  

//  TFileDirectory sub1 = fs -> mkdir ( "clusterTree3D" ) ; 
  
//  cluster3DTree = sub1.make<TTree> ("clusterTree3D", "3D Cluster Tree") ;
//  TFileDirectory sub2 = fs -> mkdir ( "dqmPlots" ) ;
//  TFileDirectory sub3 = fs -> mkdir ( "correlationPlots" ) ;  

  // Set branch addresses.
  cluster3DTree -> Branch ( "runNumber", &tree_runNumber ) ;
  cluster3DTree -> Branch ( "lumiSection", &tree_lumiSection ) ;
  cluster3DTree -> Branch ( "event", &tree_event ) ;
  cluster3DTree -> Branch ( "detId", &tree_detId ) ;
  cluster3DTree -> Branch ( "modName", &tree_modName ) ;
  cluster3DTree -> Branch ( "cluster", &tree_cluster ) ;
  cluster3DTree -> Branch ( "x", &tree_x ) ;
  cluster3DTree -> Branch ( "y", &tree_y ) ;
  cluster3DTree -> Branch ( "z", &tree_z ) ;
  cluster3DTree -> SetCircular ( tree_maxEntries ) ;
  
  TrackTree -> Branch ( "tree_trackevent",  &tree_trackevent ) ;
  TrackTree -> Branch ( "tree_trackParam0", &tree_trackParam0 ) ;
  TrackTree -> Branch ( "tree_trackParam1", &tree_trackParam1 ) ;
  TrackTree -> Branch ( "tree_trackParam2", &tree_trackParam2 ) ;
  TrackTree -> Branch ( "tree_trackParam3", &tree_trackParam3 ) ;
  
  
  testTrack  = sub2.make<TH2F>(  "Track Position on DUT", "Track Position on DUT" , 100., -3.0, 3.0, 100., -3.0, 3.0  ) ;
  testModule = sub2.make<TH2F>(  "Cluster Position on Module M3074", "Cluster Position on Module M3090 " , 100., -3.0, 3.0, 100., -3.0, 3.0  ) ;
  testModule2 = sub2.make<TH2F>(  "Cluster Position on Module M3124", "Cluster Position on Module M3124 " , 100., -3.0, 3.0, 100., -3.0, 3.0  ) ;
  testModuleLayer = sub2.make<TH2F>(  "Cluster Position on Layer", "Cluster Position on Layer " , 100., -3.0, 3.0, 100., -3.0, 3.0  ) ;
  
  
  for ( std::map<int, TString>::iterator it = detId_to_moduleName.begin(); it != detId_to_moduleName.end(); it++ ) {
    
    TString modulename = it -> second ;

    std::vector<TH2F *> tmp_vec_x ; // for the corr plots, hence the extra for loop
    std::vector<TH2F *> tmp_vec_y ; // for the corr plots, hence the extra for loop

    // Make the correlation plots
    for ( std::map<int, TString>::iterator jt = it; jt != detId_to_moduleName.end(); jt++ ) { // jt=it to make sure we do not have double plots.
   
      TString modulename0 = jt -> second ;

      TH2F* DQM_Correlation_X_tmp = sub3.make<TH2F>( ( "DQM_Correlation_X_" + modulename + "_" + modulename0).Data(), ( "X-Correlation between " + modulename + " and " + modulename0 ).Data(), 160., 0., 160., 160., 0., 160. ) ;
      TH2F* DQM_Correlation_Y_tmp = sub3.make<TH2F>( ( "DQM_Correlation_Y_" + modulename + "_" + modulename0).Data(), ( "Y-Correlation between " + modulename + " and " + modulename0 ).Data(), 416., 0., 416., 416., 0., 416. ) ;

      DQM_Correlation_X_tmp->GetXaxis()->SetTitle("x_" + modulename) ;
      DQM_Correlation_X_tmp->GetYaxis()->SetTitle("x_" + modulename0) ;
      DQM_Correlation_Y_tmp->GetXaxis()->SetTitle("y_" + modulename) ;
      DQM_Correlation_Y_tmp->GetYaxis()->SetTitle("y_" + modulename0) ;

      std::pair<uint32_t, uint32_t> modulePair = std::make_pair ( it->first, jt->first ) ;

      DQM_Correlation_X.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation_X_tmp ) ) ;
      DQM_Correlation_Y.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation_Y_tmp ) ) ;

    }//end for j 

    // Make the DQM plots
    TH1F* DQM_ClusterCharge_tmp = sub2.make<TH1F>( ( "DQM_ClusterCharge_" + modulename ).Data(), ( "Cluster charge for " + modulename).Data(), 100, 0., 100000. );
    TH1F* DQM_ClusterSize_X_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_X_" + modulename ).Data(), ( "X cluster size for " + modulename).Data(), 30, 0., 30. );
    TH1F* DQM_ClusterSize_Y_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_Y_" + modulename ).Data(), ( "Y cluster size for " + modulename ).Data(), 30, 0., 30. ) ;
    TH1F* DQM_ClusterSize_XY_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_XY_" + modulename ).Data(), ( "Cluster Size for "  + modulename ).Data(), 30, 0., 30. ) ;
    TH1F* DQM_NumbOfClusters_per_Event_tmp = sub2.make<TH1F>( ("DQM_NumbOfClusters_per_Event_" + modulename).Data(), ("number of clusters for "  + modulename).Data(), 30, 0., 30. );
    TH2F* DQM_ClusterPosition_tmp = sub2.make<TH2F>( ( "DQM_ClusterPosition_" + modulename ).Data(), ( "Cluster occupancy per col per row for " + modulename ).Data(), 416, 0. - 0.5, 416. - 0.5, 160, 0. - 0.5, 160. - 0.5 ) ;
    //TH1F* DQM_Hits_per_Pixel_per_Event_tmp = sub2.make<TH1F>( ( "DQM_HitsPerEventPerPixel_" + modulename ).Data(), ("Hits per event per pixel for " + modulename ).Data(), 66560, 0., 66560. ) ;  
    DQM_ClusterCharge_tmp -> GetXaxis ( ) ->SetTitle ( "Charge (electrons)" ) ;
    DQM_ClusterSize_X_tmp -> GetXaxis ( ) ->SetTitle ( "size (pixels)" ) ;
    DQM_ClusterSize_Y_tmp -> GetXaxis ( ) ->SetTitle ( "size (pixels)" ) ;	
    DQM_ClusterSize_XY_tmp -> GetXaxis ( ) ->SetTitle ( "size (pixels)" ) ; 
    DQM_ClusterPosition_tmp -> GetXaxis ( ) ->SetTitle ( "col" ) ; 
    DQM_ClusterPosition_tmp -> GetYaxis ( ) ->SetTitle ( "row" ) ; 
    DQM_NumbOfClusters_per_Event_tmp -> GetXaxis ( ) -> SetTitle ( "Number of clusters / event " ) ;
    DQM_NumbOfClusters_per_Event_tmp -> GetYaxis ( ) -> SetTitle ( "Count" ) ;
    //DQM_Hits_per_Pixel_per_Event_tmp -> GetXaxis ( ) -> SetTitle ( "Pixel" ) ;
    //DQM_Hits_per_Pixel_per_Event_tmp -> GetYaxis ( ) -> SetTitle ( "Total number of hits" ) ;

    DQM_ClusterCharge.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_ClusterCharge_tmp ) ) ;  
    DQM_ClusterSize_X.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_ClusterSize_X_tmp ) ) ;
    DQM_ClusterSize_Y.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_ClusterSize_Y_tmp ) ) ;
    DQM_ClusterSize_XY.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_ClusterSize_XY_tmp ) ) ;
    DQM_NumbOfClusters_per_Event.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_NumbOfClusters_per_Event_tmp ) );
    DQM_ClusterPosition.insert ( std::pair< uint32_t, TH2F* >( it->first, DQM_ClusterPosition_tmp ) ) ;      
    //DQM_Hits_per_Pixel_per_Event.insert ( std::pair< uint32_t, TH1F* >( it->first, DQM_Hits_per_Pixel_per_Event_tmp ) ) ;
    
    
    TH1F* DQM_TrackPull_X_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_X_" + modulename ).Data(), ( "pull track X for " + modulename).Data(), 100, -0.1, 0.1 );
    TH1F* DQM_TrackPull_Y_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_Y_" + modulename ).Data(), ( "pull track Y for " + modulename).Data(), 100, -0.1, 0.1 );
 
    DQM_TrackPull_X[it->first] = DQM_TrackPull_X_tmp;
    DQM_TrackPull_Y[it->first] = DQM_TrackPull_Y_tmp;
    

  }//end for it
  
  pixeldigiToken_    = consumes<edm::DetSetVector<PixelDigi> >        (iConfig.getParameter<edm::InputTag>("PixelDigisLabel"))   ;
  pixelclusterToken_ = consumes<edmNew::DetSetVector<SiPixelCluster> >(iConfig.getParameter<edm::InputTag>("PixelClustersLabel"));
  pixelhitToken_     = consumes<edmNew::DetSetVector<SiPixelRecHit> > (iConfig.getParameter<edm::InputTag>("PixelHitsLabel"))    ;
  
  //first = true;
  
}//end SimpleTracking()

SimpleTracking::~SimpleTracking()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void SimpleTracking::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {

  using namespace edm;
   
  EventID myEvId = iEvent.id();
   
  /* Handle<TrackCollection> tracks;
  iEvent.getByToken(tracksToken_, tracks);
  for(TrackCollection::const_iterator itTrack = tracks->begin();
    itTrack != tracks->end();
    ++itTrack) {
    // do something with track parameters, e.g, plot the charge.
    // int charge = itTrack->charge();
  }*/
  
  //get collection of digi
  edm::Handle<edm::DetSetVector<PixelDigi> > pixeldigis;
  iEvent.getByToken(pixeldigiToken_,pixeldigis  );
  
  //get collection of cluster
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters;
  iEvent.getByToken(pixelclusterToken_,pixelclusters  );
 
  //get collection or RecHits
  edm::Handle< edmNew::DetSetVector<SiPixelRecHit> > pixelhits;
  iEvent.getByToken(pixelhitToken_,pixelhits  );

  // Get the geometry of the tracker for converting the LocalPoint to a GlobalPoint
  edm::ESHandle<TrackerGeometry> tracker;
  iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
  const TrackerGeometry *tkgeom = &(*tracker); 
  
  // // Choose the CPE Estimator that will be used to estimate the LocalPoint of the cluster
  edm::ESHandle<PixelClusterParameterEstimator> cpEstimator;
  iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", cpEstimator);
  const PixelClusterParameterEstimator &cpe(*cpEstimator); 
  
  //---------------------------------
  //loop on digis
  //---------------------------------


  for( edm::DetSetVector<PixelDigi>::const_iterator DSViter=pixeldigis->begin(); DSViter!=pixeldigis->end(); DSViter++   ) { 
    
    edm::DetSet<PixelDigi>::const_iterator begin=DSViter->begin();
    edm::DetSet<PixelDigi>::const_iterator end  =DSViter->end();
    
    //auto id = DetId(DSViter->detId());
    
    std::map< int, int > numHits_per_Channel ;
    
    for(edm::DetSet<PixelDigi>::const_iterator iter=begin;iter!=end;++iter) {
  
      // First get the channel number of the hits
      //int channel = iter -> channel ( ) ;
      int rowN = iter -> row () ;
      int colN = iter -> column () ; 

      int channel = 0 ;
      if ( rowN == 0 ) channel = colN ;
      else channel = 160 + rowN * colN ;     

      // Add one to the number of hits of the right channel
      if ( numHits_per_Channel.count ( channel ) > 0 ) {
        numHits_per_Channel[ channel ] ++ ;
      } else {
        numHits_per_Channel.insert ( std::pair< int, int >( channel, 1 ) ) ;
      }//end if channel

    }//end for Digis   

    //for ( std::map< int, int >::iterator it = numHits_per_Channel.begin(); it != numHits_per_Channel.end(); it++  )  DQM_Hits_per_Pixel_per_Event[ id.rawId ( ) ] -> Fill ( it -> first ) ;

  }//end for Detectors

  //---------------------------------
  // Loop over the clusters
  //---------------------------------

  for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++   ) {

    edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
      
    auto id = DetId(DSViter->detId());

    for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter2=pixelclusters->begin(); DSViter2!=pixelclusters->end();DSViter2++   ) {

      edmNew::DetSet<SiPixelCluster>::const_iterator begin2=DSViter2->begin();
      edmNew::DetSet<SiPixelCluster>::const_iterator end2  =DSViter2->end();

      auto id2 = DetId(DSViter2->detId());

      for(edmNew::DetSet<SiPixelCluster>::const_iterator iter=begin;iter!=end;++iter) {

        float x = iter->x();                   // barycenter x position
        float y = iter->y();                   // barycenter y position
        //int row = x - 0.5, col = y - 0.5;
        int row = x , col = y ;

        for(edmNew::DetSet<SiPixelCluster>::const_iterator iter2=begin2;iter2!=end2;++iter2) {

          float x2 = iter2->x();                   // barycenter x position
          float y2 = iter2->y();                   // barycenter y position
          //int row2 = x2 - 0.5, col2 = y2 - 0.5;
          int row2 = x2, col2 = y2 ;
	  
          std::pair<uint32_t, uint32_t> modulePair = std::make_pair ( id.rawId(), id2.rawId() ) ;

          auto itHistMap = DQM_Correlation_X.find(modulePair);
	        
          if ( itHistMap == DQM_Correlation_X.end() ) continue;
          itHistMap->second->Fill ( row, row2 ) ;

          auto itHistMap2 = DQM_Correlation_Y.find(modulePair);
          if ( itHistMap2 == DQM_Correlation_Y.end() ) continue;
          itHistMap2->second->Fill ( col, col2 ) ;

        }//end for clusters2
      }//end for cluster
    }//end for first detectors2
  }//end for first detectors

  // Counting the number of clusters for this detector and this event
  std::map< uint32_t, int > clustersPerModule ;
  for ( std::map<int, TString>::iterator it = detId_to_moduleName.begin(); it != detId_to_moduleName.end(); it++ ) clustersPerModule.insert( std::pair< uint32_t, int >( it->first, 0 ) ) ;

  for ( edmNew::DetSetVector< SiPixelCluster >::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end(); DSViter++ ) {

    edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
      
    auto id = DetId(DSViter->detId());

    for ( edmNew::DetSet< SiPixelCluster >::const_iterator iter=begin; iter!=end; ++iter ) {
	
      float x = iter->x();                   // barycenter x position
      float y = iter->y();                   // barycenter y position
      int size = iter->size();               // total size of cluster (in pixels)
      int sizeX = iter->sizeX();             // size of cluster in x-iterrection
      int sizeY = iter->sizeY();             // size of cluster in y-iterrection
	
      int row = x-0.5, col = y -0.5;
      DQM_ClusterCharge[ id.rawId() ] -> Fill ( iter->charge() ) ;
      DQM_ClusterSize_X[ id.rawId() ] -> Fill ( sizeX ) ;   
      DQM_ClusterSize_Y[ id.rawId() ] -> Fill ( sizeY ) ;     
      DQM_ClusterSize_XY[ id.rawId() ] -> Fill ( size ) ;   
      DQM_ClusterPosition[ id.rawId() ] -> Fill ( col, row ) ;  
	
      //numberOfClustersForThisModule++ ;
      clustersPerModule[ id.rawId() ] ++ ;

    }//end for clusters in detector  
  }//end for detectors

  for ( std::map<uint32_t, int>::iterator it = clustersPerModule.begin(); it != clustersPerModule.end(); it++ ) DQM_NumbOfClusters_per_Event[ it->first ] -> Fill ( it->second ) ;

  //---------------------------------
  // Loop over the hits
  //---------------------------------

/*  for ( edmNew::DetSetVector< SiPixelRecHit >::const_iterator DSViter=pixelhits->begin(); DSViter!=pixelhits->end(); DSViter++ ) {
      
    edmNew::DetSet<SiPixelRecHit>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelRecHit>::const_iterator end  =DSViter->end();
    
    for ( edmNew::DetSet< SiPixelRecHit >::const_iterator iter=begin; iter!=end; ++iter ) {

      // Here we do something with the hits.
         
    }//end for DetSet
  }//end for DetSetVector
*/

  //---------------------------------
  // Fill the 3D tree
  //---------------------------------
  
  std::vector<TVector3> vectClust;
  std::vector<TVector3> vectClust_err;
  
  /*double xmodule = -10000;
  double ymodule = -10000;
  
  double xmodule2 = -10000;
  double ymodule2 = -10000;*/
  
  for ( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++ ) {

    edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();

    // Surface of (detId) and use the surface to convert 2D to 3D      
    auto detId = DetId(DSViter->detId());
    int iCluster=0;
    const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detId);
    LocalPoint lp(-9999., -9999., -9999.);

    // Then loop on the clusters of the module
    for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=begin; itCluster!=end; ++itCluster ) {

      PixelClusterParameterEstimator::ReturnType params = cpe.getParameters(*itCluster,*pixdet);
      lp = std::get<0>(params);

      const Surface& surface = tracker->idToDet(detId)->surface();
      GlobalPoint gp = surface.toGlobal(lp);

      double x=0, y=0, z=0;
      x = gp.x();
      y = gp.y();
      z = gp.z();
      
      double ex=0.05, ey=0.05, ez=5;
      
      TVector3 tmpGP(x, y, z);
      TVector3 tmpGP_err(ex, ey, ez);
      vectClust.push_back(tmpGP);
      vectClust_err.push_back(tmpGP_err);
      
      // Let's fill in the tree
      tree_runNumber = myEvId.run();
      tree_lumiSection = myEvId.luminosityBlock();
      tree_event = myEvId.event();
      tree_detId = detId;
      tree_cluster = iCluster++;	
      tree_x = x;
      tree_y = y;
      tree_z = z;
      tree_modName = detId_to_moduleName[detId];
      cluster3DTree->Fill();
      
      /*if(detId == 344200196) {
        xmodule = gp.x();
        ymodule = gp.y();
      } 
      if(detId == 344201220) {
        xmodule2 = gp.x();
        ymodule2 = gp.y();
      } */
      if(int(detId) == 344200196 || int(detId) == 344201220) testModuleLayer->Fill( gp.x(),  gp.y()  );
    } //end for clusters of the first detector
  } //end for first detectors
  
  
  
  //************************************************************
  //************************************************************
  //**************** This is for Tracking***********************
  //************************************************************
  //************************************************************
  //************************************************************
  
  
  //**************** Get collection of "Seeds" **********************
  //********  From pair of clusters in the 2 first layers ***********
  //***********  ask x and y position to be compatible **************
  
  doSeeding( pixelclusters, tkgeom, cpe,  tracker);
  //std::cout << "number of seeds " << theTeleTrackCollection.size() << std::endl;
  for(unsigned int itrack = 0; itrack< theTeleTrackCollection.size(); itrack++){
    std::vector<int > modulesID = theTeleTrackCollection[itrack].getModDetId();
    std::vector<TVector3 > theGP = theTeleTrackCollection[itrack].getGlobalPoints();
    //std::cout << "truc " << std::endl;
    for(unsigned int iHit=0; iHit < theGP.size(); iHit++){
      //std::cout << "det ID " << modulesID[iHit] << "  x " <<  theGP[iHit].X() << "  y " << theGP[iHit].Y()<< "  z " << theGP[iHit].Z()<< std::endl;
    }
  }
  //**************** Strat from the side, get to the next layer **********************
  
  doPatternReco( pixelclusters, tkgeom, cpe,  tracker);
  std::cout << "number of tracks " << theTeleTrackCollection.size() << std::endl;
  for(unsigned int itrack = 0; itrack< theTeleTrackCollection.size(); itrack++){
    
    
    
    
    std::vector<int > modulesID = theTeleTrackCollection[itrack].getModDetId();
    std::vector<TVector3 > theGP = theTeleTrackCollection[itrack].getGlobalPoints();
    if(theGP.size() < 6) continue; 
    std::cout << "********** new track ******* " << std::endl;
    std::cout << "parameters " << theTeleTrackCollection[itrack].getParameter(0)<< " " <<  
      theTeleTrackCollection[itrack].getParameter(1) << " " << 
      theTeleTrackCollection[itrack].getParameter(2)<< " " <<  
      theTeleTrackCollection[itrack].getParameter(3)<<  std::endl;
      
      tree_trackevent = myEvId.event();
      tree_trackParam0=theTeleTrackCollection[itrack].getParameter(0);
      tree_trackParam1=theTeleTrackCollection[itrack].getParameter(1);
      tree_trackParam2=theTeleTrackCollection[itrack].getParameter(2);
      tree_trackParam3=theTeleTrackCollection[itrack].getParameter(3);
      TrackTree->Fill();
    for(unsigned int iHit=0; iHit < theGP.size(); iHit++){
      std::cout << "modulesID " << detId_to_moduleName[modulesID[iHit]] << std::endl;
      double xtemp, ytemp, ztemp;
      double parFit[4] = {theTeleTrackCollection[itrack].getParameter(0), theTeleTrackCollection[itrack].getParameter(1), theTeleTrackCollection[itrack].getParameter(2)};
      theTeleTrackCollection[itrack].line(theGP[iHit].Z(), parFit, xtemp, ytemp, ztemp);
	  
      DQM_TrackPull_X[modulesID[iHit]]->Fill(xtemp - theGP[iHit].X());
      DQM_TrackPull_Y[modulesID[iHit]]->Fill(ytemp - theGP[iHit].Y());
    }
  }
  
  theTeleTrackCollection.clear();

}//end analyze()

// ------------ method called once each job just before starting event loop  ------------
void SimpleTracking::beginJob ( ) {
}//end void beginJob ( ) 

// ------------ method called once each job just after ending the event loop  ------------
void SimpleTracking::endJob ( ) {
}//end void endJobd ( ) 

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void SimpleTracking::fillDescriptions ( edm::ConfigurationDescriptions& descriptions ) {

  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);

}//end void fillDescriptions ( )




//**********************************
// determine the nagivation school
// ==> what is the next layex compatible with the track
//**********************************

std::pair<int, int> SimpleTracking::getNextLayer(int detid){
  
  //layer 3 : left 353376260, right  353375236 
  //layer 2 : left  353114116 , right  353113092 
  //layer 1 : left 352851972 , right  352850948 
  //layer 0 : left   352589828 , right 352588804 
  //layer -3 : left 344200196, right  344201220
  //layer -2 : left 344462340 , right 344463364 
  //layer -1 : left  344724484 , right 344725508 
  //Layer -0 : left 344986628 , right  344987652 
  
  
  std::pair<int, int> LayersDefinition;
  
  std::pair<int, int> nextModules;
  
  
  if( detid == moduleName_to_detID[""] || detid == moduleName_to_detID[""] ) {nextModules.first =moduleName_to_detID[""]; nextModules.second =moduleName_to_detID[""];};
  
  
  if(  detid ==  353376260 || detid ==  353375236 ) {nextModules.first =353114116; nextModules.second =353113092;};
  if(  detid ==  353114116 || detid == 353113092 ) {nextModules.first =352851972; nextModules.second =352850948;};
  if(  detid ==  352851972|| detid ==  352850948) {nextModules.first =352589828; nextModules.second =352588804;};
  if(  detid == 352589828 || detid ==  352588804) {nextModules.first =344200196; nextModules.second =344201220;};
  if(  detid ==  344200196|| detid == 344201220 ) {nextModules.first =344462340; nextModules.second =344463364;};
  if(  detid == 344462340 || detid ==  344463364) {nextModules.first =344724484; nextModules.second =344725508;};
  if(  detid ==  344724484|| detid ==  344725508) {nextModules.first =344986628; nextModules.second =344987652;};
  if(  detid ==  344986628 || detid ==  344987652) {nextModules.first =0; nextModules.second =0;};
  
  
  return nextModules;
  
}



//******************************************************
// pair of clusters 
// from the 2 first layes,
// with detla(X) and delta(Y) compatibile within 0.5 cm
//******************************************************

void SimpleTracking::doSeeding(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters,
 const TrackerGeometry *tkgeom,  const PixelClusterParameterEstimator &cpe, edm::ESHandle<TrackerGeometry> tracker){
  
  std::cout << "-------------" << std::endl;
  for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++   ) {

    edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
      
    auto detid = DetId(DSViter->detId());
    std::cout << "detID "  << int(detid) << std::endl;
    if(int(detid) != 353376260 && int(detid) != 353375236) continue;
      
      // Then loop on the clusters of the module
      for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=begin; itCluster!=end; ++itCluster ) {

        const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detid);
        LocalPoint lp(-9999., -9999., -9999.);

        PixelClusterParameterEstimator::ReturnType params = cpe.getParameters(*itCluster,*pixdet);
        lp = std::get<0>(params);

        const Surface& surface = tracker->idToDet(detid)->surface();
        GlobalPoint gp = surface.toGlobal(lp);

        double x=0, y=0, z=0;
        x = gp.x();
        y = gp.y();
        z = gp.z();
	
      for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter2=pixelclusters->begin(); DSViter2!=pixelclusters->end();DSViter2++   ) {
        
        auto detid2 = DetId(DSViter2->detId());
	
	if(int(detid2) == 353376260 || int(detid2) == 353375236) continue; 
	
    
        if(int(detid2) !=353114116  && int(detid2) != 353113092) continue;
	
        edmNew::DetSet<SiPixelCluster>::const_iterator begin2=DSViter2->begin();
        edmNew::DetSet<SiPixelCluster>::const_iterator end2  =DSViter2->end();
        
	
	
        for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster2=begin2; itCluster2!=end2; ++itCluster2 ) {
        
	
	  const PixelGeomDetUnit *pixdet2 = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detid2);
          LocalPoint lp2(-9999., -9999., -9999.);

          PixelClusterParameterEstimator::ReturnType params2 = cpe.getParameters(*itCluster2,*pixdet2);
          lp2 = std::get<0>(params2);

	  
	  
	  const Surface& surface2 = tracker->idToDet(detid2)->surface();
          GlobalPoint gp2 = surface2.toGlobal(lp2);

          double x2=0, y2=0, z2=0;
          x2 = gp2.x();
          y2 = gp2.y();
          z2 = gp2.z();
	  
	
	  std::cout << "detid  " << int(detid)  << "  x  " << x << " y " << y << " z " << z << std::endl;
	  std::cout << "detid2 " << int(detid2) << " x2 " << x2 << " y2 " << y2 << " z2 "<< z2 << std::endl;
	  
	  if(fabs(x-x2) < 0.5 && fabs(y-y2) < 0.5 && fabs(z-z2) < 1000 ){
            //std::pair<SiPixelCluster, SiPixelCluster> thePair;
            //thePair.first =  *(&DSViter) ;
            //thePair.second = *(&DSViter2);
            //thePixelPairSeed.push_back(thePair);
	    
	    
	    TelescopeTracks theseed;
	    TVector3 clust1(x,   y,  z);
	    TVector3 clust2(x2, y2, z2);
	    TVector3 clust_err(0.5, 0.5, 0.5);
	    
	    theseed.addGlobalPoint(clust1); theseed.addGlobalPointErr(clust_err); theseed.addCluster(*itCluster); theseed.addModDetId(int(detid));
	    theseed.addGlobalPoint(clust2); theseed.addGlobalPointErr(clust_err); theseed.addCluster(*itCluster2); theseed.addModDetId(int(detid2));
	    std::cout << "in fit tracks" << std::endl;
	    theseed.fitTrack();
	    
	    theTeleTrackCollection.push_back(theseed);
	    
	    double xtemp, ytemp, ztemp;
	    double parFit[4] = {theseed.getParameter(0), theseed.getParameter(1), theseed.getParameter(2)};
	    theseed.line(z, parFit, xtemp, ytemp, ztemp);
	    std::cout << "  check the fit " << xtemp << " " << ytemp << "  " << ztemp << std::endl;
	    std::cout << "  to be compared with " << x  << " " << y  << "  " << z  << std::endl;
	    std::cout << "-------------" << std::endl;
	    theseed.line(z2, parFit, xtemp, ytemp, ztemp);
	    std::cout << "  check the fit 2 " << xtemp << " " << ytemp << "  " << ztemp << std::endl;
	    std::cout << "  to be compared with " << x2  << " " << y2  << "  " << z2  << std::endl;
	    
	    
	  }
        }
       }
     }
   }
  
  
  
  
  
}

//******************************************************
// from pair of clusters 
// search for consecutive compatible clusters
// with detla(X) and delta(Y) compatibile within 0.5 cm
//******************************************************

void SimpleTracking::doPatternReco(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters,
 const TrackerGeometry *tkgeom,  const PixelClusterParameterEstimator &cpe, edm::ESHandle<TrackerGeometry> tracker){

  for(unsigned itrack=0; itrack<theTeleTrackCollection.size(); itrack++){
    
    
    for(int ilayer = 1; ilayer <7; ilayer++){
    
      for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++   ) {

      edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
      edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
        
      auto detid = DetId(DSViter->detId());
      
      //if(int(detid) != LayersDefinition[ilayer+1].first && int(detid) != LayersDefinition[ilayer+1].second) continue;
      if( int(detid) != LayersDefinition[ilayer+1].second) continue;
       
        TVector3 closestClust;
	SiPixelCluster closustSiPixelCulster;
	int ncluster = 0;
	int closestR = 1000;
	TVector3 clust_err(0.5, 0.5, 0.5);
        // Then loop on the clusters of the module
        for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=begin; itCluster!=end; ++itCluster ) {

          const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detid);
          LocalPoint lp(-9999., -9999., -9999.);

          PixelClusterParameterEstimator::ReturnType params = cpe.getParameters(*itCluster,*pixdet);
          lp = std::get<0>(params);
  
          const Surface& surface = tracker->idToDet(detid)->surface();
          GlobalPoint gp = surface.toGlobal(lp);

          double x=0, y=0, z=0;
          x = gp.x();
          y = gp.y();
          z = gp.z();
	  TVector3 clust(x,   y,  z);
	    
	  double xtemp, ytemp, ztemp;
	  double parFit[4] = {theTeleTrackCollection[itrack].getParameter(0), theTeleTrackCollection[itrack].getParameter(1), theTeleTrackCollection[itrack].getParameter(2)};
	  theTeleTrackCollection[itrack].line(z, parFit, xtemp, ytemp, ztemp);
	  double distanceR = pow( pow(xtemp -x , 2)+ pow(ytemp -y, 2) , 0.5);
	  if(fabs(xtemp -x ) < 0.5 &&  fabs(ytemp -y ) < 0.5 && (ncluster == 0 || closestR > distanceR  ) ){
	    closestClust.SetX(x);
	    closestClust.SetY(y);
	    closestClust.SetZ(z);
	    closustSiPixelCulster=*itCluster;
	  }
	  ncluster++;
	}
	
	theTeleTrackCollection[itrack].addGlobalPoint(closestClust); theTeleTrackCollection[itrack].addGlobalPointErr(clust_err); theTeleTrackCollection[itrack].addCluster(closustSiPixelCulster); theTeleTrackCollection[itrack].addModDetId(int(detid));
	theTeleTrackCollection[itrack].fitTrack();
	
	
      }
    }
  }
  
  
  
}



//define this as a plug-in
DEFINE_FWK_MODULE ( SimpleTracking ) ;
