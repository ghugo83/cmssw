#ifndef Geometry_TrackerPhase2TestBeam_PTelescopeParameters_h
#define Geometry_TrackerPhase2TestBeam_PTelescopeParameters_h

//#include "CondFormats/Serialization/interface/Serializable.h"



#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDVectorGetter.h"
#include "DetectorDescription/Core/interface/DDutils.h"
#include <sstream>
#include <vector>


class PTelescopeParameters {
 public:
  PTelescopeParameters( void ) {};
  ~PTelescopeParameters( void ) {};

  std::vector<int> detIdShifts;
  std::vector<int> detIdMasks;

};

#endif
