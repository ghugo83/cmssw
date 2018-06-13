#ifndef Geometry_TrackerNumberingBuilder_CmsTelescopeBuilder_H
# define Geometry_TrackerNumberingBuilder_CmsTelescopeBuilder_H

# include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
# include "FWCore/ParameterSet/interface/types.h"
# include <string>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class CmsTelescopeBuilder : public CmsTrackerLevelBuilder
{
public:
  CmsTelescopeBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
