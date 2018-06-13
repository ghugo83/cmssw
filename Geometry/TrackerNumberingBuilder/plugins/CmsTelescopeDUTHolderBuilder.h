#ifndef Geometry_TrackerNumberingBuilder_CmsTelescopeDUTHolderBuilder_H
# define Geometry_TrackerNumberingBuilder_CmsTelescopeDUTHolderBuilder_H

# include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
# include "FWCore/ParameterSet/interface/types.h"
# include <string>

/**
 * Abstract Class to construct a Level in the hierarchy
 */
class CmsTelescopeDUTHolderBuilder : public CmsTrackerLevelBuilder
{
public:
  CmsTelescopeDUTHolderBuilder();

private:
  void buildComponent( DDFilteredView& , GeometricDet*, std::string ) override;
  void sortNS( DDFilteredView& , GeometricDet* ) override;
};

#endif
