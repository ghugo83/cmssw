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


class CocoaAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>
{
 public:
  
  explicit  CocoaAnalyzer(edm::ParameterSet const& p);
  explicit  CocoaAnalyzer(int i) { }
  virtual ~ CocoaAnalyzer() { }
  
  virtual void beginJob() override;
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  // see note on endJob() at the bottom of the file.
  // virtual void endJob() ;

 private:
  void ReadXMLFile( const edm::EventSetup& evts );
  std::vector<OpticalAlignInfo> ReadCalibrationDB( const edm::EventSetup& evts );

  void CorrectOptAlignments( std::vector<OpticalAlignInfo>& oaListCalib );
  OpticalAlignInfo* FindOpticalAlignInfoXML( const OpticalAlignInfo& oaInfo );
  bool CorrectOaParam( OpticalAlignParam* oaParamXML, const OpticalAlignParam& oaParamDB );

  void RunCocoa();

  OpticalAlignInfo GetOptAlignInfoFromOptO( OpticalObject* opto );

  std::vector<double> getAllParameterValuesFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
							       const std::string& nodePath,
							       const std::string& parameterName
							       );
  double getParameterValueFromSpecParSections(const cms::DDSpecParRegistry& allSpecParSections,
					      const std::string& nodePath,
					      const std::string& parameterName,
					      const unsigned int parameterValueIndex
					      );

  std::vector<std::string> getAllParameterValuesFromSpecParSectionsString(const cms::DDSpecParRegistry& allSpecParSections,
									  const std::string& nodePath,
									  const std::string& parameterName
									  );
  std::string getParameterValueFromSpecParSectionsString(const cms::DDSpecParRegistry& allSpecParSections,
							 const std::string& nodePath,
							 const std::string& parameterName,
							 const unsigned int parameterValueIndex
							 );

 private:
  OpticalAlignments oaList_;
  OpticalAlignMeasurements measList_;
  std::string theCocoaDaqRootFileName;
  edm::ESInputTag m_tag;
};

#endif
