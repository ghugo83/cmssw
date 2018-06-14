import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config

from Configuration.Geometry.GeometryTrackerPhase2TestBeam_cff import *         # DDGeometry


from Geometry.CommonDetUnit.globalTrackingGeometry_cfi import *               # IMPORTANT: ACTIVE THIS OR EQUIVALENT ONCE A CODE IS READY
from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *
from Geometry.TrackerGeometryBuilder.trackerParameters_cfi import *
from Geometry.TrackerNumberingBuilder.trackerTopology_cfi import *
from Geometry.TrackerGeometryBuilder.idealForDigiTrackerGeometry_cff import *
trackerGeometry.applyAlignment = cms.bool(False)


