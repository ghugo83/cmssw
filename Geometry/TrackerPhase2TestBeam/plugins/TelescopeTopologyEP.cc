#include "Geometry/TrackerPhase2TestBeam/plugins/TelescopeTopologyEP.h"


TelescopeTopologyEP::TelescopeTopologyEP( const edm::ParameterSet& conf ) {
  edm::LogInfo("TELESCOPE") << "TelescopeTopologyEP::TelescopeTopologyEP";

  setWhatProduced(this);
}


TelescopeTopologyEP::~TelescopeTopologyEP() { }


void TelescopeTopologyEP::fillDescriptions( edm::ConfigurationDescriptions & descriptions )  {
  edm::ParameterSetDescription ttc;
  descriptions.add( "telescopeTopology", ttc );
}


TelescopeTopologyEP::ReturnType TelescopeTopologyEP::produce( const TelescopeTopologyRcd& iRecord ) {
  edm::LogInfo("TelescopeTopologyEP") <<  "TelescopeTopologyEP::produce(const TelescopeTopologyRcd& iRecord)";

  edm::ESHandle<PTelescopeParameters> ptp;
  iRecord.getRecord<PTelescopeParametersRcd>().get( ptp );

  
  //fillScheme(dbl_to_int( DDVectorGetter::get( "telescopeDetIdShifts" )));
  /*std::vector<int> detidShifts;
    detidShifts.push_back(28);   // TO DO: remove harcoded value
    detidShifts.push_back(25);
    detidShifts.push_back(4);
    detidShifts.push_back(2);
    detidShifts.push_back(0);
    fillScheme(detidShifts);
  */

  fillScheme( *ptp );
 
  
  ReturnType myTopo( new TelescopeTopology( telescopeScheme_ ));

  return myTopo ;
}


void TelescopeTopologyEP::fillScheme( const PTelescopeParameters& ptp ) {
  const std::vector<int>& detIdShifts = ptp.detIdShifts;
  const std::vector<int>& detIdMasks = ptp.detIdMasks;


  if ( detIdShifts.size() == numTelescopeHierarchyLevels_
       && detIdMasks.size() == numTelescopeHierarchyLevels_
       ) {
    telescopeScheme_.telescopeStartBit_ = detIdShifts.at(0);
    telescopeScheme_.telescopeMask_ = detIdMasks.at(0);

    telescopeScheme_.containerStartBit_ = detIdShifts.at(1);
    telescopeScheme_.containerMask_ = detIdMasks.at(1);

    telescopeScheme_.planeStartBit_ = detIdShifts.at(2);
    telescopeScheme_.planeMask_ = detIdMasks.at(2);

    telescopeScheme_.moduleStartBit_ = detIdShifts.at(3);
    telescopeScheme_.moduleMask_ = detIdMasks.at(3);

    telescopeScheme_.sensorStartBit_ = detIdShifts.at(4);
    telescopeScheme_.sensorMask_ = detIdMasks.at(4);
  }

  else { 
    std::cout << " TelescopeTopologyEP::fillScheme. "
	      << "Expected number of hirerarchy levels is " << numTelescopeHierarchyLevels_ 
	      << "but detIdShifts.size() = " << detIdShifts.size() 
	      << "and detIdMasks.size() = " << detIdMasks.size() 
	      << std::endl;
  }
}

DEFINE_FWK_EVENTSETUP_MODULE( TelescopeTopologyEP);

