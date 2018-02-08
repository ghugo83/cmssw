#ifndef Geometry_TrackerPhase2TestBeam_TelescopeGeometryBuilder_H
# define Geometry_TrackerPhase2TestBeam_TelescopeGeometryBuilder_H

//#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerStringToEnum.h"
//#include "FWCore/ParameterSet/interface/types.h"
//#include <string>
#include <vector>


#include <utility>
//#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
//#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
//#include "Geometry/TrackerNumberingBuilder/plugins/ExtractStringFromDDD.h"
#include "Geometry/TrackerPhase2TestBeam/interface/DetIdBuilder.h"

#include "Geometry/TrackerPhase2TestBeam/interface/DUTHolderOrArmBuilder.h"

class GeometricDet;
class DDCompactView;

/**
 * High level class to build a tracker. It will only build subdets,
 * then call subdet builders
 */

class TelescopeGeometryBuilder
{
public:
  TelescopeGeometryBuilder( void );
  const GeometricDet* construct( const DDCompactView* cpv, std::vector<int> detidShifts);
  
protected:

  std::string attribute;  
  CmsTrackerStringToEnum theCmsTrackerStringToEnum;
};

#endif
