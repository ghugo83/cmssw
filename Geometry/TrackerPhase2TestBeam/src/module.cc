#include "DataFormats/TrackerCommon/interface/TelescopeTopology.h"
#include "Geometry/TrackerPhase2TestBeam/interface/PTelescopeParameters.h"
#include "Geometry/Records/interface/PTelescopeParametersRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/TrackerGeometryBuilder/interface/PlaneBuilderForGluedDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/PlaneBuilderFromGeometricDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/ProxyPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/ProxyStripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackGeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripTopologyBuilder.h"


#include "FWCore/Utilities/interface/typelookup.h"


TYPELOOKUP_DATA_REG(TelescopeTopology);
TYPELOOKUP_DATA_REG(PTelescopeParameters);
TYPELOOKUP_DATA_REG(PTelescopeParametersRcd);
TYPELOOKUP_DATA_REG(PixelGeomDetType);
TYPELOOKUP_DATA_REG(PixelGeomDetUnit);
TYPELOOKUP_DATA_REG(PixelTopologyBuilder);
TYPELOOKUP_DATA_REG(PlaneBuilderForGluedDet);
TYPELOOKUP_DATA_REG(PlaneBuilderFromGeometricDet);
TYPELOOKUP_DATA_REG(ProxyPixelTopology);
TYPELOOKUP_DATA_REG(ProxyStripTopology);
TYPELOOKUP_DATA_REG(RectangularPixelTopology);
TYPELOOKUP_DATA_REG(StackGeomDet);
TYPELOOKUP_DATA_REG(StripGeomDetType);
TYPELOOKUP_DATA_REG(StripGeomDetUnit);
TYPELOOKUP_DATA_REG(StripTopologyBuilder);


