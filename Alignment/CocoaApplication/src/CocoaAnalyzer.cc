#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/DataRecord/interface/OpticalAlignmentsRcd.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurementInfo.h"
#include "DataFormats/Math/interface/CMSUnits.h"
#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/DDCMS/interface/DDFilteredView.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "Alignment/CocoaApplication/interface/CocoaAnalyzer.h"
#include "Alignment/CocoaUtilities/interface/ALIUtils.h"
#include "Alignment/CocoaModel/interface/Model.h"
#include "Alignment/CocoaFit/interface/Fit.h"
#include "Alignment/CocoaModel/interface/Entry.h"
#include "Alignment/CocoaUtilities/interface/ALIFileOut.h"
#include "Alignment/CocoaModel/interface/CocoaDaqReaderRoot.h"
#include "Alignment/CocoaModel/interface/OpticalObject.h"
#include "Alignment/CocoaUtilities/interface/GlobalOptionMgr.h"
#include "Alignment/CocoaFit/interface/CocoaDBMgr.h"

using namespace cms_units::operators;

CocoaAnalyzer::CocoaAnalyzer(edm::ParameterSet const& pset) {
  theCocoaDaqRootFileName_ = pset.getParameter<std::string>("cocoaDaqRootFile");
  int maxEvents = pset.getParameter<int32_t>("maxEvents");
  GlobalOptionMgr::getInstance()->setDefaultGlobalOptions();
  GlobalOptionMgr::getInstance()->setGlobalOption("maxEvents", maxEvents);
  GlobalOptionMgr::getInstance()->setGlobalOption("writeDBAlign", 1);
  GlobalOptionMgr::getInstance()->setGlobalOption("writeDBOptAlign", 1);
  usesResource("CocoaAnalyzer");
}

void CocoaAnalyzer::beginJob() {}

void CocoaAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& evts) {
  ALIUtils::setDebugVerbosity(5);

  // Get ideal geometry description + measurements for simulation.
  readXMLFile(evts);

  // Correct ideal geometry with data from DB.
  std::vector<OpticalAlignInfo> oaListCalib = readCalibrationDB(evts);
  correctOptAlignments(oaListCalib);

  // Run the least-squared fit and store results in DB.
  runCocoa();
}

/*
 * This is used to create the ideal geometry description from the XMLs.
 * Also get measurements from XMLs for simulation.
 * Resulting optical alignment info is stored in oaList_ and measList_.
 */
void CocoaAnalyzer::readXMLFile(const edm::EventSetup& evts) {
  edm::ESTransientHandle<cms::DDCompactView> myCompactView;
  evts.get<IdealGeometryRecord>().get(myCompactView);

  const cms::DDDetector* mySystem = myCompactView->detector();

  if (mySystem) {
    // Always store world volume first.
    const dd4hep::Volume& worldVolume = mySystem->worldVolume();

    if (ALIUtils::debug >= 3) {
      LogDebug("Alignment") << "CocoaAnalyzer::ReadXML: world object = " << worldVolume.name();
    }

    OpticalAlignInfo worldInfo;
    worldInfo.ID_ = 0;
    worldInfo.name_ = worldVolume.name();
    worldInfo.type_ = "system";
    worldInfo.parentName_ = "";
    worldInfo.x_.value_ = 0.;
    worldInfo.x_.error_ = 0.;
    worldInfo.x_.quality_ = 0;
    worldInfo.y_.value_ = 0.;
    worldInfo.y_.error_ = 0.;
    worldInfo.y_.quality_ = 0;
    worldInfo.z_.value_ = 0.;
    worldInfo.z_.error_ = 0.;
    worldInfo.z_.quality_ = 0;
    worldInfo.angx_.value_ = 0.;
    worldInfo.angx_.error_ = 0.;
    worldInfo.angx_.quality_ = 0;
    worldInfo.angy_.value_ = 0.;
    worldInfo.angy_.error_ = 0.;
    worldInfo.angy_.quality_ = 0;
    worldInfo.angz_.value_ = 0.;
    worldInfo.angz_.error_ = 0.;
    worldInfo.angz_.quality_ = 0;
    oaList_.opticalAlignments_.emplace_back(worldInfo);

    // This gathers all the 'SpecPar' sections from the loaded XMLs.
    // NB: Definition of a SpecPar section:
    // It is a block in the XML file(s), containing paths to specific volumes,
    // and ALLOWING THE ASSOCIATION OF SPECIFIC PARAMETERS AND VALUES TO THESE VOLUMES.
    const cms::DDSpecParRegistry& allSpecParSections = myCompactView->specpars();

    // CREATION OF A COCOA FILTERED VIEW
    // Creation of the dd4hep-based filtered view.
    // NB: not filtered yet!
    cms::DDFilteredView myFilteredView(mySystem, worldVolume);
    // Declare a container which will gather all the filtered SpecPar sections.
    cms::DDSpecParRefs cocoaParameterSpecParSections;
    // Define a COCOA filter
    const std::string cocoaParameterAttribute = "COCOA";
    const std::string cocoaParameterValue = "COCOA";
    // All the COCOA SpecPar sections are filtered from allSpecParSections,
    // and assigned to cocoaParameterSpecParSections.
    allSpecParSections.filter(cocoaParameterSpecParSections, cocoaParameterAttribute, cocoaParameterValue);
    // This finally allows to filter the filtered view, with the COCOA filter.
    // This means that we now have, in myFilteredView, all volumes whose paths were selected:
    // ie all volumes with "COCOA" parameter and value in a SpecPar section from a loaded XML.
    myFilteredView.mergedSpecifics(cocoaParameterSpecParSections);

    // Loop on parts
    int nObjects = 0;
    bool doCOCOA = myFilteredView.firstChild();

    // Loop on all COCOA volumes from filtered view
    while (doCOCOA) {
      ++nObjects;

      OpticalAlignInfo oaInfo;
      OpticalAlignParam oaParam;
      OpticalAlignMeasurementInfo oaMeas;

      // Current volume
      const dd4hep::PlacedVolume& myPlacedVolume = myFilteredView.volume();
      const std::string& name = myPlacedVolume.name();
      const std::string& nodePath = myFilteredView.path();
      oaInfo.name_ = nodePath;

      // Parent name
      oaInfo.parentName_ = nodePath.substr(0, nodePath.rfind('/', nodePath.length()));

      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << " CocoaAnalyzer::ReadXML reading object " << name;
        LogDebug("Alignment") << " @@ Name built= " << oaInfo.name_ << " short_name= " << name << " parent= " << oaInfo.parentName_;
      }

      // TRANSLATIONS

      // A) GET TRANSLATIONS FROM DDETECTOR.
      // Directly get translation from parent to child volume
      const dd4hep::Direction& transl = myPlacedVolume.position();

      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << "Local translation in cm = " << transl;
      }

      // B) READ INFO FROM XMLS
      // X
      oaInfo.x_.name_ = "X";
      oaInfo.x_.dim_type_ = "centre";
      oaInfo.x_.value_ = transl.x() / (1._m);  // COCOA units are m
      oaInfo.x_.error_ = getParameterValueFromSpecParSections<double>(allSpecParSections,
                                                                      nodePath,
                                                                      "centre_X_sigma",
                                                                      0) /
	(1._m);  // COCOA units are m
      oaInfo.x_.quality_ = static_cast<int>(
					    getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "centre_X_quality", 0));
      // Y
      oaInfo.y_.name_ = "Y";
      oaInfo.y_.dim_type_ = "centre";
      oaInfo.y_.value_ = transl.y() / (1._m);  // COCOA units are m
      oaInfo.y_.error_ = getParameterValueFromSpecParSections<double>(allSpecParSections,
                                                                      nodePath,
                                                                      "centre_Y_sigma",
                                                                      0) /
	(1._m);  // COCOA units are m
      oaInfo.y_.quality_ = static_cast<int>(
					    getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "centre_Y_quality", 0));
      // Z
      oaInfo.z_.name_ = "Z";
      oaInfo.z_.dim_type_ = "centre";
      oaInfo.z_.value_ = transl.z() / (1._m);  // COCOA units are m
      oaInfo.z_.error_ = getParameterValueFromSpecParSections<double>(allSpecParSections,
                                                                      nodePath,
                                                                      "centre_Z_sigma",
                                                                      0) /
	(1._m);  // COCOA units are m
      oaInfo.z_.quality_ = static_cast<int>(
					    getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "centre_Z_quality", 0));

      // ROTATIONS

      // A) GET ROTATIONS FROM DDETECTOR.

      // Unlike in the initial code, here we manage to directly get the rotation matrix placement
      // of the child in parent, EXPRESSED IN THE PARENT FRAME OF REFERENCE.
      // Hence the (ugly) initial block of code is replaced by just 2 lines.
      // PlacedVolume::matrix() returns the rotation matrix IN THE PARENT FRAME OF REFERENCE.
      // NB: Not using DDFilteredView::rotation(),
      // because it returns the rotation matrix IN THE WORLD FRAME OF REFERENCE.
      const TGeoHMatrix parentToChild = myPlacedVolume.matrix();
      // COCOA convention is FROM CHILD TO PARENT
      const TGeoHMatrix& childToParent = parentToChild.Inverse();

      // Convert it to CLHEP::Matrix
      // Below is not my code, below block is untouched (apart from bug fix).
      // I would just directly use childToParent...
      const Double_t* rot = childToParent.GetRotationMatrix();
      const double xx = rot[0];
      const double xy = rot[1];
      const double xz = rot[2];
      const double yx = rot[3];
      const double yy = rot[4];
      const double yz = rot[5];
      const double zx = rot[6];
      const double zy = rot[7];
      const double zz = rot[8];
      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << "Local rotation = ";
        LogDebug("Alignment") << xx << "  " << xy << "  " << xz;
        LogDebug("Alignment") << yx << "  " << yy << "  " << yz;
        LogDebug("Alignment") << zx << "  " << zy << "  " << zz;
      }
      const CLHEP::Hep3Vector colX(xx, yx, zx);
      const CLHEP::Hep3Vector colY(xy, yy, zy);
      const CLHEP::Hep3Vector colZ(xz, yz, zz);
      const CLHEP::HepRotation rotclhep(colX, colY, colZ);
      const std::vector<double>& angles = ALIUtils::getRotationAnglesFromMatrix(rotclhep, 0., 0., 0.);

      // B) READ INFO FROM XMLS
      // X
      oaInfo.angx_.name_ = "X";
      oaInfo.angx_.dim_type_ = "angles";
      oaInfo.angx_.value_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_X_value", 0);
      oaInfo.angx_.error_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_X_sigma", 0);
      oaInfo.angx_.quality_ = static_cast<int>(
					       getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_X_quality", 0));
      // Y
      oaInfo.angy_.name_ = "Y";
      oaInfo.angy_.dim_type_ = "angles";
      oaInfo.angy_.value_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Y_value", 0);
      oaInfo.angy_.error_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Y_sigma", 0);
      oaInfo.angy_.quality_ = static_cast<int>(
					       getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Y_quality", 0));
      // Z
      oaInfo.angz_.name_ = "Z";
      oaInfo.angz_.dim_type_ = "angles";
      oaInfo.angz_.value_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Z_value", 0);
      oaInfo.angz_.error_ =
	getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Z_sigma", 0);
      oaInfo.angz_.quality_ = static_cast<int>(
					       getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "angles_Z_quality", 0));

      oaInfo.type_ = getParameterValueFromSpecParSections<std::string>(allSpecParSections, nodePath, "cocoa_type", 0);

      oaInfo.ID_ =
	static_cast<int>(getParameterValueFromSpecParSections<double>(allSpecParSections, nodePath, "cmssw_ID", 0));

      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << "CocoaAnalyzer::ReadXML OBJECT " << oaInfo.name_ << " pos/angles read ";
      }

      // Check that rotations match with values from XMLs.
      // Same, that ugly code is not mine ;p
      if (fabs(oaInfo.angx_.value_ - angles[0]) > 1.E-9 || fabs(oaInfo.angy_.value_ - angles[1]) > 1.E-9 ||
          fabs(oaInfo.angz_.value_ - angles[2]) > 1.E-9) {
        std::cerr << " WRONG ANGLE IN OBJECT " << oaInfo.name_ << oaInfo.angx_.value_ << " =? " << angles[0]
                  << oaInfo.angy_.value_ << " =? " << angles[1] << oaInfo.angz_.value_ << " =? " << angles[2]
	  ;
      }

      // EXTRA PARAM ENTRIES (FROM XMLS)
      // Here initial code to define the containers was fully removed, this is much more compact.
      const std::vector<std::string>& names =
	getAllParameterValuesFromSpecParSections<std::string>(allSpecParSections, nodePath, "extra_entry");
      const std::vector<std::string>& dims =
	getAllParameterValuesFromSpecParSections<std::string>(allSpecParSections, nodePath, "dimType");
      const std::vector<double>& values =
	getAllParameterValuesFromSpecParSections<double>(allSpecParSections, nodePath, "value");
      const std::vector<double>& errors =
	getAllParameterValuesFromSpecParSections<double>(allSpecParSections, nodePath, "sigma");
      const std::vector<double>& quality =
	getAllParameterValuesFromSpecParSections<double>(allSpecParSections, nodePath, "quality");

      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << " CocoaAnalyzer::ReadXML:  Fill extra entries with read parameters ";
      }

      if (names.size() == dims.size() && dims.size() == values.size() && values.size() == errors.size() &&
          errors.size() == quality.size()) {
        for (size_t i = 0; i < names.size(); ++i) {
          double dimFactor = 1.;
          const std::string& type = dims[i];
          if (type == "centre" || type == "length") {
            dimFactor = 1. / (1._m);  // was converted to cm with getParameterValueFromSpecPar, COCOA unit is m
          } else if (type == "angles" || type == "angle" || type == "nodim") {
            dimFactor = 1.;
          }
          oaParam.value_ = values[i] * dimFactor;
          oaParam.error_ = errors[i] * dimFactor;
          oaParam.quality_ = static_cast<int>(quality[i]);
          oaParam.name_ = names[i];
          oaParam.dim_type_ = dims[i];
          oaInfo.extraEntries_.emplace_back(oaParam);
          oaParam.clear();
        }

        oaList_.opticalAlignments_.emplace_back(oaInfo);
      } else {
        LogDebug("Alignment") << "WARNING FOR NOW: sizes of extra parameters (names, dimType, value, quality) do"
			      << " not match!  Did not add " << nObjects << " item to OpticalAlignments.";
      }

      // MEASUREMENTS (FROM XMLS)
      const std::vector<std::string>& measNames =
	getAllParameterValuesFromSpecParSections<std::string>(allSpecParSections, nodePath, "meas_name");
      const std::vector<std::string>& measTypes =
	getAllParameterValuesFromSpecParSections<std::string>(allSpecParSections, nodePath, "meas_type");

      std::map<std::string, std::vector<std::string>> measObjectNames;
      std::map<std::string, std::vector<std::string>> measParamNames;
      std::map<std::string, std::vector<double>> measParamValues;
      std::map<std::string, std::vector<double>> measParamSigmas;
      std::map<std::string, std::vector<double>> measIsSimulatedValue;
      for (const auto& name : measNames) {
        measObjectNames[name] = getAllParameterValuesFromSpecParSections<std::string>(
										      allSpecParSections, nodePath, "meas_object_name_" + name);
        measParamNames[name] = getAllParameterValuesFromSpecParSections<std::string>(
										     allSpecParSections, nodePath, "meas_value_name_" + name);
        measParamValues[name] =
	  getAllParameterValuesFromSpecParSections<double>(allSpecParSections, nodePath, "meas_value_" + name);
        measParamSigmas[name] =
	  getAllParameterValuesFromSpecParSections<double>(allSpecParSections, nodePath, "meas_sigma_" + name);
        measIsSimulatedValue[name] = getAllParameterValuesFromSpecParSections<double>(
										      allSpecParSections, nodePath, "meas_is_simulated_value_" + name);
      }

      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << " CocoaAnalyzer::ReadXML:  Fill measurements with read parameters ";
      }

      if (measNames.size() == measTypes.size()) {
        for (size_t i = 0; i < measNames.size(); ++i) {
          oaMeas.ID_ = i;
          oaMeas.name_ = measNames[i];
          oaMeas.type_ = measTypes[i];
          oaMeas.measObjectNames_ = measObjectNames[oaMeas.name_];
          if (measParamNames.size() == measParamValues.size() && measParamValues.size() == measParamSigmas.size()) {
            for (size_t i2 = 0; i2 < measParamNames[oaMeas.name_].size(); i2++) {
              oaParam.name_ = measParamNames[oaMeas.name_][i2];
              oaParam.value_ = measParamValues[oaMeas.name_][i2];
              oaParam.error_ = measParamSigmas[oaMeas.name_][i2];
              oaParam.quality_ = 2;
              if (oaMeas.type_ == "SENSOR2D" || oaMeas.type_ == "COPS" || oaMeas.type_ == "DISTANCEMETER" ||
                  oaMeas.type_ == "DISTANCEMETER!DIM" || oaMeas.type_ == "DISTANCEMETER3DIM") {
                oaParam.dim_type_ = "length";
              } else if (oaMeas.type_ == "TILTMETER") {
                oaParam.dim_type_ = "angle";
              } else {
                std::cerr << "CocoaAnalyzer::readXMLFile. Invalid measurement type: " << oaMeas.type_;
                std::exception();
              }

              oaMeas.values_.emplace_back(oaParam);
              oaMeas.isSimulatedValue_.emplace_back(measIsSimulatedValue[oaMeas.name_][i2]);
              if (ALIUtils::debug >= 5) {
                LogDebug("Alignment") << oaMeas.name_ << " copying issimu "
				      << oaMeas.isSimulatedValue_[oaMeas.isSimulatedValue_.size() - 1] << " = "
				      << measIsSimulatedValue[oaMeas.name_][i2];
              }
              oaParam.clear();
            }
          } else {
            if (ALIUtils::debug >= 2) {
	      edm::LogWarning("Alignment") << "WARNING FOR NOW: sizes of measurement parameters (name, value, sigma) do"
					   << " not match! for measurement " << oaMeas.name_
					   << " !Did not fill parameters for this measurement ";
            }
          }
          measList_.oaMeasurements_.emplace_back(oaMeas);
          if (ALIUtils::debug >= 5) {
            LogDebug("Alignment") << "CocoaAnalyser: MEASUREMENT " << oaMeas.name_ << " extra entries read " << oaMeas;
          }
          oaMeas.clear();
        }

      } else {
        if (ALIUtils::debug >= 2) {
          edm::LogWarning("Alignment") << "WARNING FOR NOW: sizes of measurements (names, types do"
				       << " not match!  Did not add " << nObjects << " item to XXXMeasurements";
        }
      }

      oaInfo.clear();
      doCOCOA = myFilteredView.firstChild();
    }  // while (doCOCOA)

    if (ALIUtils::debug >= 3) {
      LogDebug("Alignment") << "CocoaAnalyzer::ReadXML: Finished building " << nObjects + 1 << " OpticalAlignInfo objects"
			    << " and " << measList_.oaMeasurements_.size() << " OpticalAlignMeasurementInfo objects ";
    }
    if (ALIUtils::debug >= 5) {
      LogDebug("Alignment") << " @@@@@@ OpticalAlignments " << oaList_;
      LogDebug("Alignment") << " @@@@@@ OpticalMeasurements " << measList_;
    }
  }
}

/*
 * This is used to get the OpticalAlignInfo from DB, 
 * which can be used to correct the OpticalAlignInfo from IdealGeometry.
 */
std::vector<OpticalAlignInfo> CocoaAnalyzer::readCalibrationDB(const edm::EventSetup& evts) {
  if (ALIUtils::debug >= 3) {
    LogDebug("Alignment") << "$$$ CocoaAnalyzer::readCalibrationDB: ";
  }

  using namespace edm::eventsetup;
  edm::ESHandle<OpticalAlignments> pObjs;
  evts.get<OpticalAlignmentsRcd>().get(pObjs);
  const std::vector<OpticalAlignInfo>& infoFromDB = pObjs.product()->opticalAlignments_;

  if (ALIUtils::debug >= 5) {
    LogDebug("Alignment") << "CocoaAnalyzer::readCalibrationDB:  Number of OpticalAlignInfo READ " << infoFromDB.size();
    for (const auto& myInfoFromDB : infoFromDB) {
      LogDebug("Alignment") << "CocoaAnalyzer::readCalibrationDB:  OpticalAlignInfo READ " << myInfoFromDB;
    }
  }

  return infoFromDB;
}

/*
 * Correct the OpticalAlignInfo from IdealGeometry with values from DB.
 */
void CocoaAnalyzer::correctOptAlignments(std::vector<OpticalAlignInfo>& oaListCalib) {
  if (ALIUtils::debug >= 3) {
    LogDebug("Alignment") << "$$$ CocoaAnalyzer::correctOptAlignments: ";
  }

  std::vector<OpticalAlignInfo>::const_iterator it;
  for (it = oaListCalib.begin(); it != oaListCalib.end(); ++it) {
    OpticalAlignInfo oaInfoDB = *it;
    OpticalAlignInfo* oaInfoXML = findOpticalAlignInfoXML(oaInfoDB);
    if (oaInfoXML == nullptr) {
      if (ALIUtils::debug >= 2) {
        std::cerr << "@@@@@ WARNING CocoaAnalyzer::correctOptAlignments:  OpticalAlignInfo read from DB is not present "
	  "in XML "
                  << *it;
      }
    } else {
      //------ Correct info
      if (ALIUtils::debug >= 5) {
        LogDebug("Alignment") << "CocoaAnalyzer::correctOptAlignments: correcting data from DB info ";
      }
      correctOaParam(&oaInfoXML->x_, oaInfoDB.x_);
      correctOaParam(&oaInfoXML->y_, oaInfoDB.y_);
      correctOaParam(&oaInfoXML->z_, oaInfoDB.z_);
      correctOaParam(&oaInfoXML->angx_, oaInfoDB.angx_);
      correctOaParam(&oaInfoXML->angy_, oaInfoDB.angy_);
      correctOaParam(&oaInfoXML->angz_, oaInfoDB.angz_);
      std::vector<OpticalAlignParam>::iterator itoap1, itoap2;
      std::vector<OpticalAlignParam> extraEntDB = oaInfoDB.extraEntries_;
      std::vector<OpticalAlignParam>* extraEntXML = &(oaInfoXML->extraEntries_);
      for (itoap1 = extraEntDB.begin(); itoap1 != extraEntDB.end(); ++itoap1) {
        bool pFound = false;
        //----- Look for the extra parameter in XML oaInfo that has the same name
        std::string oaName = (*itoap1).name_;
        for (itoap2 = extraEntXML->begin(); itoap2 != extraEntXML->end(); ++itoap2) {
          if (oaName == itoap2->name_) {
            correctOaParam(&(*itoap2), *itoap1);
            pFound = true;
            break;
          }
        }
        if (!pFound && oaName != "None") {
          if (ALIUtils::debug >= 2) {
            std::cerr
	      << "@@@@@ WARNING CocoaAnalyzer::correctOptAlignments:  extra entry read from DB is not present in XML "
	      << *itoap1 << " in object " << *it;
          }
        }
      }
      if (ALIUtils::debug >= 5) {
        LogDebug("Alignment") << "CocoaAnalyzer::correctOptAlignments: corrected OpticalAlingInfo " << oaList_;
      }
    }
  }
}

OpticalAlignInfo* CocoaAnalyzer::findOpticalAlignInfoXML(const OpticalAlignInfo& oaInfo) {
  OpticalAlignInfo* oaInfoXML = nullptr;

  for (auto& myOpticalAlignInfo : oaList_.opticalAlignments_) {
    if (ALIUtils::debug >= 5) {
      LogDebug("Alignment") << "CocoaAnalyzer::findOpticalAlignInfoXML:  looking for OAI " << myOpticalAlignInfo.name_ << " =? " << oaInfo.name_;
    }
    if (myOpticalAlignInfo.name_ == oaInfo.name_) {
      oaInfoXML = &(myOpticalAlignInfo);
      if (ALIUtils::debug >= 4) {
        LogDebug("Alignment") << "CocoaAnalyzer::findOpticalAlignInfoXML:  OAI found " << oaInfoXML->name_;
      }
      break;
    }
  }

  return oaInfoXML;
}

bool CocoaAnalyzer::correctOaParam(OpticalAlignParam* oaParamXML, const OpticalAlignParam& oaParamDB) {
  if (ALIUtils::debug >= 4) {
    LogDebug("Alignment") << "CocoaAnalyzer::correctOaParam  old value= " << oaParamXML->value_
			  << " new value= " << oaParamDB.value_;
  }
  if (oaParamDB.value_ == -9.999E9)
    return false;

  std::string type = oaParamDB.dimType();
  double dimFactor = 1.;
  if (type == "centre" || type == "length") {
    dimFactor = 1. / 1._m;  // in DB it is in cm
  } else if (type == "angles" || type == "angle" || type == "nodim") {
    dimFactor = 1.;
  } else {
    std::cerr << "!!! COCOA programming error: inform responsible: incorrect OpticalAlignParam type = " << type
      ;
    std::exception();
  }

  oaParamXML->value_ = oaParamDB.value_ * dimFactor;

  return true;
}

/*
 * Collect all information, do the fitting, and store results in DB.
 */
void CocoaAnalyzer::runCocoa() {
  if (ALIUtils::debug >= 3) {
    LogDebug("Alignment") << "$$$ CocoaAnalyzer::runCocoa: ";
  }

  // Geometry model built from XML file (corrected with values from DB)
  Model& model = Model::getInstance();
  model.BuildSystemDescriptionFromOA(oaList_);

  if (ALIUtils::debug >= 3) {
    LogDebug("Alignment") << "$$ CocoaAnalyzer::runCocoa: geometry built ";
  }

  // Build measurements
  model.BuildMeasurementsFromOA(measList_);

  if (ALIUtils::debug >= 3) {
    LogDebug("Alignment") << "$$ CocoaAnalyzer::runCocoa: measurements built ";
  }

  // Do fit and store results in DB
  Fit::getInstance();
  Fit::startFit();

  if (ALIUtils::debug >= 0)
    LogDebug("Alignment") << "............ program ended OK";
  if (ALIUtils::report >= 1) {
    ALIFileOut& fileout = ALIFileOut::getInstance(Model::ReportFName());
    fileout << "............ program ended OK";
  }
}

/* Helper: For a given node, get the values associated to a given parameter, from the XMLs SpecPar sections.
 * NB: The same parameter can appear several times WITHIN the same SpecPar section (hence, we have a std::vector).
 * WARNING: This stops at the first relevant SpecPar section encountered.
 * Hence, if A GIVEN NODE HAS SEVERAL SPECPAR XML SECTIONS RE-DEFINING THE SAME PARAMETER,
 * only the first XML SpecPar block will be considered.
 */
template <typename T>
std::vector<T> CocoaAnalyzer::getAllParameterValuesFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
                                                                       const std::string& nodePath,
                                                                       const std::string& parameterName) {
  cms::DDSpecParRefs filteredSpecParSections;
  allSpecParSections.filter(filteredSpecParSections, parameterName);
  for (const auto& mySpecParSection : filteredSpecParSections) {
    if (mySpecParSection->hasPath(nodePath)) {
      return mySpecParSection->value<std::vector<T>>(parameterName);
    }
  }

  return std::vector<T>();
}

/* Helper: For a given node, get the value associated to a given parameter, from the XMLs SpecPar sections.
 * This is the parameterValueIndex-th value (within a XML SpecPar block.) of the desired parameter. 
 */
template <typename T>
T CocoaAnalyzer::getParameterValueFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
                                                      const std::string& nodePath,
                                                      const std::string& parameterName,
                                                      const unsigned int parameterValueIndex) {
  const std::vector<T>& allParameterValues =
    getAllParameterValuesFromSpecParSections<T>(allSpecParSections, nodePath, parameterName);
  if (parameterValueIndex < allParameterValues.size()) {
    return allParameterValues.at(parameterValueIndex);
  }
  return T();
}
