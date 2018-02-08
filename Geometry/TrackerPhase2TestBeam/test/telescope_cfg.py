import FWCore.ParameterSet.Config as cms

process = cms.Process("GeometryTest")
# empty input service, fire 10 events
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# Choose Telescope Geometry
process.load("Configuration.Geometry.GeometryTrackerPhase2TestBeam_cff") # used to be reco _cff

process.telescopeGeometry.applyAlignment = cms.bool(False)

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.prod = cms.EDAnalyzer("TelescopeDigiGeometryAnalyzer")

process.p1 = cms.Path(process.prod)


