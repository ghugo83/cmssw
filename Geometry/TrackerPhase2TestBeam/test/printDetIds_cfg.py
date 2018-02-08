import FWCore.ParameterSet.Config as cms

process = cms.Process("printDetIdsTest")
process.load("Configuration.StandardSequences.Services_cff")

process.load('Configuration.Geometry.GeometryTrackerPhase2TestBeam_cff')


#  Alignment
#process.trackerSLHCGeometry.applyAlignment = cms.bool(False) 
#from Geometry.TrackerGeometryBuilder.idealForDigiTrackerSLHCGeometry_cff import *
#process.trackerGeometry.applyAlignment = cms.bool(False)


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.prod = cms.EDAnalyzer("DetIdsAnalyzer");

process.p1 = cms.Path(process.prod)
