#ifndef CocoaAnalyser_HH
#define CocoaAnalyser_HH
//-*- C++ -*-
//
// Package:    Alignment/CocoaApplication
// Class:      CocoaAnalyzer
//
/*

 Description: test access to the OpticalAlignMeasurements via OpticalAlignMeasurementsGeneratedSource
    This also should demonstrate access to a geometry via the XMLIdealGeometryESSource
    for use in THE COCOA analyzer.

 Implementation:
     Iterate over retrieved alignments.
*/
//

#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "DetectorDescription/DDCMS/interface/DDSpecParRegistry.h"

class Event;
class EventSetup;
class Entry;

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignInfo.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurements.h"

class DDFilteredView;
class DDCompactView;
class DDSpecifics;
class OpticalObject;

class CocoaAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit CocoaAnalyzer(edm::ParameterSet const& p);
  explicit CocoaAnalyzer(int i) {}
  ~CocoaAnalyzer() override {}

  void beginJob() override;
  void analyze(const edm::Event& e, const edm::EventSetup& c) override;

private:
  void readXMLFile(const edm::EventSetup& evts);
  std::vector<OpticalAlignInfo> readCalibrationDB(const edm::EventSetup& evts);

  void correctOptAlignments(std::vector<OpticalAlignInfo>& oaListCalib);
  OpticalAlignInfo* findOpticalAlignInfoXML(const OpticalAlignInfo& oaInfo);
  bool correctOaParam(OpticalAlignParam* oaParamXML, const OpticalAlignParam& oaParamDB);

  void runCocoa();

  template <typename T>
  std::vector<T> getAllParameterValuesFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
                                                          const std::string& nodePath,
                                                          const std::string& parameterName);
  template <typename T>
  T getParameterValueFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
                                         const std::string& nodePath,
                                         const std::string& parameterName,
                                         const unsigned int parameterValueIndex);

private:
  OpticalAlignments oaList_;
  OpticalAlignMeasurements measList_;
  std::string theCocoaDaqRootFileName;
  edm::ESInputTag m_tag;
};

#endif
