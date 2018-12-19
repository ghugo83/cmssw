// Package:    DQMTelescope/SimpleTracking
// Class:      SimpleTracking
//
// class SimpleTracking SimpleTracking.cc DQMTelescope/SimpleTracking/plugins/SimpleTracking.cc

// Original Author:  Jeremy Andrea
//      Updated by:  Nikkie Deelen
//         Created:  03.08.2018

//      Updated by:  Patrick Asenov


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
#include "DQMTelescope/PixelTelescope/plugins/SimplePlane.h"




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
#include <iostream>
#include <fstream>
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
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation2_X ;
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation2_Y ;
/*
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation3_X ;
    std::map< std::pair<uint32_t, uint32_t>, TH2F*> DQM_Correlation3_Y ;
*/
    
    std::map< uint32_t, TH1F* > DQM_TrackPull_X ;    
    std::map< uint32_t, TH1F* > DQM_TrackPull_Y ;
    std::map< uint32_t, TH1F* > DQM_TrackPull2_X ;    
    std::map< uint32_t, TH1F* > DQM_TrackPull2_Y ;

    std::map< uint32_t, TH2F*> DQM_Corr_dX_X ;
    std::map< uint32_t, TH2F*> DQM_Corr_dY_Y ;
    
    TH1F* DQM_NumbOfTracks_per_Event;   
    TH1F* distanceR_per_Track;   
    TH1F* deltaX_per_Track;
    TH1F* deltaY_per_Track;

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
    Double_t   tree_x2 ;
    Double_t   tree_y2 ;
    Double_t   tree_z2 ;
    Double_t   tree_xloc ;
    Double_t   tree_yloc ;
    Double_t   tree_zloc ;
    Double_t   tree_xbary ;
    Double_t   tree_ybary ;
    Double_t   tree_zbary ;
    TString    tree_modName ;
    Long64_t   tree_maxEntries = 1000000 ;
    
    
    // 3D Tree 
    TTree* TrackTree ;
    Int_t      tree_trackevent ;
    Double_t   tree_trackParam0;
    Double_t   tree_trackParam1;
    Double_t   tree_trackParam2;
    Double_t   tree_trackParam3;
    Double_t   tree_trackParam4;
    Double_t   tree_trackParam5;
    Double_t   tree_chi2;
    Int_t      tree_npoints ;
    
    
    

    // detId versus moduleName
    std::map<int , TString> detId_to_moduleName ;
    std::map<TString , int> moduleName_to_detID ;
    
    
    std::map<TString , std::vector<double> > moduleName_to_position ;
    
    
    // 3D Tree 
    //TTree* simpleTracks ;
    
    
    std::vector<double> getTracks(std::vector<TVector3>, std::vector<TVector3>);
//    void line(double , double *, double &, double &, double &) ; 
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
    
    void ApplyAlignment(int detId, double &xaligned, double &yaligned);
    
    //std::cout << "ST1: voids, consts, etc. defined";
    
    //simple implementation of the geometry
    std::vector<SimplePlane > theSimpleLayers;
    
   int noisy_det[1000];
   int noisy_barx[1000];
   int noisy_bary[1000];
   int inoisy;
    
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
  
  //std::cout << "ST2: moduleNames to det ID OK" << std::endl;

    
  //layer 3 : left 353376260, right  353375236 
  //layer 2 : left  353114116 , right  353113092 
  //layer 1 : left 352851972 , right  352850948 
  //layer 0 : left   352589828 , right 352588804 
  //layer -3 : left 344200196, right  344201220
  //layer -2 : left 344462340 , right 344463364 
  //layer -1 : left  344724484 , right 344725508 
  //Layer -0 : left 344986628 , right  344987652 
  

  //Caroline: z values to be checked
  //SimplePlane theplane1( z, layerNum, detID1,  detID2);
  // for run 100208
  /*
  theSimpleLayers.push_back( SimplePlane(14.0 + 10.9 + 12.3 + 5.0,  1, 353376260,  353375236));
  theSimpleLayers.push_back( SimplePlane(14.0 + 10.9 + 12.3,        2, 353114116,  353113092));
  theSimpleLayers.push_back( SimplePlane(14.0 + 10.9,               3, 352851972,  352850948));
  theSimpleLayers.push_back( SimplePlane(14.0,                      4, 352589828,  352588804));
  theSimpleLayers.push_back( SimplePlane(-20.5,                     5, 344200196,  344201220));
  theSimpleLayers.push_back( SimplePlane(-20.5 - 5.0,               6, 344462340,  344463364));
  theSimpleLayers.push_back( SimplePlane(-20.5 - 5.0 - 13.5,        7, 344724484,  344725508));
  theSimpleLayers.push_back( SimplePlane(-20.5 - 5.0 - 13.5 - 10.2, 8, 344986628,  344987652));
  */
  // for run 100328
  // ideal case
  theSimpleLayers.push_back( SimplePlane(50., 0., 17.3 + 5.0 + 5.0 + 5.0,   1, 353376260,  353375236) );
  theSimpleLayers.push_back( SimplePlane(50., 0., 17.3 + 5.0 + 5.0,         2, 353114116,  353113092) );
  theSimpleLayers.push_back( SimplePlane(50., 0., 17.3 + 5.0,               3, 352851972,  352850948) );
  theSimpleLayers.push_back( SimplePlane(50., 0., 17.3,                     4, 352589828,  352588804) );
  theSimpleLayers.push_back( SimplePlane(50., 0., -17.3,                    5, 344200196,  344201220) );
  theSimpleLayers.push_back( SimplePlane(50., 0., -17.3 - 5.0,              6, 344462340,  344463364) );
  theSimpleLayers.push_back( SimplePlane(50., 0., -17.3 - 5.0 - 5.0,        7, 344724484,  344725508) );
  theSimpleLayers.push_back( SimplePlane(50., 0., -17.3 - 5.0 - 5.0 - 5.0,  8, 344986628,  344987652) );

  //apply alignment
  for(unsigned int i=0; i<theSimpleLayers.size(); i++) theSimpleLayers[i].applyAlignment();
  //apply rotation
  for(unsigned int i=0; i<theSimpleLayers.size(); i++) theSimpleLayers[i].doRotation();


 // std::cout << "post doRotation " << std::endl;
 // for(unsigned int i=0; i<theSimpleLayers.size(); i++) std::cout << i << " getParamD() " << theSimpleLayers[i].getParamD() 
 //    << " D/C " << theSimpleLayers[i].getParamD()/theSimpleLayers[i].getParamC() <<std::endl;
   
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
  cluster3DTree -> Branch ( "x2", &tree_x2 ) ;
  cluster3DTree -> Branch ( "y2", &tree_y2 ) ;
  cluster3DTree -> Branch ( "z2", &tree_z2 ) ;
  cluster3DTree -> Branch ( "xloc", &tree_xloc ) ;
  cluster3DTree -> Branch ( "yloc", &tree_yloc ) ;
  cluster3DTree -> Branch ( "zloc", &tree_zloc ) ;
  cluster3DTree -> Branch ( "xbary", &tree_xbary ) ;
  cluster3DTree -> Branch ( "ybary", &tree_ybary ) ;
  cluster3DTree -> Branch ( "zbary", &tree_zbary ) ;
  cluster3DTree -> SetCircular ( tree_maxEntries ) ;
  
  //std::cout << "ST3: Branch addresses set" << std::endl;

  
  TrackTree -> Branch ( "tree_trackevent",  &tree_trackevent ) ;
  TrackTree -> Branch ( "tree_trackParam0", &tree_trackParam0 ) ;
  TrackTree -> Branch ( "tree_trackParam1", &tree_trackParam1 ) ;
  TrackTree -> Branch ( "tree_trackParam2", &tree_trackParam2 ) ;
  TrackTree -> Branch ( "tree_trackParam3", &tree_trackParam3 ) ;
  TrackTree -> Branch ( "tree_trackParam4", &tree_trackParam4 ) ;
  TrackTree -> Branch ( "tree_trackParam5", &tree_trackParam5 ) ;
  TrackTree -> Branch ( "tree_chi2", &tree_chi2 ) ;
  TrackTree -> Branch ( "tree_npoints", &tree_npoints ) ;
  
  DQM_NumbOfTracks_per_Event = sub2.make<TH1F>(  "Number of tracks/event", "Count", 30, 0., 30.0  ) ;
  DQM_NumbOfTracks_per_Event -> GetXaxis ( ) -> SetTitle ( "Number of tracks / event " ) ;
  DQM_NumbOfTracks_per_Event -> GetYaxis ( ) -> SetTitle ( "Count" ) ;

  distanceR_per_Track = sub2.make<TH1F>(  "distance-R", "distance-R", 100, 0., 20.0  ) ;
  distanceR_per_Track -> GetXaxis ( ) -> SetTitle ( "distance-R (cm)" ) ;
  distanceR_per_Track -> GetYaxis ( ) -> SetTitle ( "Count" ) ;

  deltaX_per_Track = sub2.make<TH1F>(  "deltaX", "deltaX", 200, -20., 20.0  ) ;
  deltaX_per_Track -> GetXaxis ( ) -> SetTitle ( "deltaX (cm)" ) ;
  deltaX_per_Track -> GetYaxis ( ) -> SetTitle ( "Count" ) ;
  deltaY_per_Track = sub2.make<TH1F>(  "deltaY", "deltaY", 200, -20., 20.0  ) ;
  deltaY_per_Track -> GetXaxis ( ) -> SetTitle ( "deltaY (cm)" ) ;
  deltaY_per_Track -> GetYaxis ( ) -> SetTitle ( "Count" ) ;
  
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

      TH2F* DQM_Correlation2_X_tmp = sub3.make<TH2F>( ( "DQM_Correlation2_X_" + modulename + "_" + modulename0).Data(), ( "X-Correlation between " + modulename + " and " + modulename0 ).Data(), 88, 47.8, 52.2, 88, 47.8, 52.2 ) ;
      TH2F* DQM_Correlation2_Y_tmp = sub3.make<TH2F>( ( "DQM_Correlation2_Y_" + modulename + "_" + modulename0).Data(), ( "Y-Correlation between " + modulename + " and " + modulename0 ).Data(), 60, -3., 3., 60, -3., 3. ) ;
/*
      TH2F* DQM_Correlation3_X_tmp = sub3.make<TH2F>( ( "DQM_Correlation3_X_" + modulename + "_" + modulename0).Data(), ( "dX between " + modulename0 + " and " + modulename + " vs X of " + modulename ).Data(), 860, -2., 2.3, 1000, -0.1, -0.1 ) ;
      TH2F* DQM_Correlation3_Y_tmp = sub3.make<TH2F>( ( "DQM_Correlation3_Y_" + modulename + "_" + modulename0).Data(), ( "dY between " + modulename0 + " and " + modulename + " vs Y of " + modulename   ).Data(), 1120, -2.8, 2.8, 1000, -0.1, 0.1 ) ;
*/

      DQM_Correlation_X_tmp->GetXaxis()->SetTitle("x_" + modulename) ;
      DQM_Correlation_X_tmp->GetYaxis()->SetTitle("x_" + modulename0) ;
      DQM_Correlation_Y_tmp->GetXaxis()->SetTitle("y_" + modulename) ;
      DQM_Correlation_Y_tmp->GetYaxis()->SetTitle("y_" + modulename0) ;

      DQM_Correlation2_X_tmp->GetXaxis()->SetTitle("x_" + modulename) ;
      DQM_Correlation2_X_tmp->GetYaxis()->SetTitle("x_" + modulename0) ;
      DQM_Correlation2_Y_tmp->GetXaxis()->SetTitle("y_" + modulename) ;
      DQM_Correlation2_Y_tmp->GetYaxis()->SetTitle("y_" + modulename0) ;

/*
      DQM_Correlation3_X_tmp->GetXaxis()->SetTitle("x_" + modulename) ;
      DQM_Correlation3_X_tmp->GetYaxis()->SetTitle("x_" + modulename0 + "-x_"+ modulename) ;
      DQM_Correlation3_Y_tmp->GetXaxis()->SetTitle("y_" + modulename) ;
      DQM_Correlation3_Y_tmp->GetYaxis()->SetTitle("y_" + modulename0 + "-y_"+ modulename) ;
*/

      std::pair<uint32_t, uint32_t> modulePair = std::make_pair ( it->first, jt->first ) ;

      DQM_Correlation_X.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation_X_tmp ) ) ;
      DQM_Correlation_Y.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation_Y_tmp ) ) ;

      DQM_Correlation2_X.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation2_X_tmp ) ) ;
      DQM_Correlation2_Y.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation2_Y_tmp ) ) ;
/*
      DQM_Correlation3_X.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation3_X_tmp ) ) ;
      DQM_Correlation3_Y.insert ( std::pair < std::pair<uint32_t, uint32_t>, TH2F*>( modulePair, DQM_Correlation3_Y_tmp ) ) ;
*/

    }//end for j 
    
    //std::cout << "ST4: Correlation plots made" << std::endl;


    // Make the DQM plots
    TH1F* DQM_ClusterCharge_tmp = sub2.make<TH1F>( ( "DQM_ClusterCharge_" + modulename ).Data(), ( "Cluster charge for " + modulename).Data(), 100, 0., 100000. );
    TH1F* DQM_ClusterSize_X_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_X_" + modulename ).Data(), ( "X cluster size for " + modulename).Data(), 30, 0., 30. );
    TH1F* DQM_ClusterSize_Y_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_Y_" + modulename ).Data(), ( "Y cluster size for " + modulename ).Data(), 30, 0., 30. ) ;
    TH1F* DQM_ClusterSize_XY_tmp = sub2.make<TH1F>( ( "DQM_ClusterSize_XY_" + modulename ).Data(), ( "Cluster Size for "  + modulename ).Data(), 30, 0., 30. ) ;
    TH1F* DQM_NumbOfClusters_per_Event_tmp = sub2.make<TH1F>( ("DQM_NumbOfClusters_per_Event_" + modulename).Data(), ("number of clusters for "  + modulename).Data(), 30, 0., 30. );
    TH2F* DQM_ClusterPosition_tmp = sub2.make<TH2F>( ( "DQM_ClusterPosition_" + modulename ).Data(), ( "Cluster occupancy per col per row for " + modulename ).Data(), 416, 0. - 0.5, 416. - 0.5, 160, 0. - 0.5, 160. - 0.5 ); 



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
    
    
//    TH1F* DQM_TrackPull_X_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_X_" + modulename ).Data(), ( "pull track X for " + modulename).Data(), 100, -0.1, 0.1 );
//    TH1F* DQM_TrackPull_Y_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_Y_" + modulename ).Data(), ( "pull track Y for " + modulename).Data(), 100, -0.1, 0.1 );
    TH1F* DQM_TrackPull_X_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_X_" + modulename ).Data(), ( "pull track X for " + modulename).Data(), 500, -0.5, 0.5 );
    TH1F* DQM_TrackPull_Y_tmp = sub2.make<TH1F>( ( "DQM_TrackPull_Y_" + modulename ).Data(), ( "pull track Y for " + modulename).Data(), 500, -0.5, 0.5 );
    TH1F* DQM_TrackPull_X_tmp2 = sub2.make<TH1F>( ( "DQM_TrackPull2_X_" + modulename ).Data(), ( "pull track X for " + modulename).Data(), 500, -0.05, 0.05 );
    TH1F* DQM_TrackPull_Y_tmp2 = sub2.make<TH1F>( ( "DQM_TrackPull2_Y_" + modulename ).Data(), ( "pull track Y for " + modulename).Data(), 500, -0.05, 0.05 );
    TH2F* DQM_Corr_dX_X_tmp = sub2.make<TH2F>( ( "DQM_Corr_dX_X_" + modulename ).Data(), ( "dXloc vs Xloc for " + modulename).Data(), 32, -1.6, 1.6, 500, -0.1, 0.1 ) ;
    TH2F* DQM_Corr_dY_Y_tmp = sub2.make<TH2F>( ( "DQM_Corr_dY_Y_" + modulename ).Data(), ( "dYloc vs Yloc for " + modulename).Data(), 64, -3.2, 3.2, 500, -0.1, 0.1 ) ;
 
    DQM_TrackPull_X[it->first] = DQM_TrackPull_X_tmp;
    DQM_TrackPull_Y[it->first] = DQM_TrackPull_Y_tmp;
    DQM_TrackPull2_X[it->first] = DQM_TrackPull_X_tmp2;
    DQM_TrackPull2_Y[it->first] = DQM_TrackPull_Y_tmp2;
    DQM_Corr_dX_X[it->first] = DQM_Corr_dX_X_tmp;
    DQM_Corr_dY_Y[it->first] = DQM_Corr_dY_Y_tmp;
    
    //std::cout << "ST5: DQM plots made" << std::endl;

    

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
  
  //std::cout << "ST6: end of loop on digis" << std::endl;


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

        // caro 
        const PixelGeomDetUnit *pixdet1 = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(id);
        LocalPoint lp1(-9999., -9999., -9999.);
        PixelClusterParameterEstimator::ReturnType params1 = cpe.getParameters(*iter,*pixdet1);
        lp1 = std::get<0>(params1);
        const Surface& surface1 = tracker->idToDet(id)->surface();
        GlobalPoint gp1 = surface1.toGlobal(lp1);
        double xgp1=0, ygp1=0;
        xgp1 = gp1.x();
        ygp1 = gp1.y();
        //end caro


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

          // caro
          //
          //
          const PixelGeomDetUnit *pixdet2 = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(id2);
          LocalPoint lp2(-9999., -9999., -9999.);
          PixelClusterParameterEstimator::ReturnType params2 = cpe.getParameters(*iter2,*pixdet2);
          lp2 = std::get<0>(params2);
          const Surface& surface2 = tracker->idToDet(id2)->surface();
          GlobalPoint gp2 = surface2.toGlobal(lp2);

          double xgp2=0, ygp2=0;
          xgp2 = gp2.x();
          ygp2 = gp2.y();
          auto itHistMap3 = DQM_Correlation2_X.find(modulePair);
          if ( itHistMap3 == DQM_Correlation2_X.end() ) continue;
          itHistMap3->second->Fill ( xgp1, xgp2 ) ;

          auto itHistMap4 = DQM_Correlation2_Y.find(modulePair);
          if ( itHistMap4 == DQM_Correlation2_Y.end() ) continue;
          itHistMap4->second->Fill ( ygp1, ygp2 ) ;

/*
          auto itHistMap5 = DQM_Correlation3_X.find(modulePair);
          if ( itHistMap5 == DQM_Correlation3_X.end() ) continue;
          if (fabs(xgp2-xgp1)<0.1 && fabs(ygp2-ygp1)<0.1) itHistMap5->second->Fill ( xgp1, xgp2-xgp1 ) ;

          auto itHistMap6 = DQM_Correlation3_Y.find(modulePair);
          if ( itHistMap6 == DQM_Correlation3_Y.end() ) continue;
          if (fabs(xgp2-xgp1)<0.1 && fabs(ygp2-ygp1)<0.1) itHistMap6->second->Fill ( ygp1, ygp2-ygp1 ) ;
*/
        }//end for clusters2
      }//end for cluster
    }//end for first detectors2
  }//end for first detectors
  
  //std::cout << "ST7: After the loop over the clusters" << std::endl;


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
  
  //std::cout << "ST8: After counting the number of clusters for this detector and this event" << std::endl;


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

  //std::cout << "ST9: After the loop over the hits" << std::endl;


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
      tree_xloc=lp.x();
      tree_yloc=lp.y();
      tree_zloc=lp.z();
      tree_xbary= itCluster->x();                   // barycenter x position
      tree_ybary= itCluster->y();                   // barycenter y position
      tree_zbary= 0;

      //Caro
      int ilayernumber=-1;
      double x2=0,y2=0,z2=0;
      for (int il1=0; il1<8; il1++) {
        if (int(detId) == LayersDefinition[il1].first || int(detId) == LayersDefinition[il1].second) ilayernumber=il1;
      }
      if (ilayernumber>-1) {
        TVector3 gptemp(x,y,z); 
        TVector3 lptemp = theSimpleLayers[ilayernumber].getLocalPointPosition(gptemp);
//       std::cout << "check layer " ;
//       std::cout << ilayernumber << " getParamD() " << theSimpleLayers[ilayernumber].getParamD() 
//        << " D/C " << theSimpleLayers[ilayernumber].getParamD()/theSimpleLayers[ilayernumber].getParamC() <<std::endl;
        x2= lptemp.x();
//        std::cout << "x glo " << x << " x loc " << x2 << std::endl;
        y2= lptemp.y();
        z2= lptemp.z();
      } 
      tree_x2 = x2;
      tree_y2 = y2;
      tree_z2 = z2;
      //endcaro



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
  
  //std::cout << "ST10: 3D tree filled" << std::endl;

  
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
//  std::cout << "number of seeds " << theTeleTrackCollection.size() << std::endl;
/*
  for(unsigned int itrack = 0; itrack< theTeleTrackCollection.size(); itrack++){
    std::vector<int > modulesID = theTeleTrackCollection[itrack].getModDetId();
    std::vector<TVector3 > theGP = theTeleTrackCollection[itrack].getGlobalPoints();
    //std::cout << "truc " << std::endl;
    for(unsigned int iHit=0; iHit < theGP.size(); iHit++){
      //std::cout << "det ID " << modulesID[iHit] << "  x " <<  theGP[iHit].X() << "  y " << theGP[iHit].Y()<< "  z " << theGP[iHit].Z()<< std::endl;
    }
  }
*/
  //**************** Strat from the side, get to the next layer **********************
  
  //std::cout << "ST11: Seeding done" << std::endl;

  
  doPatternReco( pixelclusters, tkgeom, cpe,  tracker);
//  std::cout << "number of tracks " << theTeleTrackCollection.size() << std::endl;
  
  DQM_NumbOfTracks_per_Event-> Fill (theTeleTrackCollection.size()) ;
  
  for(unsigned int itrack = 0; itrack< theTeleTrackCollection.size(); itrack++){
    
    
    
    
    std::vector<int > modulesID = theTeleTrackCollection[itrack].getModDetId();
    std::vector<TVector3 > theGP = theTeleTrackCollection[itrack].getGlobalPoints();
    std::vector<TVector3 > theLP = theTeleTrackCollection[itrack].getPseudoLocalPoints();
//    if(theGP.size() < 6) continue; 
    if(theGP.size() < 4) continue;   // temporary check by Caroline
//    if(theTeleTrackCollection[itrack].getChi2() > 3) continue;   // temporary check by Caroline
//    std::cout << "********** new track ******* " << std::endl;
/*
    std::cout << "parameters " << theTeleTrackCollection[itrack].getParameter(0)<< " " <<  
      theTeleTrackCollection[itrack].getParameter(1) << " " << 
      theTeleTrackCollection[itrack].getParameter(2)<< " " <<  
      theTeleTrackCollection[itrack].getParameter(3)<< " " << 
      theTeleTrackCollection[itrack].getParameter(4)<< " " <<  
      theTeleTrackCollection[itrack].getParameter(5)<<  std::endl;
      std::cout << " chi2 " << theTeleTrackCollection[itrack].getChi2() <<  " norm chi2 " << theTeleTrackCollection[itrack].getNormChi2() <<  std::endl;
*/
      
      tree_trackevent = myEvId.event();
      tree_trackParam0=theTeleTrackCollection[itrack].getParameter(0);
      tree_trackParam1=theTeleTrackCollection[itrack].getParameter(1);
      tree_trackParam2=theTeleTrackCollection[itrack].getParameter(2);
      tree_trackParam3=theTeleTrackCollection[itrack].getParameter(3);
      tree_trackParam4=theTeleTrackCollection[itrack].getParameter(4);
      tree_trackParam5=theTeleTrackCollection[itrack].getParameter(5);
      tree_chi2=theTeleTrackCollection[itrack].getChi2();
      tree_npoints=theGP.size();
      TrackTree->Fill();
    for(unsigned int iHit=0; iHit < theGP.size(); iHit++){
//      std::cout << "modulesID " << detId_to_moduleName[modulesID[iHit]] << std::endl;
      double xtemp, ytemp, ztemp;
      double parFit[6] = {theTeleTrackCollection[itrack].getParameter(0), theTeleTrackCollection[itrack].getParameter(1), theTeleTrackCollection[itrack].getParameter(2), theTeleTrackCollection[itrack].getParameter(3), theTeleTrackCollection[itrack].getParameter(4), theTeleTrackCollection[itrack].getParameter(5)};

      //theTeleTrackCollection[itrack].line(theGP[iHit].Z(), parFit, xtemp, ytemp, ztemp); 
      double planeqz[4]; 
      int ilayernumber=-1;
      for (int il1=0; il1<8; il1++) {
//        std::cout << "modulesID[iHit]" << modulesID[iHit] << " LayersDefinition " << LayersDefinition[il1].first << "   "  << LayersDefinition[il1].second << std::endl;
        if (modulesID[iHit] == LayersDefinition[il1].first || modulesID[iHit] == LayersDefinition[il1].second) ilayernumber=il1;
      }
      if (ilayernumber>-1) {
//        std::cout << " GP on layer " << ilayernumber << std::endl;
        planeqz[0]= theSimpleLayers[ilayernumber].getParamA();
        planeqz[1]= theSimpleLayers[ilayernumber].getParamB(); 
        planeqz[2]= theSimpleLayers[ilayernumber].getParamC(); 
        planeqz[3]= theSimpleLayers[ilayernumber].getParamD(); 
        theTeleTrackCollection[itrack].intersection(planeqz, parFit, xtemp, ytemp, ztemp);
        TVector3 gptemp(xtemp, ytemp, ztemp);
        TVector3 lptemp = theSimpleLayers[ilayernumber].getLocalPointPosition(gptemp);
        DQM_TrackPull_X[modulesID[iHit]]->Fill(lptemp.X() - theLP[iHit].X());
        DQM_TrackPull_Y[modulesID[iHit]]->Fill(lptemp.Y() - theLP[iHit].Y()); 
        DQM_TrackPull2_X[modulesID[iHit]]->Fill(lptemp.X() - theLP[iHit].X());
        DQM_TrackPull2_Y[modulesID[iHit]]->Fill(lptemp.Y() - theLP[iHit].Y()); 
        DQM_Corr_dX_X[modulesID[iHit]]->Fill(theLP[iHit].X(),lptemp.X() - theLP[iHit].X());
        DQM_Corr_dY_Y[modulesID[iHit]]->Fill(theLP[iHit].Y(),lptemp.Y() - theLP[iHit].Y());
      }
      else {
//        std::cout << " not found out on which layer is GP " << ilayernumber << std::endl;
        DQM_TrackPull_X[modulesID[iHit]]->Fill(-10 );
        DQM_TrackPull_Y[modulesID[iHit]]->Fill(-10 ); 
        DQM_TrackPull2_X[modulesID[iHit]]->Fill(-10 );
        DQM_TrackPull2_Y[modulesID[iHit]]->Fill(-10 ); 
      }
	  
      //must add number of tracks per event
    }
  }
  
  //std::cout << "ST12: PatternRECO done" << std::endl;

  //std::cout << "ST13: DQM_NumbOfTracks_per_Event filled" << std::endl;
  
  theTeleTrackCollection.clear();

}//end analyze()


// ------------ method called once each job just before starting event loop  ------------
void SimpleTracking::beginJob ( ) {
  //std::cout << "ST14: beginJob" << std::endl;


  // load the noisy channel list
   std::ifstream file_noisy;
   file_noisy.open("/opt/sbg/data/safe1/cms/ccollard/TrackerTelescope/ui6/PixelGeomV2/CMSSW_10_1_11/src/Geometry/PixelTelescope/test/noisy_list.txt");
   inoisy=0;
   if (!file_noisy) { std::cerr << "cannot open file  noisy_list.txt " << std::endl; }
   else
   {
      while (!file_noisy.eof () && inoisy<1000) {
       file_noisy >> noisy_det[inoisy] >> noisy_barx[inoisy] >> noisy_bary[inoisy];
       inoisy++;
      }
   }
    std::cout << " file_noisy : " << inoisy << " entries " << std::endl;
    if (inoisy>0) std::cout << " example detId "<< noisy_det[0] << " x "<< noisy_barx[0] << " y " << noisy_bary[0] <<  std::endl;
   file_noisy.close ();
  //
  //
}//end void beginJob ( ) 

// ------------ method called once each job just after ending the event loop  ------------
void SimpleTracking::endJob ( ) {
  //std::cout << "ST15: endJob" << std::endl;
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

  //std::cout << "ST16: what is the next layer compatible with the track -> checked" << std::endl;
  
}


void SimpleTracking::ApplyAlignment(int detId, double &xaligned, double &yaligned) {

          if (detId==353114116) {
            xaligned-=-2.56917e-02;
            yaligned-=-2.38370e-02;
          }
          else if (detId==353113092) {
            xaligned-=-2.02377e-02;
            yaligned-=-2.70933e-02;
          }
          if (detId==352851972) {
           xaligned-=2.47805e-02;
           yaligned-=-6.97738e-03;
          }
          else if (detId==352850948) {
           xaligned-=1.85113e-02;
           yaligned-=-9.17572e-03;
          }
          else if (detId==352589828) {
           xaligned-=5.50173e-02;
           yaligned-=-3.05621e-02;
          }
          else if (detId==352588804) {
           xaligned-=6.73395e-02;
           yaligned-=-3.06608e-02;
          }
          else if (detId==344200196) {
           xaligned-=-2.77220e-02;
           yaligned-=-9.31173e-02;
          }
          else if (detId==344201220) {
           xaligned-=-2.24462e-02;
           yaligned-=-8.72844e-02;
          }
          else if (detId==344462340) {
           xaligned-=7.14737e-03;
           yaligned-=-1.09290e-01;
          }
          else if (detId==344724484) {
           xaligned-=-7.60673e-02;
           yaligned-=-1.20672e-01;
          }
          else if (detId==344986628) {
           xaligned-=7.71767e-02;
           yaligned-=-1.20759e-01;
          }
          else if (detId==344987652) {
           xaligned-=9.50454e-02;
           yaligned-=-1.33751e-01;
          }


}


//******************************************************
// pair of clusters 
// from the 2 first layes,
// with delta(X) and delta(Y) compatibile within 0.1 cm
//******************************************************

void SimpleTracking::doSeeding(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters,
 const TrackerGeometry *tkgeom,  const PixelClusterParameterEstimator &cpe, edm::ESHandle<TrackerGeometry> tracker){
  
//  std::cout << "-------------" << std::endl;
  for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++   ) {

    edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
      
    auto detid = DetId(DSViter->detId());
//    std::cout << "detID "  << int(detid) << std::endl;



/*
    std::cout << " caro debug position " << std::endl;

       for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=begin; itCluster!=end; ++itCluster ) {
         const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detid);
        LocalPoint lp(-9999., -9999., -9999.);

        PixelClusterParameterEstimator::ReturnType params = cpe.getParameters(*itCluster,*pixdet);
        lp = std::get<0>(params);
 
        double xl=0, yl=0, zl=0;
        xl = lp.x();
        yl = lp.y();
        zl = lp.z();

        const Surface& surface = tracker->idToDet(detid)->surface();
        GlobalPoint gp = surface.toGlobal(lp);

        double x=0, y=0, z=0;
        x = gp.x();
        y = gp.y();
        z = gp.z();
        std::cout << " caro local position    x   "  << xl  << "   y "  << yl  <<  "   z   "  << zl  << std::endl;
        std::cout << " caro global position   x   "  << x  << "   y "  << y  <<  "   z   "  << z  << std::endl;
        std::cout << " caro barycenter position   x   "  << itCluster->x()  << "   y "  << itCluster->y() << std::endl;
      
        TVector3 gp2(x, y, z);
        TVector3 lp2 = theSimpleLayers[0].getLocalPointPosition(gp2);
        double xl2=0, yl2=0, zl2=0;
        xl2 = lp2.x();
        yl2 = lp2.y();
        zl2 = lp2.z();
        std::cout << " caro global position*rot    x   "  << xl2  << "   y "  << yl2  <<  "   z   "  << zl2  << std::endl;

        TVector3 lp3(xl, yl, zl);
        TVector3 gp3 = theSimpleLayers[0].getLocalPointPosition(lp3);
        double x3=0, y3=0, z3=0;
        x3 = gp3.x();
        y3 = gp3.y();
        z3 = gp3.z();
        std::cout << " caro local position*rot    x   "  << x3  << "   y "  << y3  <<  "   z   "  << z3  << std::endl;


       }
    std::cout << " end caro debug position " << std::endl;
*/


    if(int(detid) != 353376260 && int(detid) != 353375236) continue; 

    // if(int(detid) != 353375236) continue; //x between -0.3 and 0.1, y between -1.1 and 0.0, for 352850948 the same
      
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

        // check the noisy clusters
        bool noisyflag=false;
        for (int in=0; in<inoisy; in++) {
         if (int(detid)==noisy_det[in]) {
            double xbary=itCluster->x();
            double ybary=itCluster->y();
            if (int(xbary)==noisy_barx[in] && int(ybary)==noisy_bary[in]) {
                   noisyflag=true;
             }
          }
        }
        if (noisyflag) continue;
        // end check 



/*
	if ((x < -0.3) && (x > 0.1)) continue;
	if ((y < -1.1) && (y > 0.0)) continue;
*/
	
      for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter2=pixelclusters->begin(); DSViter2!=pixelclusters->end();DSViter2++   ) {

        auto detid2 = DetId(DSViter2->detId());
	
	if(int(detid2) == 353376260 || int(detid2) == 353375236) continue; 
	//if(int(detid2) == 353375236) continue;  // layer 0
	
    
        if(int(detid2) !=353114116  && int(detid2) != 353113092) continue;  // layer 1
        // if(int(detid2) != 352850948) continue;  // layer 2
        // need a protection for the seeding to look also for the combination layer 0 + layer 2 or layer 1 + layer 2?
	
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

          // check the noisy clusters
          bool noisyflag2=false;
          for (int in=0; in<inoisy; in++) {
           if (int(detid2)==noisy_det[in]) {
            double xbary2=itCluster2->x();
            double ybary2=itCluster2->y();
            if (int(xbary2)==noisy_barx[in] && int(ybary2)==noisy_bary[in]) {
                   noisyflag2=true;
             }
            }
          }
          if (noisyflag2) continue;
          // end check 

          ApplyAlignment(int(detid2),x2,y2);
/*
//        test Caro alignment
          if (int(detid2)==353114116) {
            x2-=-2.56917e-02;
            y2-=-2.38370e-02;
          }
          else if (int(detid2)==353113092) {
            x2-=-2.02377e-02;
            y2-=-2.70933e-02;
          }
//        end test alignment
*/

	  
	
//	  std::cout << "detid  " << int(detid)  << "  x  " << x << " y " << y << " z " << z << std::endl;
//	  std::cout << "detid2 " << int(detid2) << " x2 " << x2 << " y2 " << y2 << " z2 "<< z2 << std::endl;
	  
	  if(fabs(x-x2) < 0.1 && fabs(y-y2) < 0.1 && fabs(z-z2) < 1000 ){
            //std::pair<SiPixelCluster, SiPixelCluster> thePair;
            //thePair.first =  *(&DSViter) ;
            //thePair.second = *(&DSViter2);
            //thePixelPairSeed.push_back(thePair);
	    
	    
	    TelescopeTracks theseed;
	    TVector3 clust1(x,   y,  z);
	    TVector3 clust2(x2, y2, z2);
//	    TVector3 clust_err(0.5, 0.5, 0.5);
//	    TVector3 clust_err(0.016, 0.016, 0.016);  // to be checked
//	    TVector3 clust_err(0.0043, 0.0029, 0.0029);  // to be checked 150microns/sqrt(12), 100microns/sqrt(12);
	    TVector3 clust_err(0.0029, 0.0043, 0.0029);  // to be checked 150microns/sqrt(12), 100microns/sqrt(12);
	    
	    theseed.addGlobalPoint(clust1); theseed.addGlobalPointErr(clust_err); theseed.addCluster(*itCluster); theseed.addModDetId(int(detid));
	    theseed.addGlobalPoint(clust2); theseed.addGlobalPointErr(clust_err); theseed.addCluster(*itCluster2); theseed.addModDetId(int(detid2));

            TVector3 locclust1 = theSimpleLayers[0].getLocalPointPosition(clust1);
	    theseed.addPseudoLocalPoint(locclust1); theseed.addPseudoLocalPointErr(clust_err); theseed.addAssPlaneA(theSimpleLayers[0].getParamA());
            theseed.addAssPlaneB(theSimpleLayers[0].getParamB()); theseed.addAssPlaneC(theSimpleLayers[0].getParamC());
            theseed.addAssPlaneD(theSimpleLayers[0].getParamD());
            theseed.addAssPlaneX0(theSimpleLayers[0].getOriginX()); theseed.addAssPlaneY0(theSimpleLayers[0].getOriginY()); 
            theseed.addAssPlaneZ0(theSimpleLayers[0].getOriginZ());

            TVector3 locclust2 = theSimpleLayers[1].getLocalPointPosition(clust2);
	    theseed.addPseudoLocalPoint(locclust2); theseed.addPseudoLocalPointErr(clust_err); theseed.addAssPlaneA(theSimpleLayers[1].getParamA());
            theseed.addAssPlaneB(theSimpleLayers[1].getParamB()); theseed.addAssPlaneC(theSimpleLayers[1].getParamC());
            theseed.addAssPlaneD(theSimpleLayers[1].getParamD());
            theseed.addAssPlaneX0(theSimpleLayers[1].getOriginX()); theseed.addAssPlaneY0(theSimpleLayers[1].getOriginY()); 
            theseed.addAssPlaneZ0(theSimpleLayers[1].getOriginZ());

//	    std::cout << "in fit tracks" << std::endl;
	    theseed.fitTrack();
	    
	    theTeleTrackCollection.push_back(theseed);
	    
	    double xtemp, ytemp, ztemp;
	    double parFit[6] = {theseed.getParameter(0), theseed.getParameter(1), theseed.getParameter(2), theseed.getParameter(3), theseed.getParameter(4), theseed.getParameter(5)};
//	    theseed.line(z, parFit, xtemp, ytemp, ztemp);
	    double planeq0[4];
            planeq0[0]= theSimpleLayers[0].getParamA();
            planeq0[1]= theSimpleLayers[0].getParamB();
            planeq0[2]= theSimpleLayers[0].getParamC();
            planeq0[3]= theSimpleLayers[0].getParamD();
	    theseed.intersection(planeq0, parFit, xtemp, ytemp, ztemp);
//	    std::cout << "  check the fit " << xtemp << " " << ytemp << "  " << ztemp << std::endl;
//	    std::cout << "  to be compared with " << x  << " " << y  << "  " << z  << std::endl;
//	    std::cout << "-------------" << std::endl;
//	    theseed.line(z2, parFit, xtemp, ytemp, ztemp);
	    double planeq1[4];
            planeq1[0]= theSimpleLayers[1].getParamA();
            planeq1[1]= theSimpleLayers[1].getParamB();
            planeq1[2]= theSimpleLayers[1].getParamC();
            planeq1[3]= theSimpleLayers[1].getParamD();
	    theseed.intersection(planeq1, parFit, xtemp, ytemp, ztemp);
//	    std::cout << "  check the fit 2 " << xtemp << " " << ytemp << "  " << ztemp << std::endl;
//	    std::cout << "  to be compared with " << x2  << " " << y2  << "  " << z2  << std::endl; // ??
	    
	    
	  }
        }
       }
     }
   }
  
  
  
   //std::cout << "ST17: end: pair of clusters from the 2 first layes, with delta(X) and delta(Y) compatibile within 0.5 cm" << std::endl;
  
}

//******************************************************
// from pair of clusters 
// search for consecutive compatible clusters
// with delta(X) and delta(Y) compatibile within 0.5 cm
//******************************************************

void SimpleTracking::doPatternReco(edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelclusters,
 const TrackerGeometry *tkgeom,  const PixelClusterParameterEstimator &cpe, edm::ESHandle<TrackerGeometry> tracker){

  for(unsigned itrack=0; itrack<theTeleTrackCollection.size(); itrack++){
    double distanceR = 0.;
    double deltaX = 0.;
    double deltaY = 0.;
    
    
    for(int ilayer = 2; ilayer <8; ilayer++){
//    for(int ilayer = 2; ilayer <6; ilayer++){
 
//      if (ilayer==4) continue;   
//      if (ilayer==5) continue;   

      for( edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=pixelclusters->begin(); DSViter!=pixelclusters->end();DSViter++   ) {

      edmNew::DetSet<SiPixelCluster>::const_iterator begin=DSViter->begin();
      edmNew::DetSet<SiPixelCluster>::const_iterator end  =DSViter->end();
        
      auto detid = DetId(DSViter->detId());
 
           
      //if(int(detid) != LayersDefinition[ilayer+1].first && int(detid) != LayersDefinition[ilayer+1].second) continue;
//      if( int(detid) != LayersDefinition[ilayer+1].second) continue;
      if( int(detid) != LayersDefinition[ilayer].first && int(detid) != LayersDefinition[ilayer].second) continue;
       
        TVector3 closestClust;
        TVector3 closestClust2;
	SiPixelCluster closustSiPixelCulster;
	int ncluster = 0;
	int closestR = 1000;
//	TVector3 clust_err(0.5, 0.5, 0.5);
//	TVector3 clust_err(0.016, 0.016, 0.016);  // to be checked
//        TVector3 clust_err(0.0043, 0.0029, 0.0029);  // to be checked 150microns/sqrt(12), 100microns/sqrt(12);
	TVector3 clust_err(0.0029, 0.0043, 0.0029);  // to be checked 150microns/sqrt(12), 100microns/sqrt(12);
        // Then loop on the clusters of the module
        for ( edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=begin; itCluster!=end; ++itCluster ) {

          const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detid);
          LocalPoint lp(-9999., -9999., -9999.);

          PixelClusterParameterEstimator::ReturnType params = cpe.getParameters(*itCluster,*pixdet);
          lp = std::get<0>(params);
  
          const Surface& surface = tracker->idToDet(detid)->surface();
          GlobalPoint gp = surface.toGlobal(lp);

          double x3=0, y3=0, z3=0;
          x3 = gp.x();
          y3 = gp.y();
          z3 = gp.z();

          // check the noisy clusters
          bool noisyflag=false;
          for (int in=0; in<inoisy; in++) {
           if (int(detid)==noisy_det[in]) {
            double xbary=itCluster->x();
            double ybary=itCluster->y();
            if (int(xbary)==noisy_barx[in] && int(ybary)==noisy_bary[in]) {
                   noisyflag=true;
             }
            }
          }
          if (noisyflag) continue;
          // end check 


          ApplyAlignment(int(detid),x3,y3);
/*
//        test Caro alignment
          if (int(detid)==352851972) {
           x3-=2.47805e-02;
           y3-=-6.97738e-03;
          }
          else if (int(detid)==352850948) {
           x3-=1.85113e-02;
           y3-=-9.17572e-03;
          }
          else if (int(detid)==352589828) {
           x3-=5.50173e-02;
           y3-=-3.05621e-02;
          }
          else if (int(detid)==352588804) {
           x3-=6.73395e-02;
           y3-=-3.06608e-02;
          }
          else if (int(detid)==344200196) {
           x3-=-2.77220e-02;
           y3-=-9.31173e-02;
          }
          else if (int(detid)==344201220) {
           x3-=-2.24462e-02;
           y3-=-8.72844e-02;
          }
          else if (int(detid)==344462340) {
           x3-=7.14737e-03;
           y3-=-1.09290e-01;
          }
          else if (int(detid)==344724484) {
           x3-=-7.60673e-02;
           y3-=-1.20672e-01;
          }
          else if (int(detid)==344986628) {
           x3-=7.71767e-02;
           y3-=-1.20759e-01;
          }
          else if (int(detid)==344987652) {
           x3-=9.50454e-02;
           y3-=-1.33751e-01;
          }
*/



//        end test alignment


//	  if ((x3 < -0.3) && (x3 > 0.1)) continue;
//	  if ((y3 < -1.1) && (y3 > 0.0)) continue;

	  TVector3 clust(x3,   y3,  z3);
          TVector3 locclust = theSimpleLayers[ilayer].getLocalPointPosition(clust);



	    
	  double xtemp, ytemp, ztemp;
	  double parFit[6] = {theTeleTrackCollection[itrack].getParameter(0), theTeleTrackCollection[itrack].getParameter(1), theTeleTrackCollection[itrack].getParameter(2), theTeleTrackCollection[itrack].getParameter(3), theTeleTrackCollection[itrack].getParameter(4), theTeleTrackCollection[itrack].getParameter(5)};
//	  theTeleTrackCollection[itrack].line(z3, parFit, xtemp, ytemp, ztemp);
          double planeq[4];
          planeq[0]= theSimpleLayers[ilayer].getParamA();
          planeq[1]= theSimpleLayers[ilayer].getParamB();
          planeq[2]= theSimpleLayers[ilayer].getParamC();
          planeq[3]= theSimpleLayers[ilayer].getParamD();
          theTeleTrackCollection[itrack].intersection(planeq, parFit, xtemp, ytemp, ztemp);
       /// Local Caro
          TVector3 gptemp(xtemp, ytemp, ztemp);
          TVector3 pseudoloc = theSimpleLayers[ilayer].getLocalPointPosition(gptemp);

//        original formula
//	  distanceR = pow( pow(xtemp - x3 , 2)+ pow(ytemp - y3, 2) , 0.5);
//        Caroline: Go to distance in 3D 
//	  distanceR = pow( pow(xtemp - x3 , 2)+ pow(ytemp - y3, 2) + pow(ztemp - z3, 2), 0.5);
//	  Caroline: Go to distance in 2D plane of the telescope
          distanceR = pow( pow(pseudoloc.X() - locclust.X() , 2)+ pow(pseudoloc.Y() - locclust.Y(), 2),  0.5);
	  double deltaXprim = xtemp - x3;  
	  double deltaYprim = ytemp - y3;  
	  deltaX = pseudoloc.X() - locclust.X();
	  deltaY = pseudoloc.X() - locclust.Y();
/*
 	  std::cout << "distanceR = " <<distanceR << std::endl;
	  std::cout << "xtemp = " <<xtemp <<  "  pseudoloc " << pseudoloc.X() << std::endl;
	  std::cout << "x = " <<x3 << "  locclust " << locclust.X()  << std::endl;
	  std::cout << "ytemp = " <<ytemp << "  pseudoloc " << pseudoloc.Y() << std::endl;
	  std::cout << "y = " <<y3 << "  locclust " << locclust.Y() << std::endl;
*/
//	  std::cout << "ztemp = " <<ztemp << std::endl;
//	  std::cout << "z = " <<z3 << std::endl;
//	  if(fabs(deltaX) < 0.1 &&  fabs(deltaY ) < 0.1 && (ncluster == 0 || closestR > distanceR  ) ){
//	  if(ncluster == 0 || closestR > distanceR  ){
//	  if(fabs(deltaXprim) < 1. &&  fabs(deltaYprim ) < 1. && closestR > distanceR){
	  if(fabs(deltaX) < 0.3 &&  fabs(deltaY) < 0.3 && closestR > distanceR){
            // limit the region where to find a close cluster
            closestR=distanceR;
	    closestClust.SetX(x3);
	    closestClust.SetY(y3);
	    closestClust.SetZ(z3);
	    closestClust2 = locclust;
	    closustSiPixelCulster=*itCluster;
	  }
	  ncluster++;
	}
	
        if (closestR<1000) {
          // don't add the point (0,0,0) if no close cluster has been found
  	  theTeleTrackCollection[itrack].addGlobalPoint(closestClust); theTeleTrackCollection[itrack].addGlobalPointErr(clust_err); theTeleTrackCollection[itrack].addCluster(closustSiPixelCulster); theTeleTrackCollection[itrack].addModDetId(int(detid));
	  theTeleTrackCollection[itrack].addPseudoLocalPoint(closestClust2); 
          theTeleTrackCollection[itrack].addPseudoLocalPointErr(clust_err); 
          theTeleTrackCollection[itrack].addAssPlaneA(theSimpleLayers[ilayer].getParamA());
          theTeleTrackCollection[itrack].addAssPlaneB(theSimpleLayers[ilayer].getParamB()); 
          theTeleTrackCollection[itrack].addAssPlaneC(theSimpleLayers[ilayer].getParamC());
          theTeleTrackCollection[itrack].addAssPlaneD(theSimpleLayers[ilayer].getParamD());
          theTeleTrackCollection[itrack].addAssPlaneX0(theSimpleLayers[ilayer].getOriginX()); 
          theTeleTrackCollection[itrack].addAssPlaneY0(theSimpleLayers[ilayer].getOriginY()); 
          theTeleTrackCollection[itrack].addAssPlaneZ0(theSimpleLayers[ilayer].getOriginZ());
  	  theTeleTrackCollection[itrack].fitTrack();
        }
	
      }
    }

    distanceR_per_Track-> Fill (distanceR) ;
    deltaX_per_Track-> Fill (deltaX) ;
    deltaY_per_Track-> Fill (deltaY) ;
  }

  //std::cout << "ST18: end: from pair of clusters search for consecutive compatible clusters with delta(X) and delta(Y) compatibile within 0.5 cm" << std::endl;
 
}



//define this as a plug-in
DEFINE_FWK_MODULE ( SimpleTracking ) ;

