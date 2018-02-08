#include "TelescopeDigiGeometryESModule.h"


//__________________________________________________________________
TelescopeDigiGeometryESModule::TelescopeDigiGeometryESModule(const edm::ParameterSet & p) 
  : alignmentsLabel_(p.getParameter<std::string>("alignmentsLabel")),
    myLabel_(p.getParameter<std::string>("appendToDataLabel"))
{
    applyAlignment_ = p.getParameter<bool>("applyAlignment");
    fromDDD_ = p.getParameter<bool>("fromDDD");

    setWhatProduced(this);

    edm::LogInfo("Geometry") << "@SUB=TelescopeDigiGeometryESModule"
			     << "Label '" << myLabel_ << "' "
			     << (applyAlignment_ ? "looking for" : "IGNORING")
			     << " alignment labels '" << alignmentsLabel_ << "'.";
}

//__________________________________________________________________
TelescopeDigiGeometryESModule::~TelescopeDigiGeometryESModule() {}

void
TelescopeDigiGeometryESModule::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
{
  edm::ParameterSetDescription descDB;
  descDB.add<std::string>( "appendToDataLabel", "" );
  descDB.add<bool>( "fromDDD", false );
  descDB.add<bool>( "applyAlignment", true );
  descDB.add<std::string>( "alignmentsLabel", "" );
  descriptions.add( "telescopeGeometryDB", descDB );

  edm::ParameterSetDescription desc;
  desc.add<std::string>( "appendToDataLabel", "" );
  desc.add<bool>( "fromDDD", true );
  desc.add<bool>( "applyAlignment", true );
  desc.add<std::string>( "alignmentsLabel", "" );
  descriptions.add( "telescopeGeometry", desc );
}

//__________________________________________________________________
std::shared_ptr<TelescopeGeometry> 
TelescopeDigiGeometryESModule::produce(const TelescopeDigiGeometryRecord & iRecord)
{ 
  //
  // Called whenever the alignments, alignment errors or global positions change
  //
  edm::ESHandle<GeometricDet> gD;
  iRecord.getRecord<IdealGeometryRecord>().get( gD );

  edm::ESHandle<TelescopeTopology> tTopoHand;
  iRecord.getRecord<TelescopeTopologyRcd>().get(tTopoHand);
  const TelescopeTopology *tTopo=tTopoHand.product();

  // Parameters are not really needed here
  /*edm::ESHandle<PTelescopeParameters> ptp;
    iRecord.getRecord<PTelescopeParametersRcd>().get( ptp );*/
  
  TelescopeGeomBuilderFromGeometricDet builder;
  //_telescope  = std::shared_ptr<TelescopeGeometry>(builder.build(&(*gD), *ptp, tTopo));
  _telescope  = std::shared_ptr<TelescopeGeometry>(builder.build(&(*gD), tTopo));


  /* No thanks
  if (applyAlignment_) {
    // Since fake is fully working when checking for 'empty', we should get rid of applyAlignment_!
    edm::ESHandle<Alignments> globalPosition;
    iRecord.getRecord<GlobalPositionRcd>().get(alignmentsLabel_, globalPosition);
    edm::ESHandle<Alignments> alignments;
    iRecord.getRecord<TelescopeAlignmentRcd>().get(alignmentsLabel_, alignments);
    edm::ESHandle<AlignmentErrorsExtended> alignmentErrors;
    iRecord.getRecord<TelescopeAlignmentErrorExtendedRcd>().get(alignmentsLabel_, alignmentErrors);
    // apply if not empty:
    if (alignments->empty() && alignmentErrors->empty() && globalPosition->empty()) {
      edm::LogInfo("Config") << "@SUB=TelescopeDigiGeometryRecord::produce"
			     << "Alignment(Error)s and global position (label '"
	 		     << alignmentsLabel_ << "') empty: Geometry producer (label "
			     << "'" << myLabel_ << "') assumes fake and does not apply.";
    } else {
      GeometryAligner ali;
      ali.applyAlignments<TelescopeGeometry>(&(*_telescope), &(*alignments), &(*alignmentErrors),
                                           align::DetectorGlobalPosition(*globalPosition,
                                                                         DetId(DetId::Telescope)));
    }

    edm::ESHandle<AlignmentSurfaceDeformations> surfaceDeformations;
    iRecord.getRecord<TelescopeSurfaceDeformationRcd>().get(alignmentsLabel_, surfaceDeformations);
    // apply if not empty:
    if (surfaceDeformations->empty()) {
      edm::LogInfo("Config") << "@SUB=TelescopeDigiGeometryRecord::produce"
			     << "AlignmentSurfaceDeformations (label '"
			     << alignmentsLabel_ << "') empty: Geometry producer (label "
			     << "'" << myLabel_ << "') assumes fake and does not apply.";
    } else {
      GeometryAligner ali;
      ali.attachSurfaceDeformations<TelescopeGeometry>(&(*_telescope), &(*surfaceDeformations));
    }
    }
  */
  
  return _telescope;
}

DEFINE_FWK_EVENTSETUP_MODULE(TelescopeDigiGeometryESModule);
