/****************************************************************************
*
* This is a part of the TOTEM offline software.
* Authors: 
*	Jan Kašpar (jan.kaspar@gmail.com) 
*	CMSSW developers (based on GeometricDet class)
*
****************************************************************************/

#include <utility>

#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSDDDNames.h"
#include "CondFormats/GeometryObjects/interface/PDetGeomDesc.h"
#include "CondFormats/PPSObjects/interface/CTPPSRPAlignmentCorrectionData.h"

#include "DataFormats/Math/interface/CMSUnits.h"
#include "DetectorDescription/DDCMS/interface/DDFilteredView.h"
#include "DetectorDescription/DDCMS/interface/DDShapes.h"
#include "DetectorDescription/DDCMS/interface/DDSolidShapes.h"
#include "TGeoMatrix.h"

#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"


using namespace std;
using namespace cms_units::operators;


// Constructor from DD4Hep DDFilteredView, also using the SpecPars to access 2x2 wafers info.
//  A conversion factor (/1._mm) is applied wherever needed.
DetGeomDesc::DetGeomDesc(const cms::DDFilteredView& fv, const cms::DDSpecParRegistry& allSpecParSections)
  : m_name(computeNameWithNoNamespace(fv.name())),
    m_mat(fv.materialName()),
    m_copy(fv.copyNum()),
    m_trans(fv.translation() / 1._mm),  // Convert cm (DD4hep) to mm (legacy)
    m_rot(fv.rotation()),
    m_params(computeParameters(fv)),
    m_allparams(computeParametersTEST(fv)),
    m_isABox(fv.isABox()),
    m_sensorType(computeSensorType(fv.name(), fv.path(), allSpecParSections)),
    m_geographicalID(computeDetID(m_name, fv.copyNos(), fv.copyNum())),
  m_z(fv.translation().z() / 1._mm)  // Convert cm (DD4hep) to mm (legacy)
{}


std::vector<double> DetGeomDesc::computeParametersTEST(const cms::DDFilteredView& fv) const {

  std::vector<double> result;

  const cms::DDSolidShape& mySolidShape = cms::dd::getCurrentShape(fv);

  if (mySolidShape == cms::DDSolidShape::ddbox) {
    const cms::dd::DDBox& myShape = cms::dd::DDBox(fv);
    result = { myShape.halfX() / 1._mm,
		   myShape.halfY() / 1._mm,
		   myShape.halfZ() / 1._mm
    };
  }
  else if (mySolidShape == cms::DDSolidShape::ddcons) {
    const cms::dd::DDCons& myShape = cms::dd::DDCons(fv);
    result = { myShape.zhalf() / 1._mm,
		   myShape.rInMinusZ() / 1._mm,
		   myShape.rOutMinusZ() / 1._mm,
		   myShape.rInPlusZ() / 1._mm,
		   myShape.rOutPlusZ() / 1._mm,
		   myShape.phiFrom(),
		   myShape.deltaPhi()
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::ddtrap) {
    const cms::dd::DDTrap& myShape = cms::dd::DDTrap(fv);
    result = { myShape.halfZ() / 1._mm,
		   myShape.theta(),
		   myShape.phi(),
		   myShape.y1() / 1._mm,
		   myShape.x1() / 1._mm,
		   myShape.x2() / 1._mm,
		   myShape.alpha1(),
		   myShape.y2() / 1._mm,
		   myShape.x3() / 1._mm,
		   myShape.x4() / 1._mm,		 
		   myShape.alpha2()
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::ddtubs) {
    const cms::dd::DDTubs& myShape = cms::dd::DDTubs(fv);
    result = { myShape.zhalf() / 1._mm,
		   myShape.rIn() / 1._mm,
		   myShape.rOut() / 1._mm,
		   myShape.startPhi(),
		   myShape.deltaPhi()
    };
  }
  else if (mySolidShape == cms::DDSolidShape::ddtrunctubs) {
    const cms::dd::DDTruncTubs& myShape = cms::dd::DDTruncTubs(fv);
    result = { myShape.zHalf() / 1._mm,
		   myShape.rIn() / 1._mm,
		   myShape.rOut() / 1._mm,
		   myShape.startPhi(),
		   myShape.deltaPhi(),
		   myShape.cutAtStart() / 1._mm,
		   myShape.cutAtDelta() / 1._mm,
		   static_cast<double>(myShape.cutInside())
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::dd_not_init) {
    auto myShape = fv.solid();
    const std::vector<double>& params = myShape.dimensions();
    if (fv.isA<dd4hep::Trd1>()) {
      result = { params[3] / 1._mm, // z
		     0.,
		     0.,
		     params[2] / 1._mm, // y
		     params[0] / 1._mm, // x1
		     params[0] / 1._mm, // x1
		     0.,
		     params[2] / 1._mm, // y
		     params[1] / 1._mm, // x2
		     params[1] / 1._mm, // x2
		     0.  
      };
    }
    else if (fv.isA<dd4hep::Polycone>()) {
      int counter = 0;
      for (const auto& para : params) {	
	if (counter != 2) {
	  const double factor = (counter >= 2 ? (1. / 1._mm) : 1.);
	  result.emplace_back(para * factor);
	}
	++counter;
      }
    }
  }

  return result;
}


void DetGeomDesc::print() const {
  std::cout << " " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "!!!!!!!!!!!!!!!!    item.name_ = " << m_name << std::endl;
  std::cout << "item.copy_ = " << m_copy << std::endl;
  std::cout << "item.translation = " << std::fixed << std::setprecision(7) << m_trans << std::endl;
  std::cout << "item.rotation = " << std::fixed << std::setprecision(7) << m_rot << std::endl;

  /*
  if (m_isABox) {
    std::cout << "item.getDiamondDimensions() = " << std::fixed << std::setprecision(7) << getDiamondDimensions().xHalfWidth << " " << getDiamondDimensions().yHalfWidth << " " << getDiamondDimensions().zHalfWidth << std::endl;
    }*/
  std::cout << "item.sensorType_ = " << m_sensorType << std::endl;
  //std::cout << "path = " << fv.path() << std::endl;

  std::cout << "item.parentZPosition() = " << std::fixed << std::setprecision(7) << m_z << std::endl;
  if ((int)m_geographicalID() != 0) {
    std::cout << "item.geographicalID() = " << m_geographicalID << std::endl;
  }
  std::cout << "item.materialName() = " << m_mat << std::endl;

  if (!m_allparams.empty()) {
    std::cout << "item.parameters() = " << std::fixed << std::setprecision(7);
    for (const auto& para : m_allparams) {
      std::cout << para << "  ";
    }
    std::cout << " " << std::endl;
  }

}


bool DetGeomDesc::operator<(const DetGeomDesc& other) const { 
  return (name() != other.name() ? name() < other.name() : copyno() < other.copyno()); 
}



DetGeomDesc::DetGeomDesc(const DetGeomDesc& ref) { (*this) = ref; }


DetGeomDesc& DetGeomDesc::operator=(const DetGeomDesc& ref) {
  m_name = ref.m_name;
  m_mat = ref.m_mat;
  m_copy = ref.m_copy;
  m_trans = ref.m_trans;
  m_rot = ref.m_rot;
  m_params = ref.m_params;
  m_allparams = ref.m_allparams;
  m_isABox = ref.m_isABox;
  m_sensorType = ref.m_sensorType;
  m_geographicalID = ref.m_geographicalID;
  m_z = ref.m_z;
  return (*this);
}


DetGeomDesc::~DetGeomDesc() { deepDeleteComponents(); }


DetGeomDesc::Container DetGeomDesc::components() const { return m_container; }


void DetGeomDesc::addComponent(DetGeomDesc* det) { m_container.emplace_back(det); }


DiamondDimensions DetGeomDesc::getDiamondDimensions() const {
  // Convert parameters units from cm (DD4hep standard) to mm (expected by PPS reco software).
  // This implementation is customized for the diamond sensors, which are represented by the 
  // Box shape parameterized by x, y and z half width.
  DiamondDimensions parameters;
  if (isABox()) {
    parameters = { m_params.at(0) / 1._mm, m_params.at(1) / 1._mm, m_params.at(2) / 1._mm };
  }
  else {
    edm::LogError("DetGeomDesc::getDiamondDimensions is not called on a box, for solid ")
      << name() << ", Id = " << geographicalID();
  }
  return parameters;
}


void DetGeomDesc::applyAlignment(const CTPPSRPAlignmentCorrectionData& t) {
  m_rot = t.getRotationMatrix() * m_rot;
  m_trans = t.getTranslation() + m_trans;
}


/*
 * private
 */


void DetGeomDesc::deleteComponents() { m_container.erase(m_container.begin(), m_container.end()); }


void DetGeomDesc::deepDeleteComponents() {
  for (auto& it : m_container) {
    it->deepDeleteComponents();
    delete it;
  }
  clearComponents();
}


std::string DetGeomDesc::computeNameWithNoNamespace(const std::string_view nameFromView) const {
  const auto& semiColonPos = nameFromView.find(":");
  const std::string name { (semiColonPos != std::string::npos 
			    ? nameFromView.substr(semiColonPos + 1) 
			    : nameFromView) };
  return name;
}


std::vector<double> DetGeomDesc::computeParameters(const cms::DDFilteredView& fv) const {
  auto myShape = fv.solid();
  const std::vector<double>& parameters = myShape.dimensions();
  return parameters;
}


DetId DetGeomDesc::computeDetID(const std::string& name, const std::vector<int>& copyNos, unsigned int copyNum) const {
  DetId geoID;

  // strip sensors
  if (name == DDD_TOTEM_RP_SENSOR_NAME) { 
    // check size of copy numbers array
    if (copyNos.size() < 4)
      throw cms::Exception("DDDTotemRPContruction")
	<< "size of copyNumbers for strip sensor is " << copyNos.size() << ". It must be >= 4.";

    // extract information
    const unsigned int decRPId = copyNos[2];
    const unsigned int arm = decRPId / 100;
    const unsigned int station = (decRPId % 100) / 10;
    const unsigned int rp = decRPId % 10;
    const unsigned int detector = copyNos[0];
    geoID = TotemRPDetId(arm, station, rp, detector);
  }

  // strip and pixels RPs
  else if (name == DDD_TOTEM_RP_RP_NAME || name == DDD_CTPPS_PIXELS_RP_NAME) {
    unsigned int decRPId = copyNum;

    // check if it is a pixel RP
    if (decRPId >= 10000) {
      decRPId = decRPId % 10000;
      const unsigned int armIdx = (decRPId / 100) % 10;
      const unsigned int stIdx = (decRPId / 10) % 10;
      const unsigned int rpIdx = decRPId % 10;
      geoID = CTPPSPixelDetId(armIdx, stIdx, rpIdx);
    } else {
      const unsigned int armIdx = (decRPId / 100) % 10;
      const unsigned int stIdx = (decRPId / 10) % 10;
      const unsigned int rpIdx = decRPId % 10;
      geoID = TotemRPDetId(armIdx, stIdx, rpIdx);
    }
  }

  else if (std::regex_match(name, std::regex(DDD_TOTEM_TIMING_SENSOR_TMPL))) {
    // check size of copy numbers array
    if (copyNos.size() < 5)
      throw cms::Exception("DDDTotemRPContruction")
	<< "size of copyNumbers for TOTEM timing sensor is " << copyNos.size() << ". It must be >= 5.";

    const unsigned int decRPId = copyNos[3];
    const unsigned int arm = decRPId / 100, station = (decRPId % 100) / 10, rp = decRPId % 10;
    const unsigned int plane = copyNos[1], channel = copyNos[0];
    geoID = TotemTimingDetId(arm, station, rp, plane, channel);
  }

  else if (name == DDD_TOTEM_TIMING_RP_NAME) {
    const unsigned int arm = copyNum / 100, station = (copyNum % 100) / 10, rp = copyNum % 10;
    geoID = TotemTimingDetId(arm, station, rp);
  }

  // pixel sensors
  else if (name == DDD_CTPPS_PIXELS_SENSOR_NAME) {   
    // check size of copy numbers array
    if (copyNos.size() < 5)
      throw cms::Exception("DDDTotemRPContruction")
	<< "size of copyNumbers for pixel sensor is " << copyNos.size() << ". It must be >= 5.";

    // extract information
    const unsigned int decRPId = copyNos[3] % 10000;
    const unsigned int arm = decRPId / 100;
    const unsigned int station = (decRPId % 100) / 10;
    const unsigned int rp = decRPId % 10;
    const unsigned int detector = copyNos[1] - 1;
    geoID = CTPPSPixelDetId(arm, station, rp, detector);
  }

  // diamond/UFSD sensors
  else if (name == DDD_CTPPS_DIAMONDS_SEGMENT_NAME || name == DDD_CTPPS_UFSD_SEGMENT_NAME) {

    const unsigned int id = copyNos[0];
    const unsigned int arm = copyNos[copyNos.size()-3] - 1;
    const unsigned int station = 1;
    const unsigned int rp = 6;
    const unsigned int plane = (id / 100);
    const unsigned int channel = id % 100;

    geoID = CTPPSDiamondDetId(arm, station, rp, plane, channel);
  }

  // diamond/UFSD RPs
  else if (name == DDD_CTPPS_DIAMONDS_RP_NAME) { 

    // check size of copy numbers array
    if (copyNos.size() < 3)
      throw cms::Exception("DDDTotemRPContruction")
	<< "size of copyNumbers for diamond RP is " << copyNos.size() << ". It must be >= 3.";

    const unsigned int arm = copyNos[(copyNos.size()-3)] - 1;
    const unsigned int station = 1;
    const unsigned int rp = 6;

    geoID = CTPPSDiamondDetId(arm, station, rp);
  }

  return geoID;
}


/*
 * Find out from the name (from DB) or the nodePath (from XMLs), whether a sensor type is 2x2.
 */
std::string DetGeomDesc::computeSensorType(const std::string_view name, const std::string& nodePath, const cms::DDSpecParRegistry& allSpecParSections) {
  std::string sensorType;

  // Case A: Construction from DB.
  // Namespace is present, and allow identification of 2x2 sensor type: just look for "2x2:RPixWafer" in name.
  const auto& found = name.find(DDD_CTPPS_PIXELS_SENSOR_TYPE_2x2 + ":" + DDD_CTPPS_PIXELS_SENSOR_NAME);
  if (found != std::string::npos) {
    sensorType = DDD_CTPPS_PIXELS_SENSOR_TYPE_2x2;
  }


  // Case B: Construction from XMLs.
  // Namespace is not present. XML SPecPar sections allow identification of 2x2 sensor type.
  // If nodePath has a 2x2RPixWafer parameter defined in an XML SPecPar section, sensorType is 2x2.
  const std::string& parameterName = DDD_CTPPS_2x2_RPIXWAFER_PARAMETER_NAME;
  cms::DDSpecParRefs filteredSpecParSections;
  allSpecParSections.filter(filteredSpecParSections, parameterName);
  for (const auto& mySpecParSection : filteredSpecParSections) {
    if (mySpecParSection->hasPath(nodePath)) {
      sensorType = DDD_CTPPS_PIXELS_SENSOR_TYPE_2x2;
    }
  }

  return sensorType;
}
