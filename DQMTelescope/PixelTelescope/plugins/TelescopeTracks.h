
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"

#include <iostream>
#include <vector>
#include "TVector3.h"


#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"


class TelescopeTracks {
    
    double p0_, p1_, p2_, p3_;
    
    double xOnPlane_, yOnPlane_, zOnPlane_; 
    
    std::vector<SiPixelCluster> clusterList_;
    
    std::vector<TVector3> globalPoints;
    std::vector<TVector3> globalPoints_err;
    std::vector<int>      modDetId_;
    double normChi2_;
    double chi2_;
    
  public:
    //Default Constructor 
    TelescopeTracks() 
    { 
        p0_ = -1000;
        p1_ = -1000;
        p2_ = -1000;
        p3_ = -1000;
	xOnPlane_= -1000;
	yOnPlane_= -1000;
	zOnPlane_= -1000;
	 
    } 
    TelescopeTracks(double p0, double p1, double p2, double p3 ) 
    { 
        p0_ = p0;
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
	xOnPlane_= -1000;
	yOnPlane_= -1000;
	zOnPlane_= -1000;
	 
    } 
    void setTrackParam(int, double);
    
    double getParameter(int );
    void propagateToPlane(std::vector<double> );
    
    double getXonPlane(){return xOnPlane_; };
    double getYonPlane(){return yOnPlane_; };
    double getZonPlane(){return zOnPlane_; };
    
    void addCluster(SiPixelCluster );
    void cleanClusterList();
    std::vector<SiPixelCluster> getclusterList();
    
    void addGlobalPoint(TVector3 );
    void cleanGlobalPoints();
    std::vector<TVector3> getGlobalPoints();
    
    void addGlobalPointErr(TVector3 );
    void cleanGlobalPointsErr();
    std::vector<TVector3> getGlobalPointsErr();
    
    
    void addModDetId(int thedetid){modDetId_.push_back(thedetid);};
    void cleanModDetId(){modDetId_.clear();};
    std::vector<int>  getModDetId(){return modDetId_;};
    
    double getNormChi2(){return normChi2_;};
    double getChi2(){return chi2_;};
    
    void line(double , double *, double &, double &, double &);
    
    void fitTrack();
    
    
} ;
