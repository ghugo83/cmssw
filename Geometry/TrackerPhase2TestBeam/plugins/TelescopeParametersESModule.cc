#include "Geometry/TrackerPhase2TestBeam/plugins/TelescopeParametersESModule.h"


TelescopeParametersESModule::TelescopeParametersESModule( const edm::ParameterSet& )
{
  edm::LogInfo("TELESCOPE") << "TelescopeParametersESModule::TelescopeParametersESModule";

  setWhatProduced(this);
}

TelescopeParametersESModule::~TelescopeParametersESModule()
{ 
}

void
TelescopeParametersESModule::fillDescriptions( edm::ConfigurationDescriptions & descriptions ) 
{
  edm::ParameterSetDescription desc;
  descriptions.add( "telescopeParameters", desc );
}

TelescopeParametersESModule::ReturnType
TelescopeParametersESModule::produce( const PTelescopeParametersRcd& iRecord )
{
  edm::LogInfo("TelescopeParametersESModule") <<  "TelescopeParametersESModule::produce(const PTelescopeParametersRcd& iRecord)" << std::endl;
  edm::ESTransientHandle<DDCompactView> cpv;
  iRecord.getRecord<IdealGeometryRecord>().get( cpv );
    
  PTelescopeParameters* ptp = new PTelescopeParameters();
  TelescopeParametersFromDD builder;
  builder.build( &(*cpv), *ptp );

  return ReturnType( ptp ) ;
}

DEFINE_FWK_EVENTSETUP_MODULE( TelescopeParametersESModule);
