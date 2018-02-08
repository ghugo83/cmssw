import FWCore.ParameterSet.Config as cms

process = cms.Process("NumberingTest")

#process.load("Configuration.Geometry.GeometryReco_cff")
#process.load("Geometry.CMSCommonData.cmsExtendedGeometryXML_cfi")
process.load("Configuration.Geometry.GeometryTrackerPhase2TestBeam_cff")

process.telescopeGeometry.applyAlignment = cms.bool(False)

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.prod = cms.EDAnalyzer("TelescopeTopologyAnalyzer");

process.p1 = cms.Path(process.prod)
