#ifndef Geometry_TrackerNumberingBuilder_CmsTelescopePhase1PixelModuleBuilder_H
# define Geometry_TrackerNumberingBuilder_CmsTelescopePhase1PixelModuleBuilder_H

# include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
# include "FWCore/ParameterSet/interface/types.h"
# include <string>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class CmsTelescopePhase1PixelModuleBuilder : public CmsTrackerLevelBuilder
{
public:
  CmsTelescopePhase1PixelModuleBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
