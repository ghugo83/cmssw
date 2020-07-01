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
  theCocoaDaqRootFileName = pset.getParameter<std::string>("cocoaDaqRootFile");
  m_tag = pset.getParameter<edm::ESInputTag>("DDDetector");
  int maxEvents = pset.getParameter<int32_t>("maxEvents");
  GlobalOptionMgr::getInstance()->setDefaultGlobalOptions();
  GlobalOptionMgr::getInstance()->setGlobalOption("maxEvents", maxEvents);
  GlobalOptionMgr::getInstance()->setGlobalOption("writeDBAlign", 1);
  GlobalOptionMgr::getInstance()->setGlobalOption("writeDBOptAlign", 1);
  usesResource("CocoaAnalyzer");
}


void CocoaAnalyzer::beginJob() {}


void CocoaAnalyzer::runCocoa() {
  if (ALIUtils::debug >= 3) {
    std::cout << std::endl << "$$$ CocoaAnalyzer::runCocoa: " << std::endl;
  }

  // Build the Model out of the system description text or XML file
  Model& model = Model::getInstance();
  model.BuildSystemDescriptionFromOA(oaList_);
  if (ALIUtils::debug >= 3) {
    std::cout << "$$ CocoaAnalyzer::runCocoa: geometry built " << std::endl;
  }

  model.BuildMeasurementsFromOA(measList_);

  if (ALIUtils::debug >= 3) {
    std::cout << "$$ CocoaAnalyzer::runCocoa: measurements built " << std::endl;
  }

  Fit::getInstance();

  Fit::startFit();

  if (ALIUtils::debug >= 0)
    std::cout << "............ program ended OK" << std::endl;
  if (ALIUtils::report >= 1) {
    ALIFileOut& fileout = ALIFileOut::getInstance(Model::ReportFName());
    fileout << "............ program ended OK" << std::endl;
  }

}


void CocoaAnalyzer::readXMLFile(const edm::EventSetup& evts) {
  // STEP ONE:  Initial COCOA objects will be built from a DDL geometry
  // description.
  edm::ESTransientHandle<cms::DDCompactView> myCompactView;
  evts.get<IdealGeometryRecord>().get(myCompactView);

  const cms::DDDetector* mySystem = myCompactView->detector();
  if (mySystem) {
    // First oaInfo to be stored is about the world volume.
    const dd4hep::Volume& worldVolume = mySystem->worldVolume();

    if (ALIUtils::debug >= 3) {
      std::cout << std::endl << "$$$ CocoaAnalyzer::ReadXML: world object= " << worldVolume.name() << std::endl;
    }

    OpticalAlignInfo oaInfo;
    oaInfo.ID_ = 0;
    oaInfo.name_ = worldVolume.name();
    oaInfo.parentName_ = "";
    oaInfo.x_.quality_ = 0;
    oaInfo.x_.value_ = 0.;
    oaInfo.x_.error_ = 0.;
    oaInfo.x_.quality_ = 0;
    oaInfo.y_.value_ = 0.;
    oaInfo.y_.error_ = 0.;
    oaInfo.y_.quality_ = 0;
    oaInfo.z_.value_ = 0.;
    oaInfo.z_.error_ = 0.;
    oaInfo.z_.quality_ = 0;
    oaInfo.angx_.value_ = 0.;
    oaInfo.angx_.error_ = 0.;
    oaInfo.angx_.quality_ = 0;
    oaInfo.angy_.value_ = 0.;
    oaInfo.angy_.error_ = 0.;
    oaInfo.angy_.quality_ = 0;
    oaInfo.angz_.value_ = 0.;
    oaInfo.angz_.error_ = 0.;
    oaInfo.angz_.quality_ = 0;

    oaInfo.type_ = "system";

    oaList_.opticalAlignments_.push_back(oaInfo);
    oaInfo.clear();

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
    OpticalAlignParam oaParam;
    OpticalAlignMeasurementInfo oaMeas;

    bool doCOCOA = myFilteredView.firstChild();
    // Loop on all COCOA volumes from filtered view
    while (doCOCOA) {
      ++nObjects;

      // Current volume
      const dd4hep::PlacedVolume& myPlacedVolume = myFilteredView.volume();
      const std::string& name = myPlacedVolume.name();
      const std::string& nodePath = myFilteredView.path();
      oaInfo.name_ = nodePath;

      // Parent name
      oaInfo.parentName_ = nodePath.substr(0, nodePath.rfind('/', nodePath.length()));

      if (ALIUtils::debug >= 4) {
        std::cout << " CocoaAnalyzer::ReadXML reading object " << name << std::endl;
        std::cout << " @@ Name built= " << oaInfo.name_ << " short_name= " << name << " parent= " << oaInfo.parentName_
                  << std::endl;
      }

      // TRANSLATIONS

      // A) GET TRANSLATIONS FROM DDETECTOR.
      // Directly get translation from parent to child volume
      const dd4hep::Position& transl = myPlacedVolume.position();

      if (ALIUtils::debug >= 4) {
        std::cout << "Local translation in cm = " << transl << std::endl;
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
        std::cout << "Local rotation = " << std::endl;
        std::cout << xx << "  " << xy << "  " << xz << std::endl;
        std::cout << yx << "  " << yy << "  " << yz << std::endl;
        std::cout << zx << "  " << zy << "  " << zz << std::endl;
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
        std::cout << "CocoaAnalyzer::ReadXML OBJECT " << oaInfo.name_ << " pos/angles read " << std::endl;
      }

      // Check that rotations match with values from XMLs.
      // Same, that ugly code is not mine ;p
      if (fabs(oaInfo.angx_.value_ - angles[0]) > 1.E-9 || fabs(oaInfo.angy_.value_ - angles[1]) > 1.E-9 ||
          fabs(oaInfo.angz_.value_ - angles[2]) > 1.E-9) {
        std::cerr << " WRONG ANGLE IN OBJECT " << oaInfo.name_ << oaInfo.angx_.value_ << " =? " << angles[0]
                  << oaInfo.angy_.value_ << " =? " << angles[1] << oaInfo.angz_.value_ << " =? " << angles[2]
                  << std::endl;
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
        std::cout << " CocoaAnalyzer::ReadXML:  Fill extra entries with read parameters " << std::endl;
      }

      //--- Fill extra entries with read parameters
      if (names.size() == dims.size() && dims.size() == values.size() && values.size() == errors.size() &&
          errors.size() == quality.size()) {
        for (size_t ind = 0; ind < names.size(); ++ind) {
          double dimFactor = 1.;
          //std::string type = oaParam.dimType();
          const std::string& type = dims.at(ind);
          if (type == "centre" || type == "length") {
            dimFactor = 1. / (1._m);  // was converted to cm with getParameterValueFromSpecPar, COCOA unit is m
          } else if (type == "angles" || type == "angle" || type == "nodim") {
            dimFactor = 1.;
          }
          oaParam.value_ = values[ind] * dimFactor;
          oaParam.error_ = errors[ind] * dimFactor;
          oaParam.quality_ = static_cast<int>(quality[ind]);
          oaParam.name_ = names[ind];
          oaParam.dim_type_ = dims[ind];
          oaInfo.extraEntries_.push_back(oaParam);
          oaParam.clear();
        }

        oaList_.opticalAlignments_.push_back(oaInfo);
      } else {
        std::cout << "WARNING FOR NOW: sizes of extra parameters (names, dimType, value, quality) do"
                  << " not match!  Did not add " << nObjects << " item to OpticalAlignments." << std::endl;
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
        std::cout << " CocoaAnalyzer::ReadXML:  Fill measurements with read parameters " << std::endl;
      }

      //--- Fill measurements with read parameters
      if (measNames.size() == measTypes.size()) {
        for (size_t ind = 0; ind < measNames.size(); ++ind) {
          oaMeas.ID_ = ind;
          oaMeas.name_ = measNames[ind];
          oaMeas.type_ = measTypes[ind];
          oaMeas.measObjectNames_ = measObjectNames[oaMeas.name_];
          if (measParamNames.size() == measParamValues.size() && measParamValues.size() == measParamSigmas.size()) {
            for (size_t ind2 = 0; ind2 < measParamNames[oaMeas.name_].size(); ind2++) {
              oaParam.name_ = measParamNames[oaMeas.name_][ind2];
              oaParam.value_ = measParamValues[oaMeas.name_][ind2];
              oaParam.error_ = measParamSigmas[oaMeas.name_][ind2];
              if (oaMeas.type_ == "SENSOR2D" || oaMeas.type_ == "COPS" || oaMeas.type_ == "DISTANCEMETER" ||
                  oaMeas.type_ == "DISTANCEMETER!DIM" || oaMeas.type_ == "DISTANCEMETER3DIM") {
                oaParam.dim_type_ = "length";
              } else if (oaMeas.type_ == "TILTMETER") {
                oaParam.dim_type_ = "angle";
              } else {
                std::cerr << "CocoaAnalyzer::readXMLFile. Invalid measurement type: " << oaMeas.type_ << std::endl;
                std::exception();
              }

              oaMeas.values_.push_back(oaParam);
              oaMeas.isSimulatedValue_.push_back(measIsSimulatedValue[oaMeas.name_][ind2]);
              if (ALIUtils::debug >= 5) {
                std::cout << oaMeas.name_ << " copying issimu "
                          << oaMeas.isSimulatedValue_[oaMeas.isSimulatedValue_.size() - 1] << " = "
                          << measIsSimulatedValue[oaMeas.name_][ind2] << std::endl;
                //-           std::cout << ind2 << " adding meas value " << oaParam << std::endl;
              }
              oaParam.clear();
            }
          } else {
            if (ALIUtils::debug >= 2) {
              std::cout << "WARNING FOR NOW: sizes of measurement parameters (name, value, sigma) do"
                        << " not match! for measurement " << oaMeas.name_
                        << " !Did not fill parameters for this measurement " << std::endl;
            }
          }
          measList_.oaMeasurements_.push_back(oaMeas);
          if (ALIUtils::debug >= 5) {
            std::cout << "CocoaAnalyser: MEASUREMENT " << oaMeas.name_ << " extra entries read " << oaMeas << std::endl;
          }
          oaMeas.clear();
        }

      } else {
        if (ALIUtils::debug >= 2) {
          std::cout << "WARNING FOR NOW: sizes of measurements (names, types do"
                    << " not match!  Did not add " << nObjects << " item to XXXMeasurements" << std::endl;
        }
      }

      oaInfo.clear();
      doCOCOA = myFilteredView.firstChild();
    }  // while (doCOCOA)

    if (ALIUtils::debug >= 3) {
      std::cout << "CocoaAnalyzer::ReadXML: Finished building " << nObjects + 1 << " OpticalAlignInfo objects"
                << " and " << measList_.oaMeasurements_.size() << " OpticalAlignMeasurementInfo objects " << std::endl;
    }
    if (ALIUtils::debug >= 5) {
      std::cout << " @@@@@@ OpticalAlignments " << oaList_ << std::endl;
      std::cout << " @@@@@@ OpticalMeasurements " << measList_ << std::endl;
    }
  }
}

//------------------------------------------------------------------------
std::vector<OpticalAlignInfo> CocoaAnalyzer::readCalibrationDB(const edm::EventSetup& evts) {
  if (ALIUtils::debug >= 3) {
    std::cout << std::endl << "$$$ CocoaAnalyzer::readCalibrationDB: " << std::endl;
  }

  using namespace edm::eventsetup;
  edm::ESHandle<OpticalAlignments> pObjs;
  evts.get<OpticalAlignmentsRcd>().get(pObjs);
  const OpticalAlignments* dbObj = pObjs.product();

  if (ALIUtils::debug >= 5) {
    std::vector<OpticalAlignInfo>::const_iterator it;
    for (it = dbObj->opticalAlignments_.begin(); it != dbObj->opticalAlignments_.end(); ++it) {
      std::cout << "CocoaAnalyzer::readCalibrationDB:  OpticalAlignInfo READ " << *it << std::endl;
    }
  }

  if (ALIUtils::debug >= 4) {
    std::cout << "CocoaAnalyzer::readCalibrationDB:  Number of OpticalAlignInfo READ "
              << dbObj->opticalAlignments_.size() << std::endl;
  }

  return dbObj->opticalAlignments_;
}

//------------------------------------------------------------------------
void CocoaAnalyzer::correctOptAlignments(std::vector<OpticalAlignInfo>& oaListCalib) {
  if (ALIUtils::debug >= 3) {
    std::cout << std::endl << "$$$ CocoaAnalyzer::correctOptAlignments: " << std::endl;
  }

  std::vector<OpticalAlignInfo>::const_iterator it;
  for (it = oaListCalib.begin(); it != oaListCalib.end(); ++it) {
    OpticalAlignInfo oaInfoDB = *it;
    OpticalAlignInfo* oaInfoXML = findOpticalAlignInfoXML(oaInfoDB);
    std::cerr << "error " << (*it).name_ << std::endl;
    if (oaInfoXML == nullptr) {
      if (ALIUtils::debug >= 2) {
        std::cerr << "@@@@@ WARNING CocoaAnalyzer::correctOptAlignments:  OpticalAlignInfo read from DB is not present "
                     "in XML "
                  << *it << std::endl;
      }
    } else {
      //------ Correct info
      if (ALIUtils::debug >= 5) {
        std::cout << "CocoaAnalyzer::correctOptAlignments: correcting data from DB info " << std::endl;
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
                << *itoap1 << " in object " << *it << std::endl;
          }
        }
      }
      if (ALIUtils::debug >= 5) {
        std::cout << "CocoaAnalyzer::correctOptAlignments: corrected OpticalAlingInfo " << oaList_ << std::endl;
      }
    }
  }
}

//------------------------------------------------------------------------
OpticalAlignInfo* CocoaAnalyzer::findOpticalAlignInfoXML(const OpticalAlignInfo& oaInfo) {
  OpticalAlignInfo* oaInfoXML = nullptr;
  std::vector<OpticalAlignInfo>::iterator it;
  for (it = oaList_.opticalAlignments_.begin(); it != oaList_.opticalAlignments_.end(); ++it) {
    std::string oaName = oaInfo.name_;

    if (ALIUtils::debug >= 5) {
      std::cout << "CocoaAnalyzer::findOpticalAlignInfoXML:  looking for OAI " << (*it).name_ << " =? " << oaName
                << std::endl;
    }
    if ((*it).name_ == oaName) {
      oaInfoXML = &(*it);
      if (ALIUtils::debug >= 4) {
        std::cout << "CocoaAnalyzer::findOpticalAlignInfoXML:  OAI found " << oaInfoXML->name_ << std::endl;
      }
      break;
    }
  }

  return oaInfoXML;
}

//------------------------------------------------------------------------
bool CocoaAnalyzer::correctOaParam(OpticalAlignParam* oaParamXML, const OpticalAlignParam& oaParamDB) {
  if (ALIUtils::debug >= 4) {
    std::cout << "CocoaAnalyzer::correctOaParam  old value= " << oaParamXML->value_
              << " new value= " << oaParamDB.value_ << std::endl;
  }
  if (oaParamDB.value_ == -9.999E9)
    return false;

  double dimFactor = 1.;
  //loop for an Entry with equal type to entries to know which is the
  std::string type = oaParamDB.dimType();
  if (type == "centre" || type == "length") {
    dimFactor = 0.01;  // in DB it is in cm
  } else if (type == "angles" || type == "angle" || type == "nodim") {
    dimFactor = 1.;
  } else {
    std::cerr << "!!! COCOA programming error: inform responsible: incorrect OpticalAlignParam type = " << type
              << std::endl;
    std::exception();
  }

  oaParamXML->value_ = oaParamDB.value_ * dimFactor;

  return true;
}

//-#include "Alignment/CocoaUtilities/interface/GlobalOptionMgr.h"
//-#include "Alignment/CocoaUtilities/interface/ALIFileIn.h"

//-----------------------------------------------------------------------
void CocoaAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& evts) {
  ALIUtils::setDebugVerbosity(5);

  readXMLFile(evts);

  // Write DB
  Model& model = Model::getInstance();
  model.BuildSystemDescriptionFromOA( oaList_ );
  CocoaDBMgr::getInstance()->DumpCocoaResults();

  //std::vector<OpticalAlignInfo> oaListCalib = readCalibrationDB(evts);
  //correctOptAlignments(oaListCalib);

  

  //runCocoa();





  //new CocoaDaqReaderRoot( theCocoaDaqRootFileName );
  /*-
  int nEvents = daqReader->GetNEvents();
  for( int ii = 0; ii < nEvents; ii++) {
    if( ! daqReader->ReadEvent( ii ) ) break;
  }
  */

  //  std::cout << "!!!! NOT  DumpCocoaResults() " << std::endl;

  //  CocoaDBMgr::getInstance()->DumpCocoaResults();

  return;

  //  using namespace edm::eventsetup;
  //  std::cout <<" I AM IN RUN NUMBER "<<evt.id().run() <<std::endl;
  //  std::cout <<" ---EVENT NUMBER "<<evt.id().event() <<std::endl;

  // just a quick dump of the private OpticalAlignments object
  //  std::cout << oaList_ << std::endl;

  // STEP 2:
  // Get calibrated OpticalAlignments.  In this case I'm using
  // some sqlite database examples that are generated using
  // testOptAlignWriter.cc
  // from CondFormats/OptAlignObjects/test/

  //   edm::ESHandle<OpticalAlignments> oaESHandle;
  //   context.get<OpticalAlignmentsRcd>().get(oaESHandle);

  //   // This assumes they ALL come in together.  This may not be
  //   // the "real" case.  One could envision different objects coming
  //   // in and we would get by label each one (type).

  //   std::cout << "========== eventSetup data changes with IOV =========" << std::endl;
  //   std::cout << *oaESHandle << std::endl;
  //   //============== COCOA WORK!
  //   //  calibrated values should be used to "correct" the ones read in during beginJob
  //   //==============

  //   //
  //   // to see how to iterate over the OpticalAlignments, please
  //   // refer to the << operator of OpticalAlignments, OpticalAlignInfo
  //   // and OpticalAlignParam.
  //   //     const OpticalAlignments* myoa=oa.product();

  //   // STEP 3:
  //   // This retrieves the Measurements
  //   // for each event, a new set of measurements is available.
  //  edm::Handle<OpticalAlignMeasurements> measHandle;
  //  evt.getByLabel("OptAlignGeneratedSource", measHandle);

  //  std::cout << "========== event data product changes with every event =========" << std::endl;
  //  std::cout << *measHandle << std::endl;

  //============== COCOA WORK!
  //  Each set of optical alignment measurements can be used
  //  in whatever type of analysis COCOA does.
  //==============

}  //end of ::analyze()

// STEP 4:  one could use ::endJob() to write out the OpticalAlignments
// generated by the analysis. Example code of writing is in
// CondFormats/Alignment/test/testOptAlignWriter.cc

//-----------------------------------------------------------------------
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
