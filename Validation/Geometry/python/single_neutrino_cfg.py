import FWCore.ParameterSet.Config as cms

process = cms.Process("TestProcess")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.load("IOMC.EventVertexGenerators.VtxSmearedFlat_cfi")
from Configuration.StandardSequences.VtxSmeared import VtxSmeared
#process.load(VtxSmeared['Flat'])
process.load(VtxSmeared['NoSmear'])

process.load("GeneratorInterface.Core.generatorSmeared_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
    #input = cms.untracked.int32(50000)
)

process.load("IOMC.RandomEngine.IOMC_cff")
process.RandomNumberGeneratorService.generator.initialSeed = 456789
process.RandomNumberGeneratorService.VtxSmeared.engineName = cms.untracked.string('HepJamesRandom')
process.RandomNumberGeneratorService.VtxSmeared.initialSeed = cms.untracked.uint32(98765432)

process.source = cms.Source("EmptySource",
    firstRun        = cms.untracked.uint32(1),
    firstEvent      = cms.untracked.uint32(1)
)

process.generator = cms.EDProducer("FlatRandomEGunProducer",
    PGunParameters = cms.PSet(
        PartID = cms.vint32(14),
        MinEta = cms.double(0.0),
        MaxEta = cms.double(4.0),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        MinE   = cms.double(10.0),
        MaxE   = cms.double(10.0)
    ),
    AddAntiParticle = cms.bool(False),
    Verbosity       = cms.untracked.int32(0)
)

process.o1 = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('single_neutrino_random.root')
)

process.p1 = cms.Path(process.generator*process.VtxSmeared*process.generatorSmeared)
process.outpath = cms.EndPath(process.o1)

