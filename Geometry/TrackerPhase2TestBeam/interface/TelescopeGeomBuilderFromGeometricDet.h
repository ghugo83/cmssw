#ifndef Geometry_TrackerPhase2TestBeam_TelescopeGeomBuilderFromGeometricDet_H
#define Geometry_TrackerPhase2TestBeam_TelescopeGeomBuilderFromGeometricDet_H

#include <string>
#include <vector>
#include <map>
#include "Geometry/TrackerGeometryBuilder/interface/PlaneBuilderFromGeometricDet.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"

#include "DataFormats/DetId/interface/DetId.h"
#include <iostream>

class TelescopeGeometry;
class TelescopeTopology;
class PixelGeomDetType;
class StripGeomDetType;
//class PTelescopeParameters;

class TelescopeGeomBuilderFromGeometricDet {
public:

  //TelescopeGeometry* build(const GeometricDet* gd, const PTelescopeParameters & ptp, const TelescopeTopology* tTopo);
  TelescopeGeometry* build(const GeometricDet* gd, const TelescopeTopology* tTopo);

private:

  void buildPixel(std::vector<const GeometricDet*> const &,
		  TelescopeGeometry*,GeomDetType::SubDetector det,
		  bool upgradeGeometry,
		  int BIG_PIX_PER_ROC_X,
		  int BIG_PIX_PER_ROC_Y);
  void buildSilicon(std::vector<const GeometricDet*> const &,
		    TelescopeGeometry*,GeomDetType::SubDetector det, const std::string& part);
  void buildGeomDet(TelescopeGeometry* );

  PlaneBuilderFromGeometricDet::ResultType
  buildPlaneWithMaterial(const GeometricDet* gd,double scaleFactor=1.) const;

  std::map<std::string,const PixelGeomDetType*> thePixelDetTypeMap;
  std::map<std::string,const StripGeomDetType*> theStripDetTypeMap;
  const TelescopeTopology* theTopo;
};

#endif
