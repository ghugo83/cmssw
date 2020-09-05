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
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/DDCMS/interface/DDFilteredView.h"
#include "DetectorDescription/DDCMS/interface/DDShapes.h"
#include "DetectorDescription/DDCMS/interface/DDSolidShapes.h"
#include "TGeoMatrix.h"

#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/Math/interface/GeantUnits.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


/*
 *  Constructor from old DD DDFilteredView, also using the SpecPars to access 2x2 wafers info.
 */
DetGeomDesc::DetGeomDesc(const DDFilteredView& fv)
  : m_name(computeNameWithNoNamespace(fv.name())),
    m_copy(fv.copyno()),
    m_trans(fv.translation()),  // mm (legacy)
    m_rot(fv.rotation()),
    m_params(fv.parameters()),  // default unit from old DD (mm)
    m_isABox(fv.shape() == DDSolidShape::ddbox),
    m_sensorType(computeSensorType(fv.name())),
    m_geographicalID(computeDetID(m_name, fv.copyNumbers(), fv.copyno())),
  m_z(fv.translation().z()),   // mm (legacy)
  m_isDD4hep(false),
  m_mat(fv.material()),
    m_allparams(fv.parameters())
{}

  /*
   *  Constructor from DD4Hep DDFilteredView, also using the SpecPars to access 2x2 wafers info.
   */
  DetGeomDesc::DetGeomDesc(const cms::DDFilteredView& fv, const cms::DDSpecParRegistry& allSpecParSections)
    : m_name(computeNameWithNoNamespace(fv.name())),
      m_copy(fv.copyNum()),
      m_trans(geant_units::operators::convertCmToMm(fv.translation())),  // convert cm (DD4hep) to mm (legacy)
      m_rot(fv.rotation()),
      m_params(computeParameters(fv)),  // default unit from DD4hep (cm)
      m_isABox(fv.isABox()),
      m_sensorType(computeSensorType(fv.name(), fv.path(), allSpecParSections)),
      m_geographicalID(computeDetIDFromDD4hep(m_name, fv.copyNos(), fv.copyNum())),
      m_z(geant_units::operators::convertCmToMm(fv.translation().z())),  // convert cm (DD4hep) to mm (legacy)
  m_isDD4hep(true),
 m_mat(fv.materialName()),
  m_allparams(computeParametersTEST(fv))
  {}

DetGeomDesc::DetGeomDesc(const DetGeomDesc& ref) { (*this) = ref; }

DetGeomDesc& DetGeomDesc::operator=(const DetGeomDesc& ref) {
  m_name = ref.m_name;
  m_copy = ref.m_copy;
  m_trans = ref.m_trans;
  m_rot = ref.m_rot;
  m_params = ref.m_params;
  m_isABox = ref.m_isABox;
  m_sensorType = ref.m_sensorType;
  m_geographicalID = ref.m_geographicalID;
  m_z = ref.m_z;
  m_isDD4hep = ref.m_isDD4hep;
  m_mat = ref.m_mat;
  m_allparams = ref.m_allparams;
  return (*this);
}

DetGeomDesc::~DetGeomDesc() { deepDeleteComponents(); }
	  
void DetGeomDesc::addComponent(DetGeomDesc* det) { m_container.emplace_back(det); }

DiamondDimensions DetGeomDesc::getDiamondDimensions() const {
  // Convert parameters units from cm (DD4hep standard) to mm (expected by PPS reco software).
  // This implementation is customized for the diamond sensors, which are represented by the
  // Box shape parameterized by x, y and z half width.
  DiamondDimensions parameters;
  std::cout << "am in DetGeomDesc::getDiamondDimensions(), isDD4hep() = " << isDD4hep() << std::endl;
  if (isABox()) {
    if (!isDD4hep()) {
      // mm (legacy)
      std::cout << "am in DetGeomDesc::getDiamondDimensions(), old DD case:" << std::endl;
      parameters = {m_params.at(0),
		    m_params.at(1),
		    m_params.at(2)};
    } else {
      // convert cm (DD4hep) to mm (legacy)
      parameters = {geant_units::operators::convertCmToMm(m_params.at(0)),
		    geant_units::operators::convertCmToMm(m_params.at(1)),
		    geant_units::operators::convertCmToMm(m_params.at(2))};
    }
  } else {
    edm::LogError("DetGeomDesc::getDiamondDimensions is not called on a box, for solid ")
      << name() << ", Id = " << geographicalID();
  }
  return parameters;
}

void DetGeomDesc::applyAlignment(const CTPPSRPAlignmentCorrectionData& t) {
  m_rot = t.getRotationMatrix() * m_rot;
  m_trans = t.getTranslation() + m_trans;
}


void DetGeomDesc::print() const {
  edm::LogVerbatim("DetGeomDesc::print") << " " << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << " " << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << " " << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << "............................." << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << "name = " << m_name << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << "copy = " << m_copy << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << "translation = " << std::fixed << std::setprecision(7) << m_trans
                                         << std::endl;
  edm::LogVerbatim("DetGeomDesc::print") << "rotation = " << std::fixed << std::setprecision(7) << m_rot << std::endl;

  if (m_isABox) {
    edm::LogVerbatim("DetGeomDesc::print")
        << "getDiamondDimensions() = " << std::fixed << std::setprecision(7) << getDiamondDimensions().xHalfWidth << " "
        << getDiamondDimensions().yHalfWidth << " " << getDiamondDimensions().zHalfWidth << std::endl;
  }

  edm::LogVerbatim("DetGeomDesc::print") << "sensorType = " << m_sensorType << std::endl;

  if (m_geographicalID() != 0) {
    edm::LogVerbatim("DetGeomDesc::print") << "geographicalID() = " << m_geographicalID << std::endl;
  }

  edm::LogVerbatim("DetGeomDesc::print") << "parentZPosition() = " << std::fixed << std::setprecision(7) << m_z
                                         << std::endl;

  std::cout << "item.materialName() = " << m_mat << std::endl;
  if (!m_allparams.empty()) {
    std::cout << "item.parameters() = " << std::fixed << std::setprecision(7);
    for (const auto& para : m_allparams) {
      std::cout << para << "  ";
    }
    std::cout << " " << std::endl;
  }
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
  const std::string name{(semiColonPos != std::string::npos ? nameFromView.substr(semiColonPos + 1) : nameFromView)};
  return name;
}

std::vector<double> DetGeomDesc::computeParameters(const cms::DDFilteredView& fv) const {
  auto myShape = fv.solid();
  const std::vector<double>& parameters = myShape.dimensions();  // default unit from DD4hep (cm)
  return parameters;
}


std::vector<double> DetGeomDesc::computeParametersTEST(const cms::DDFilteredView& fv) const {

  std::vector<double> result;

  const cms::DDSolidShape& mySolidShape = cms::dd::getCurrentShape(fv);

  if (mySolidShape == cms::DDSolidShape::ddbox) {
    const cms::dd::DDBox& myShape = cms::dd::DDBox(fv);
    result = { geant_units::operators::convertCmToMm(myShape.halfX() ),
		   geant_units::operators::convertCmToMm(myShape.halfY() ),
		   geant_units::operators::convertCmToMm(myShape.halfZ() )
    };
  }
  else if (mySolidShape == cms::DDSolidShape::ddcons) {
    const cms::dd::DDCons& myShape = cms::dd::DDCons(fv);
    result = { geant_units::operators::convertCmToMm(myShape.zhalf() ),
		   geant_units::operators::convertCmToMm(myShape.rInMinusZ() ),
		   geant_units::operators::convertCmToMm(myShape.rOutMinusZ() ),
		   geant_units::operators::convertCmToMm(myShape.rInPlusZ() ),
		   geant_units::operators::convertCmToMm(myShape.rOutPlusZ() ),
		   myShape.phiFrom(),
		   myShape.deltaPhi()
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::ddtrap) {
    const cms::dd::DDTrap& myShape = cms::dd::DDTrap(fv);
    result = { geant_units::operators::convertCmToMm(myShape.halfZ() ),
		   myShape.theta(),
		   myShape.phi(),
		   geant_units::operators::convertCmToMm(myShape.y1() ),
		   geant_units::operators::convertCmToMm(myShape.x1() ),
		   geant_units::operators::convertCmToMm(myShape.x2() ),
		   myShape.alpha1(),
		   geant_units::operators::convertCmToMm(myShape.y2() ),
		   geant_units::operators::convertCmToMm(myShape.x3() ),
		   geant_units::operators::convertCmToMm(myShape.x4() ),		 
		   myShape.alpha2()
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::ddtubs) {
    const cms::dd::DDTubs& myShape = cms::dd::DDTubs(fv);
    result = { geant_units::operators::convertCmToMm(myShape.zhalf() ),
		   geant_units::operators::convertCmToMm(myShape.rIn() ),
		   geant_units::operators::convertCmToMm(myShape.rOut() ),
		   myShape.startPhi(),
		   myShape.deltaPhi()
    };
  }
  else if (mySolidShape == cms::DDSolidShape::ddtrunctubs) {
    const cms::dd::DDTruncTubs& myShape = cms::dd::DDTruncTubs(fv);
    result = { geant_units::operators::convertCmToMm(myShape.zHalf() ),
		   geant_units::operators::convertCmToMm(myShape.rIn() ),
		   geant_units::operators::convertCmToMm(myShape.rOut() ),
		   myShape.startPhi(),
		   myShape.deltaPhi(),
		   geant_units::operators::convertCmToMm(myShape.cutAtStart() ),
		   geant_units::operators::convertCmToMm(myShape.cutAtDelta() ),
		   static_cast<double>(myShape.cutInside())
    }; 
  }
  else if (mySolidShape == cms::DDSolidShape::dd_not_init) {
    auto myShape = fv.solid();
    const std::vector<double>& params = myShape.dimensions();
    if (fv.isA<dd4hep::Trd1>()) {
      result = { geant_units::operators::convertCmToMm(params[3] ), // z
		     0.,
		     0.,
		     geant_units::operators::convertCmToMm(params[2] ), // y
		     geant_units::operators::convertCmToMm(params[0] ), // x1
		     geant_units::operators::convertCmToMm(params[0] ), // x1
		     0.,
		     geant_units::operators::convertCmToMm(params[2] ), // y
		     geant_units::operators::convertCmToMm(params[1] ), // x2
		     geant_units::operators::convertCmToMm(params[1] ), // x2
		     0.  
      };
    }
    else if (fv.isA<dd4hep::Polycone>()) {
      int counter = 0;
      for (const auto& para : params) {	
	if (counter != 2) {
	  const double factor = (counter >= 2 ? (10.) : 1.);
	  result.emplace_back(para * factor);
	}
	++counter;
      }
    }
  }

  return result;
}


/*
 * old DD DetId computation.
 * Relies on name and volumes copy numbers.
 */
DetId DetGeomDesc::computeDetID(const std::string& name, const std::vector<int>& copyNos, unsigned int copyNum) const {
  DetId geoID;

  // strip sensors
  if (name == DDD_TOTEM_RP_SENSOR_NAME) {
    // check size of copy numbers array
    if (copyNos.size() < 3)
      throw cms::Exception("DDDTotemRPContruction")
          << "size of copyNumbers for strip sensor is " << copyNos.size() << ". It must be >= 3.";

    // extract information
    const unsigned int decRPId = copyNos[copyNos.size() - 3];
    const unsigned int arm = decRPId / 100;
    const unsigned int station = (decRPId % 100) / 10;
    const unsigned int rp = decRPId % 10;
    const unsigned int detector = copyNos[copyNos.size() - 1];
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
    if (copyNos.size() < 4)
      throw cms::Exception("DDDTotemRPContruction")
          << "size of copyNumbers for TOTEM timing sensor is " << copyNos.size() << ". It must be >= 4.";

    const unsigned int decRPId = copyNos[copyNos.size() - 4];
    const unsigned int arm = decRPId / 100, station = (decRPId % 100) / 10, rp = decRPId % 10;
    const unsigned int plane = copyNos[copyNos.size() - 2], channel = copyNos[copyNos.size() - 1];
    geoID = TotemTimingDetId(arm, station, rp, plane, channel);
  }

  else if (name == DDD_TOTEM_TIMING_RP_NAME) {
    const unsigned int arm = copyNum / 100, station = (copyNum % 100) / 10, rp = copyNum % 10;
    geoID = TotemTimingDetId(arm, station, rp);
  }

  // pixel sensors
  else if (name == DDD_CTPPS_PIXELS_SENSOR_NAME) {
    // check size of copy numbers array
    if (copyNos.size() < 4)
      throw cms::Exception("DDDTotemRPContruction")
          << "size of copyNumbers for pixel sensor is " << copyNos.size() << ". It must be >= 4.";

    // extract information
    const unsigned int decRPId = copyNos[copyNos.size() - 4] % 10000;
    const unsigned int arm = decRPId / 100;
    const unsigned int station = (decRPId % 100) / 10;
    const unsigned int rp = decRPId % 10;
    const unsigned int detector = copyNos[copyNos.size() - 2] - 1;
    geoID = CTPPSPixelDetId(arm, station, rp, detector);
  }

  // diamond/UFSD sensors
  else if (name == DDD_CTPPS_DIAMONDS_SEGMENT_NAME || name == DDD_CTPPS_UFSD_SEGMENT_NAME) {
    const unsigned int id = copyNos[copyNos.size() - 1];
    const unsigned int arm = copyNos[1] - 1;
    const unsigned int station = 1;
    const unsigned int rp = 6;
    const unsigned int plane = (id / 100);
    const unsigned int channel = id % 100;

    geoID = CTPPSDiamondDetId(arm, station, rp, plane, channel);
  }

  // diamond/UFSD RPs
  else if (name == DDD_CTPPS_DIAMONDS_RP_NAME) {
    // check size of copy numbers array
    if (copyNos.size() < 2)
      throw cms::Exception("DDDTotemRPContruction")
          << "size of copyNumbers for diamond RP is " << copyNos.size() << ". It must be >= 2.";

    const unsigned int arm = copyNos[1] - 1;
    const unsigned int station = 1;
    const unsigned int rp = 6;

    geoID = CTPPSDiamondDetId(arm, station, rp);
  }

  return geoID;
}


/*
 * DD4hep DetId computation.
 */
DetId DetGeomDesc::computeDetIDFromDD4hep(const std::string& name, const std::vector<int>& copyNos, unsigned int copyNum) const {
  std::vector<int> copyNosOldDD = { copyNos.rbegin() + 1, copyNos.rend() };

  return computeDetID(name, copyNosOldDD, copyNum);
}


/*
 * old DD sensor type computation.
 * Find out from the namespace, whether a sensor type is 2x2.
 */
std::string DetGeomDesc::computeSensorType(const std::string_view name) {
  std::string sensorType;

  // Namespace is present, and allow identification of 2x2 sensor type: just look for "2x2:RPixWafer" in name.
  const auto& found = name.find(DDD_CTPPS_PIXELS_SENSOR_TYPE_2x2 + ":" + DDD_CTPPS_PIXELS_SENSOR_NAME);
  if (found != std::string::npos) {
    sensorType = DDD_CTPPS_PIXELS_SENSOR_TYPE_2x2;
  }

  return sensorType;
}


/*
 * DD4hep sensor type computation.
 * Find out from the namespace (from DB) or the nodePath (from XMLs), whether a sensor type is 2x2.
 */
std::string DetGeomDesc::computeSensorType(const std::string_view name,
                                           const std::string& nodePath,
                                           const cms::DDSpecParRegistry& allSpecParSections) {
  std::string sensorType;

  // Case A: Construction from DB.
  // Namespace is present, and allow identification of 2x2 sensor type: just look for "2x2:RPixWafer" in name.
  sensorType = computeSensorType(name);

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
