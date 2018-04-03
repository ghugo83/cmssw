import FWCore.ParameterSet.Config as cms

def customise(process):
    process.VtxSmeared.MinZ = -200.0                     # In cm! TO DO: Might need to be updated.
    process.VtxSmeared.MaxZ = -200.0 
    process.VtxSmeared.MinX = -7.5                       # In cm! TO DO: Might need to be updated.
    process.VtxSmeared.MaxX =  7.5
    process.VtxSmeared.MinY = -7.5
    process.VtxSmeared.MaxY =  7.5
    process.VtxSmeared.MinT = cms.double(0.0)
    process.VtxSmeared.MaxT = cms.double(0.0)

    return process
