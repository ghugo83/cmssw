/****************************************************************************
*
* Authors:
*	Jan Kašpar (jan.kaspar@gmail.com) 
*	CMSSW developpers (based on class GeometricDet)
*
****************************************************************************/

#ifndef Geometry_VeryForwardGeometryBuilder_DetGeomDesc
#define Geometry_VeryForwardGeometryBuilder_DetGeomDesc

#include <utility>
#include <vector>

#include "DetectorDescription/DDCMS/interface/DDFilteredView.h"

#include "DataFormats/DetId/interface/DetId.h"
#include <Math/Rotation3D.h>

class DDFilteredView;
class PDetGeomDesc;
class CTPPSRPAlignmentCorrectionData;

/**
 * \brief Geometrical description of a sensor.
 *
 * Class resembling GeometricDet class. Slight changes were made to suit needs of the TOTEM RP description.
 * Each instance is a tree node, with geometrical information from DDD (shift, rotation, material, ...), ID and list of children nodes.
 * 
 * The <b>translation</b> and <b>rotation</b> parameters are defined by <b>local-to-global</b>
 * coordinate transform. That is, if r_l is a point in local coordinate system and x_g in global,
 * then the transform reads:
 \verbatim
    x_g = rotation * x_l + translation
 \endverbatim
 *
 * Aug 2020: Migrated to DD4hep
 *
 *  PPS software expects mm but DD4hep standard unit of length is cm. A conversion 
 *  factor (/1._mm) is applied wherever needed. 
 *
 **/

class DetGeomDesc {
public:
  using Container = std::vector<DetGeomDesc*>;
  using RotationMatrix = ROOT::Math::Rotation3D;
  using Translation = ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>>;

  ///Default constructors
  DetGeomDesc() {};

  ///Constructors to be used when looping over DDD
  DetGeomDesc(DDFilteredView* fv);

  ///Constructor from DD4Hep DDFilteredView
  DetGeomDesc(const cms::DDFilteredView& fv, const cms::DDSpecParRegistry& allSpecParSections);

  /// copy constructor and assignment operator
  DetGeomDesc(const DetGeomDesc&);
  DetGeomDesc& operator=(const DetGeomDesc&);

  /// destructor
  virtual ~DetGeomDesc();

  /// ID stuff
  void setGeographicalID(DetId id) { m_geographicalID = id; }
  DetId geographicalID() const { return m_geographicalID; }

  /// access to the tree structure
  Container components() const;
  float parentZPosition() const { return m_z; }

  /// components (children) management
  void addComponent(DetGeomDesc*);
  bool isLeaf() const { return m_container.empty(); }

  /// geometry information
  RotationMatrix rotation() const { return m_rot; }
  Translation translation() const { return m_trans; }
  const std::string& name() const { return m_name; }
  int copyno() const { return m_copy; }
  const std::string& sensorType() const { return m_sensorType; }
  std::vector<double> getDiamondWidth() const;
  
  // Method params() is left for general access to solid shape parameters but should be used 
  // only with great care, for two reasons: 1. order of parameters may possibly change from 
  // a version to another of DD4hep; 2. length parameters unit is cm while PPS uses mm.    
  std::vector<double> params() const { return m_params; }

  /// alignment
  void applyAlignment(const CTPPSRPAlignmentCorrectionData&);

private:
//  DetGeomDesc() {}
  void deleteComponents();      /// deletes just the first daughters
  void deepDeleteComponents();  /// traverses the tree and deletes all nodes.
  void clearComponents() { m_container.resize(0); }

  std::vector<double> copyParameters(const cms::DDFilteredView& fv) const;
  DetId computeDetID(const cms::DDFilteredView& fv) const;
  std::string computeSensorType(const std::string& nodePath, const cms::DDSpecParRegistry& allSpecParSections);

  Container m_container;
  Translation m_trans;
  RotationMatrix m_rot;
  std::string m_name;
  std::vector<double> m_params;
  DetId m_geographicalID;
  int m_copy;
  float m_z;
  std::string m_sensorType;
};

#endif
