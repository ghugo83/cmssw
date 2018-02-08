#ifndef Geometry_TrackerPhase2TestBeam_TelescopeGeometry_H
#define Geometry_TrackerPhase2TestBeam_TelescopeGeometry_H

#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetEnumerators.h"
#include "Geometry/CommonDetUnit/interface/TrackerGeomDet.h"

class GeometricDet;

/**
 * A specific Telescope Builder which builds a Telescope from a list of DetUnits. 
 * Pattern recognition is used to discover layers, rings etc.
 */
class TelescopeGeometry final : public TrackingGeometry {

  explicit TelescopeGeometry(GeometricDet const* gd=nullptr);  

  friend class TelescopeGeomBuilderFromGeometricDet;

  void addType(GeomDetType const * p);
  void addDetUnit(GeomDet const * p);
  void addDetUnitId(DetId p);
  void addDet(GeomDet const * p);
  void addDetId(DetId p);
  void finalize();

public:
  typedef GeomDetEnumerators::SubDetector SubDetector;

  enum class ModuleType {
    UNKNOWN, 
      PXB, 
      PXF, 
      IB1, 
      IB2, 
      OB1, 
      OB2, 
      W1A, 
      W2A, 
      W3A, 
      W1B, 
      W2B, 
      W3B, 
      W4, 
      W5, 
      W6, 
      W7, 
      Ph1PXB, 
      Ph1PXF, 
      Ph2PXB, 
      Ph2PXF, 
      Ph2PSP, 
      Ph2PSS, 
      Ph2SS
   };

  ~TelescopeGeometry() override ;

  const DetTypeContainer&  detTypes()         const override {return theDetTypes;}
  const DetContainer&      detUnits()         const override {return theDetUnits;}
  const DetContainer&      dets()             const override {return theDets;}
  const DetIdContainer&    detUnitIds()       const override {return theDetUnitIds;}
  const DetIdContainer&    detIds()           const override { return theDetIds;}
  const TrackerGeomDet*    idToDetUnit(DetId) const override;
  const TrackerGeomDet*    idToDet(DetId)     const override;

  const GeomDetEnumerators::SubDetector geomDetSubDetector(int subdet) const;
  unsigned int numberOfLayers(int subdet) const;
  bool isThere(GeomDetEnumerators::SubDetector subdet) const;

  unsigned int offsetDU(SubDetector sid) const { return theOffsetDU[sid];}
  unsigned int endsetDU(SubDetector sid) const { return theEndsetDU[sid];}
  // Magic : better be called at the right moment...
  void setOffsetDU(SubDetector sid) { theOffsetDU[sid]=detUnits().size();}
  void setEndsetDU(SubDetector sid) { theEndsetDU[sid]=detUnits().size();}
  void fillTestMap(const GeometricDet* gd);

  ModuleType moduleType(const std::string& name) const;

  GeometricDet const * trackerDet() const {return  theTrackerDet;}

  const DetContainer& detsPXB() const;
  const DetContainer& detsPXF() const;
  const DetContainer& detsTIB() const;
  const DetContainer& detsTID() const;
  const DetContainer& detsTOB() const;
  const DetContainer& detsTEC() const;

  ModuleType getDetectorType(DetId) const;
  float getDetectorThickness(DetId) const;


private:

  GeometricDet const * theTrackerDet; 

  /// Aligner has access to map
  friend class GeometryAligner;

  DetTypeContainer  theDetTypes;  // owns the DetTypes
  DetContainer      theDetUnits;  // they're all also into 'theDets', so we assume 'theDets' owns them
  unsigned int      theOffsetDU[6]; // offsets in the above
  unsigned int      theEndsetDU[6]; // end offsets in the above
  DetContainer      theDets;      // owns *ONLY* the GeomDet * corresponding to GluedDets.
  DetIdContainer    theDetUnitIds;
  DetIdContainer    theDetIds; 
  mapIdToDetUnit    theMapUnit; // does not own GeomDetUnit *
  mapIdToDet        theMap;     // does not own GeomDet *

  DetContainer      thePXBDets; // not owned: they're also in 'theDets'
  DetContainer      thePXFDets; // not owned: they're also in 'theDets'
  DetContainer      theTIBDets; // not owned: they're also in 'theDets'
  DetContainer      theTIDDets; // not owned: they're also in 'theDets'
  DetContainer      theTOBDets; // not owned: they're also in 'theDets'
  DetContainer      theTECDets; // not owned: they're also in 'theDets'

  GeomDetEnumerators::SubDetector theSubDetTypeMap[3];
  unsigned int theNumberOfLayers[3];
  std::vector< std::tuple< DetId, TelescopeGeometry::ModuleType, float> > theDetTypetList; 
};

#endif
