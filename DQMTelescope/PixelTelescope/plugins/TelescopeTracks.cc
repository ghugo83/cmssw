#include "DQMTelescope/PixelTelescope/plugins/TelescopeTracks.h"
#include "TGraph2DErrors.h"
#include "TVirtualFitter.h"
#include "Math/Vector3D.h"
#include "TMath.h"



///////////////////////////////////////////////////////
/////////// external function definition //////////////
/////////// needed for the fit           //////////////
///////////////////////////////////////////////////////


using namespace ROOT::Math;

// calculate distance line-point 
double distance2(double x,double y,double z, double *p) { 
   // distance line point is D= | (xp-x0) cross  ux | 
   // where ux is direction of line and x0 is a point in the line (like t = 0) 
   
   XYZVector xp(x,y,z); 
   XYZVector x0(p[0], p[2], 0. ); 
   XYZVector x1(p[0] + p[1], p[2] + p[3], 1. ); 
   XYZVector u = (x1-x0).Unit(); 
   double d2 = ((xp-x0).Cross(u)) .Mag2();
   
   /*XYZVector x0(x,y,z); // points from where to measure the distance
   XYZVector x1(p[0],p[2],p[4]); // one point on the direction
   XYZVector x2(p[1],p[3],p[5] ); // one point on the direction
   
   double d = 
   	(((x0-x1).Cross(x0-x2)).Mag2())    /
	(x2-x1).Mag2();*/
   
   //std::cout << "d2 ******* " << d2 << std::endl;
   return d2; 
}
	
// function to be minimized 
void SumDistance2(int &, double *, double & sum, double * par, int ) { 
  // the TGraph must be a global variable
     TGraph2D * gr = dynamic_cast<TGraph2D*>( (TVirtualFitter::GetFitter())->GetObjectFit() );
     assert(gr != 0);
     double * x = gr->GetX();
     double * y = gr->GetY();
     double * z = gr->GetZ();
     int npoints = gr->GetN();
     sum = 0;
     for (int i  = 0; i < npoints; ++i) { 
     	double d = distance2(x[i],y[i],z[i],par); 
	sum += d;
	//if (first) std::cout << "point " << i << "\t" << x[i] << "\t"  << y[i] << "\t" << z[i] << "\t"  << std::sqrt(d) << std::endl; 
     }
     sum /=npoints;
     //first = false;
     //std::cout << "sum " << sum << std::endl;
}




///////////////////////////////////////////////////////////
/////////// def of members of Telescopetrack //////////////
///////////////////////////////////////////////////////////




void TelescopeTracks::setTrackParam(int ipara, double value){

  if(ipara == 0)  p0_ = value;
  else if(ipara == 1)  p1_= value;
  else if(ipara == 2)  p2_= value;
  else if(ipara == 3)  p3_= value;
  

}


void TelescopeTracks::addCluster(SiPixelCluster theCluster){ clusterList_.push_back(theCluster);}


void TelescopeTracks::cleanClusterList(){ clusterList_.clear(); }


std::vector<SiPixelCluster> TelescopeTracks::getclusterList(){ return clusterList_;}


void     	      TelescopeTracks::addGlobalPoint(TVector3 gp ){globalPoints.push_back(gp); }
void 		      TelescopeTracks::cleanGlobalPoints(){ globalPoints.clear(); }
std::vector<TVector3> TelescopeTracks::getGlobalPoints(){ return globalPoints;}
    
    
void        	      TelescopeTracks::addGlobalPointErr(TVector3 gp ){globalPoints_err.push_back(gp); }
void                  TelescopeTracks::cleanGlobalPointsErr(){ globalPoints_err.clear(); }
std::vector<TVector3> TelescopeTracks::getGlobalPointsErr(){ return globalPoints_err;}
    
    
    
double TelescopeTracks::getParameter(int ipara){

  if(ipara == 0) return p0_;
  else if(ipara == 1) return p1_;
  else if(ipara == 2) return p2_;
  else if(ipara == 3) return p3_;
  else return -10000;
  
}

void TelescopeTracks::propagateToPlane(std::vector<double> planeParam) {
  
  if(planeParam.size() !=4) std::cout << "not the right number of parameters for the plane, should be 4 " << std::endl; 
  
  double a = planeParam[0];
  double b = planeParam[1];
  double c = planeParam[2];
  double d = planeParam[3];
  double t = (-a*p0_ - b*p2_-d)/(p1_*a + b*p3_+c);
  
  xOnPlane_ = p0_+t*p1_;
  yOnPlane_ = p2_+t*p3_;
  zOnPlane_ = t;
  
   
}


void TelescopeTracks::line(double t, double *p, double &x, double &y, double &z) { 
  // a parameteric line is define from 6 parameters but 4 are independent
  // x0,y0,z0,z1,y1,z1 which are the coordinates of two points on the line
  // can choose z0 = 0 if line not parallel to x-y plane and z1 = 1; 
  x = p[0] + p[1]*t;
  y = p[2] + p[3]*t;
  z = t;  
}


void TelescopeTracks::fitTrack(){
  
  std::vector<double> track;
  
  TGraph2DErrors * gr = new TGraph2DErrors();
  double p0[4] = {10,20,1,2};
  
  const int nCluster = globalPoints.size();
  
  
  for(int icls=0; icls<nCluster; icls++){
    gr->SetPoint(icls, globalPoints[icls].X(),     globalPoints[icls].Y(),     globalPoints[icls].Z()      );
    gr->SetPointError(icls, globalPoints_err[icls].X(), globalPoints_err[icls].Y(), globalPoints_err[icls].Z()  );
  }
  
  
  TVirtualFitter *min = TVirtualFitter::Fitter(0,4);
  
  min->SetObjectFit(gr);
  min->SetFCN(SumDistance2);
  
  Double_t arglist[10];
  arglist[0] = 3;
  min->ExecuteCommand("SET PRINT",arglist,-1);
  
  double pStart[4] = {1,1,1,1};
  min->SetParameter(0,"p0",pStart[0],0.01,0,0);
  min->SetParameter(1,"p1",pStart[1],0.01,0,0);
  min->SetParameter(2,"p2",pStart[2],0.01,0,0);
  min->SetParameter(3,"p3",pStart[3],0.01,0,0);
  arglist[0] = 10000; // number of function calls 
  arglist[1] = 0.001; // tolerance 
  min->ExecuteCommand("MIGRAD",arglist,2);
  
  int nvpar,nparx; 
  double amin,edm, errdef;
  min->GetStats(amin,edm,errdef,nvpar,nparx);
  int ndf = nCluster-nvpar;
  
  normChi2_ =amin/ndf;
  chi2_=amin;
  //min->PrintResults(1,amin);
  
  // get fit parameters
  //double parFit[4];
  //for (int i = 0; i <4; ++i) parFit[i] = min->GetParameter(i);
 
  p0_ = min->GetParameter(0);
  p1_ = min->GetParameter(1);
  p2_ = min->GetParameter(2);
  p3_ = min->GetParameter(3);
  
}



