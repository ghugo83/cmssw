#ifndef Geometry_TrackerNumberingBuilder_DetIdsAnalyzer_h
#define Geometry_TrackerNumberingBuilder_DetIdsAnalyzer_h
// -*- C++ -*-
//
// Package:    DetIdsAnalyzer
// Class:      DetIdsAnalyzer
// 


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include <string>
#include <iostream>
#include <fstream>

class DetIdsAnalyzer : public edm::EDAnalyzer {

public:

  explicit DetIdsAnalyzer(const edm::ParameterSet&);
  ~DetIdsAnalyzer() override;

private:

  void beginJob() override;
  void beginRun(const edm::Run &, const edm::EventSetup &) override;
  void analyze(const edm::Event &, const edm::EventSetup &) override;

};
#endif
