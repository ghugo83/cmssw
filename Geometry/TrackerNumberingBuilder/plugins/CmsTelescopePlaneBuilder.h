#ifndef Geometry_TrackerNumberingBuilder_CmsTelescopePlaneBuilder_H
# define Geometry_TrackerNumberingBuilder_CmsTelescopePlaneBuilder_H

# include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
# include "FWCore/ParameterSet/interface/types.h"
# include <string>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class CmsTelescopePlaneBuilder : public CmsTrackerLevelBuilder
{
public:
  CmsTelescopePlaneBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
