#ifndef CocoaAnalyser_HH
#define CocoaAnalyser_HH

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "DetectorDescription/DDCMS/interface/DDSpecParRegistry.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurements.h"


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
};

#endif
