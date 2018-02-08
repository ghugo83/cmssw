#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeomBuilderFromGeometricDet.h"
#include "Geometry/TrackerPhase2TestBeam/interface/TelescopeGeometry.h"

#include "Geometry/TrackerGeometryBuilder/interface/PlaneBuilderForGluedDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonDetUnit/interface/GluedGeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackGeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripTopologyBuilder.h"
//#include "CondFormats/GeometryObjects/interface/PTelescopeParameters.h"
//#include "DataFormats/TrackerCommon/interface/PTelescopeParameters.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TelescopeTopology.h"
#include "DataFormats/GeometrySurface/interface/MediumProperties.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <cfloat>
#include <cassert>
using std::vector;
using std::string;

/*namespace {
  void verifyDUinTG(TelescopeGeometry const & tg) {
    int off=0; int end=0;
    for ( int i=1; i!=7; i++) {
      auto det = GeomDetEnumerators::tkDetEnum[i];
      off = tg.offsetDU(det);
      end = tg.endsetDU(det); assert(end>=off); // allow empty subdetectors. Needed for upgrade
      for (int j=off; j!=end; ++j) {
	assert(tg.detUnits()[j]->geographicalId().subdetId()==i);
	assert(GeomDetEnumerators::subDetGeom[tg.detUnits()[j]->subDetector()]==det);
	assert(tg.detUnits()[j]->index()==j);
      }
    }
  }
  }*/

//TelescopeGeometry* TelescopeGeomBuilderFromGeometricDet::build( const GeometricDet* gd, const PTelescopeParameters& ptp, const TelescopeTopology* tTopo ) {
TelescopeGeometry* TelescopeGeomBuilderFromGeometricDet::build( const GeometricDet* gd, const TelescopeTopology* tTopo) {
  //int BIG_PIX_PER_ROC_X = ptp.vpars[2];
  //int BIG_PIX_PER_ROC_Y = ptp.vpars[3];
    
  thePixelDetTypeMap.clear();
  theStripDetTypeMap.clear();
   
  TelescopeGeometry* telescope = new TelescopeGeometry(gd);
  std::vector<const GeometricDet*> comp;
  gd->deepComponents(comp);
 
  if(tTopo)  theTopo = tTopo;

  //define a vector which associate to the detid subdetector index -1 (from 0 to 5) the GeometridDet enumerator to be able to know which type of subdetector it is
  

  /* ALL THIS IS TO SORT THE GEOMETRICDET BY SUBDETECTOR !!
  std::vector<GeometricDet::GDEnumType> gdsubdetmap(6,GeometricDet::unknown); // hardcoded "6" should not be a surprise... 
  GeometricDet::ConstGeometricDetContainer subdetgd = gd->components();
  
  LogDebug("SubDetectorGeometricDetType") << "GeometriDet enumerator values of the subdetectors";
  for(unsigned int i=0;i<subdetgd.size();++i) {
    assert(subdetgd[i]->geographicalId().subdetId()>0 && subdetgd[i]->geographicalId().subdetId()<7);
    gdsubdetmap[subdetgd[i]->geographicalId().subdetId()-1]= subdetgd[i]->type();
    LogTrace("SubDetectorGeometricDetType") << "subdet " << i 
					    << " type " << subdetgd[i]->type()
					    << " detid " <<  subdetgd[i]->geographicalId().rawId()
					    << " subdetid " <<  subdetgd[i]->geographicalId().subdetId();
					    }
*/
  
  std::vector<const GeometricDet*> dets[3];
  std::vector<const GeometricDet*> & armNegSideDets = dets[0]; armNegSideDets.reserve(comp.size());
  std::vector<const GeometricDet*> & DUTDets = dets[1]; DUTDets.reserve(comp.size());
  std::vector<const GeometricDet*> & armPosSideDets  = dets[2];  armPosSideDets.reserve(comp.size());
  



  
  for(auto & i : comp) {
    dets[i->geographicalID().subdetId()-1].emplace_back(i);
  }
  
 

  //loop on all the six elements of dets and firstly check if they are from pixel-like detector and call buildPixel, then loop again and check if they are strip and call buildSilicon. "unknown" can be filled either way but the vector of GeometricDet must be empty !!
  // this order is VERY IMPORTANT!!!!! For the moment I (AndreaV) understand that some pieces of code rely on pixel-like being before strip-like 
  
  // now building the Pixel-like subdetectors
  /*
  for(unsigned int i=0;i<6;++i) {
    if(gdsubdetmap[i] == GeometricDet::PixelBarrel) 
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::PixelBarrel,
		 false,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
    if(gdsubdetmap[i] == GeometricDet::PixelPhase1Barrel) 
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P1PXB,
		 false,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y);
    // Phase2 case
        if(gdsubdetmap[i] == GeometricDet::PixelPhase2Barrel) 
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P2PXB,
		 true,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y);
	//
    if(gdsubdetmap[i] == GeometricDet::PixelEndCap)
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::PixelEndcap,
		 false,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
    if(gdsubdetmap[i] == GeometricDet::PixelPhase1EndCap)
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P1PXEC,
		 false,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
    if(gdsubdetmap[i] == GeometricDet::PixelPhase2EndCap)
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P2PXEC,
		 true,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
    if(gdsubdetmap[i] == GeometricDet::OTPhase2Barrel) 
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P2OTB,
		 true,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
    if(gdsubdetmap[i] == GeometricDet::OTPhase2EndCap) 
      buildPixel(dets[i],telescope,GeomDetEnumerators::SubDetector::P2OTEC,
		 true,
		 BIG_PIX_PER_ROC_X,
		 BIG_PIX_PER_ROC_Y); 
  }
  //now building Strips
  for(unsigned int i=0;i<6;++i) {
    if(gdsubdetmap[i] == GeometricDet::TIB)   buildSilicon(dets[i],telescope,GeomDetEnumerators::SubDetector::TIB, "barrel");
    if(gdsubdetmap[i] == GeometricDet::TID)   buildSilicon(dets[i],telescope,GeomDetEnumerators::SubDetector::TID, "endcap");
    if(gdsubdetmap[i] == GeometricDet::TOB)   buildSilicon(dets[i],telescope,GeomDetEnumerators::SubDetector::TOB, "barrel");
    if(gdsubdetmap[i] == GeometricDet::TEC)   buildSilicon(dets[i],telescope,GeomDetEnumerators::SubDetector::TEC, "endcap");
  }  
  // and finally the "empty" subdetectors (maybe it is not needed)
  for(unsigned int i=0;i<6;++i) {
    if(gdsubdetmap[i] == GeometricDet::unknown) {
      if(!dets[i].empty()) throw cms::Exception("NotEmptyUnknownSubDet") << "Subdetector " << i+1 << " is unknown but it is not empty: " << dets[i].size();
      buildSilicon(dets[i],telescope,GeomDetEnumerators::tkDetEnum[i+1], "barrel"); // "barrel" is used but it is irrelevant
    }
  }
  */


  int BIG_PIX_PER_ROC_X_Phase1 = 1; // To DO: add in telescope params
  int BIG_PIX_PER_ROC_Y_Phase1 = 2;

  // Telescope arm, (-Z) side
  buildPixel(armNegSideDets,telescope,GeomDetEnumerators::SubDetector::P1PXEC,
	     false,
	     BIG_PIX_PER_ROC_X_Phase1,
	     BIG_PIX_PER_ROC_Y_Phase1);

  // Telescope arm, (+Z) side
  buildPixel(armPosSideDets,telescope,GeomDetEnumerators::SubDetector::P1PXEC,
	     false,
	     BIG_PIX_PER_ROC_X_Phase1,
	     BIG_PIX_PER_ROC_Y_Phase1);


  int BIG_PIX_PER_ROC_X_Phase2 = 0; // To DO: add in telescope params
  int BIG_PIX_PER_ROC_Y_Phase2 = 0;

  // Telescope DUT containers (contains only 1 DUT here) 
  buildPixel(DUTDets,telescope,GeomDetEnumerators::SubDetector::P2OTB,
	     true,
	     BIG_PIX_PER_ROC_X_Phase2,
	     BIG_PIX_PER_ROC_Y_Phase2); 

 
  




  buildGeomDet(telescope);//"GeomDet"

  //verifyDUinTG(*telescope);

  return telescope;
}

void TelescopeGeomBuilderFromGeometricDet::buildPixel(std::vector<const GeometricDet*>  const & gdv, 
						    TelescopeGeometry* telescope,
						    GeomDetType::SubDetector det,
						    bool upgradeGeometry,
						    int BIG_PIX_PER_ROC_X, // in x direction, rows. BIG_PIX_PER_ROC_X = 0 for SLHC
						    int BIG_PIX_PER_ROC_Y) // in y direction, cols. BIG_PIX_PER_ROC_Y = 0 for SLHC
{
  LogDebug("BuildingGeomDetUnits") << " Pixel type. Size of vector: " << gdv.size() 
				   << " GeomDetType subdetector: " << det 
				   << " logical subdetector: " << GeomDetEnumerators::subDetGeom[det]
				   << " big pix per ROC x: " << BIG_PIX_PER_ROC_X << " y: " << BIG_PIX_PER_ROC_Y
				   << " is upgrade: " << upgradeGeometry;
  
  telescope->setOffsetDU(GeomDetEnumerators::subDetGeom[det]);

  for(auto i : gdv){

    std::string const & detName = i->name().fullname();
    if (thePixelDetTypeMap.find(detName) == thePixelDetTypeMap.end()) {
      std::unique_ptr<const Bounds> bounds(i->bounds());
      
      PixelTopology* t = 
	  PixelTopologyBuilder().build(&*bounds,
				       upgradeGeometry,
				       i->pixROCRows(),
				       i->pixROCCols(),
				       BIG_PIX_PER_ROC_X,
				       BIG_PIX_PER_ROC_Y,
				       i->pixROCx(), i->pixROCy());
      
      thePixelDetTypeMap[detName] = new PixelGeomDetType(t,detName,det);
      telescope->addType(thePixelDetTypeMap[detName]);
    }

    PlaneBuilderFromGeometricDet::ResultType plane = buildPlaneWithMaterial(i);
    GeomDetUnit* temp =  new PixelGeomDetUnit(&(*plane),thePixelDetTypeMap[detName],i->geographicalID());

    telescope->addDetUnit(temp);
    telescope->addDetUnitId(i->geographicalID());
  }
  telescope->setEndsetDU(GeomDetEnumerators::subDetGeom[det]);
}

void TelescopeGeomBuilderFromGeometricDet::buildSilicon(std::vector<const GeometricDet*>  const & gdv, 
						      TelescopeGeometry* telescope,
						      GeomDetType::SubDetector det,
						      const std::string& part)
{ 
  LogDebug("BuildingGeomDetUnits") << " Strip type. Size of vector: " << gdv.size() 
				   << " GeomDetType subdetector: " << det 
				   << " logical subdetector: " << GeomDetEnumerators::subDetGeom[det]
				   << " part " << part;
  
  telescope->setOffsetDU(GeomDetEnumerators::subDetGeom[det]);

  for(auto i : gdv){

    std::string const & detName = i->name().fullname();
    if (theStripDetTypeMap.find(detName) == theStripDetTypeMap.end()) {
       std::unique_ptr<const Bounds> bounds(i->bounds());
       StripTopology* t =
	   StripTopologyBuilder().build(&*bounds,
				       i->siliconAPVNum(),
				       part);
      theStripDetTypeMap[detName] = new  StripGeomDetType( t,detName,det,
						   i->stereo());
      telescope->addType(theStripDetTypeMap[detName]);
    }
     
    double scale  = (theTopo->partnerDetId(i->geographicalID())) ? 0.5 : 1.0 ;

    PlaneBuilderFromGeometricDet::ResultType plane = buildPlaneWithMaterial(i,scale);  
    GeomDetUnit* temp = new StripGeomDetUnit(&(*plane), theStripDetTypeMap[detName],i->geographicalID());
    
    telescope->addDetUnit(temp);
    telescope->addDetUnitId(i->geographicalID());
  }  
  telescope->setEndsetDU(GeomDetEnumerators::subDetGeom[det]);

}


void TelescopeGeomBuilderFromGeometricDet::buildGeomDet(TelescopeGeometry* telescope){

  PlaneBuilderForGluedDet gluedplaneBuilder;
  auto const & gdu = telescope->detUnits();
  auto const & gduId = telescope->detUnitIds();

  for(u_int32_t i=0;i<gdu.size();i++){

    telescope->addDet(gdu[i]);
    telescope->addDetId(gduId[i]);
    string gduTypeName = gdu[i]->type().name();

    //this step is time consuming >> TO FIX with a MAP?
    if( (gduTypeName.find("Ster")!=std::string::npos || 
         gduTypeName.find("Lower")!=std::string::npos) && 
        (theTopo->glued(gduId[i])!=0 || theTopo->stack(gduId[i])!=0 )) {
    

      std::cout << "TelescopeGeomBuilderFromGeometricDet::buildGeomDet  gduId[i].rawId() = " << gduId[i].rawId() << std::endl;
      std::cout << "TelescopeGeomBuilderFromGeometricDet::buildGeomDet  theTopo->partnerDetId(gduId[i]).rawId() = " << theTopo->partnerDetId(gduId[i]).rawId() << std::endl;


      int partner_pos=-1;
      for(u_int32_t jj=0;jj<gduId.size();jj++){
  	  if(theTopo->partnerDetId(gduId[i]) == gduId[jj]) {
  	    partner_pos=jj;
  	    break;
  	  }
      }
      if(partner_pos==-1){
	  throw cms::Exception("Configuration") <<"Module Type is Stereo or Lower but no partner detector found \n"
					        <<"There is a problem on Telescope geometry configuration\n";
      }

      const GeomDetUnit* dus = gdu[i];
      const GeomDetUnit* dum = gdu[partner_pos];
      std::vector<const GeomDetUnit *> composed(2);
      composed[0]=dum;
      composed[1]=dus;
      DetId composedDetId;
      if(gduTypeName.find("Ster")!=std::string::npos){

        PlaneBuilderForGluedDet::ResultType plane = gluedplaneBuilder.plane(composed);
        composedDetId = theTopo->glued(gduId[i]);
        GluedGeomDet* gluedDet = new GluedGeomDet(&(*plane),dum,dus,composedDetId);
        telescope->addDet((GeomDet*) gluedDet);
        telescope->addDetId(composedDetId);

      } else if (gduTypeName.find("Lower")!=std::string::npos){

        //FIXME::ERICA: the plane builder is built in the middle...
        PlaneBuilderForGluedDet::ResultType plane = gluedplaneBuilder.plane(composed);
        composedDetId = theTopo->stack(gduId[i]);
        StackGeomDet* stackDet = new StackGeomDet(&(*plane),dum,dus,composedDetId);
	std::cout << "TelescopeGeomBuilderFromGeometricDet::buildGeomDet  composedDetId.rawId() = " << composedDetId.rawId() << std::endl;
        telescope->addDet((GeomDet*) stackDet);
        telescope->addDetId(composedDetId);

      } 

    }

  }
}



PlaneBuilderFromGeometricDet::ResultType
TelescopeGeomBuilderFromGeometricDet::buildPlaneWithMaterial(const GeometricDet* gd,
							   double scale) const
{
  PlaneBuilderFromGeometricDet planeBuilder;
  PlaneBuilderFromGeometricDet::ResultType plane = planeBuilder.plane(gd);  
  //
  // set medium properties (if defined)
  //
  plane->setMediumProperties(MediumProperties(gd->radLength()*scale,gd->xi()*scale));

  return plane;
}
