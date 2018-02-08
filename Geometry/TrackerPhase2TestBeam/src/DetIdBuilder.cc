#include "Geometry/TrackerPhase2TestBeam/interface/DetIdBuilder.h"

DetIdBuilder::DetIdBuilder( std::vector<int> detidShifts )
  : detIdShifts_(detidShifts), numHierarchyLevels_(detIdShifts_.size())
{}


void DetIdBuilder::build(GeometricDet* telescope) {
  LogDebug("BuildingTelescopeDetIds") << "Starting to build Telescope DetIds";

  int hierarchyLevel = 0;
  uint32_t siblingCounter = 0;
  uint32_t parentId = 0;

  iterate(parentId, telescope, siblingCounter, hierarchyLevel);
}


void DetIdBuilder::iterate(uint32_t parentId, GeometricDet* volume, int siblingCounter, int hierarchyLevel) {

  if (hierarchyLevel >= numHierarchyLevels_) {
    edm::LogError( "TelescopeDetIdBuilder" ) << " ERROR - I reached hierarchyLevel " << hierarchyLevel << ". Level should be strictly lower than " << numHierarchyLevels_;
  }

  else {

    uint32_t indicator = 0;
    if (hierarchyLevel == 0) { indicator = 8; }  // At telescope volume level: DetId::Telescope = 8 (see DataFormats/DetId/interface/DetId.h ).
    else if (hierarchyLevel == (numHierarchyLevels_ - 1)) { indicator = volume->geographicalID().rawId(); }
    else { indicator = siblingCounter + 1; }

    uint32_t id = parentId | ( indicator << detIdShifts_.at(hierarchyLevel));
    volume->setGeographicalID(DetId(id));



    std::cout << "GeometricDet = " << theCmsTrackerStringToEnum_.name(volume->type())
	      << ", DetId = " << volume->geographicalID().rawId() 
	      << ", x = " << volume->translation().X() 
	      << ", y = " << volume->translation().Y()
	      << ", z = " << volume->translation().Z()
	      << ", phi = "  << volume->phi() * 180. / M_PI << std::endl;



    hierarchyLevel++;
    parentId = id;
    uint32_t numSiblings = volume->components().size();

    for (uint32_t siblingCounter = 0; siblingCounter < numSiblings; siblingCounter++) {
      GeometricDet* child = volume->component(siblingCounter);
      iterate(parentId, child, siblingCounter, hierarchyLevel);
    }
  }

}
