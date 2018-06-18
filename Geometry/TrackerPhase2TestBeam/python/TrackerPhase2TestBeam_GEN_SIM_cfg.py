# NB: Single Pion, 120 GeV. Straight beam.
# It's either Pi+ or Pi-. But we could also have e- of mu- or even protons (400 GeV). 
# 120 GeV is a good staring point but this may vary.
# The list of possible observed processes is described at SimG4Core/Physics/src/G4ProcessTypeEnumerator.cc

# This does not pretend to be realistic for the moment, but is there to check that SimHits can be produced on the telescope geometry.
# Conditions will need to be tuned obviously.



import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('SIM')
#process = cms.Process('SIM',eras.Phase2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')

process.load('SimGeneral.MixingModule.mixNoPU_cfi')

process.load('Configuration.Geometry.GeometryTrackerPhase2TestBeamReco_cff') # Load DDGeometry + telescopeGeometryNumbering_cfi + telescopeParameters_cfi + telescopeTopology_cfi + telescopeGeometry_cfi


#process.load('Configuration.StandardSequences.MagneticField_cff')  # TO DO: Tune here. But according to Nikkie: Around the telescope there is no magnetic field. 
process.load('Configuration.StandardSequences.MagneticField_0T_cff')

process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedFlat_cfi')  #process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#process.load('SimG4CMS.HGCalTestBeam.HGCalTBCheckGunPosition_cfi')
#process.load('SimG4CMS.HGCalTestBeam.HGCalTBAnalyzer_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('SinglePionE120GeV_cfi nevts:50000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",       # process.FEVTDEBUGoutput
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('TrackerPhase2TestBeam_GEN_SIM.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,   # process.FEVTDEBUGEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string('TBGenSim.root')
#                                  )

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
process.g4SimHits.UseMagneticField = cms.bool(False)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')  # TO DO: Conditions will obviously need to be tuned.
# 'auto:phase2_realistic'

process.generator = cms.EDProducer("FlatRandomEThetaGunProducer",
    AddAntiParticle = cms.bool(False),
    PGunParameters = cms.PSet(
        MinE = cms.double(119.99),
        MaxE = cms.double(120.01),                   # In GeV!
        MinTheta = cms.double(0.0),
        MaxTheta = cms.double(0.0),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        PartID = cms.vint32(211)                     # Pion Id
    ),
    Verbosity = cms.untracked.int32(0),
    firstRun = cms.untracked.uint32(1),
    psethack = cms.string('Single Pion E 120 GeV')
)
process.VtxSmeared.MinZ = -200.0                     # In cm! TO DO: Might need to be updated.
process.VtxSmeared.MaxZ = -200.0 
process.VtxSmeared.MinX = -7.5                       # In cm! TO DO: Might need to be updated.
process.VtxSmeared.MaxX =  7.5
process.VtxSmeared.MinY = -7.5
process.VtxSmeared.MaxY =  7.5


#process.HGCalTBAnalyzer.DoDigis = False
#process.HGCalTBAnalyzer.DoRecHits = False

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
#process.gunfilter_step  = cms.Path(process.HGCalTBCheckGunPostion)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
#process.analysis_step = cms.Path(process.HGCalTBAnalyzer)       # Can add an Analyzer directly here if desired.
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,
				process.genfiltersummary_step,
				process.simulation_step,
				#process.gunfilter_step,
				#process.analysis_step,
				process.endjob_step,
				process.RAWSIMoutput_step,
				)

#from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
#associatePatAlgosToolsTask(process)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 



# Add early deletion of temporary data products to reduce peak memory need
#from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
#process = customiseEarlyDelete(process)
# End adding early deletion
