import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config

from Geometry.TrackerPhase2TestBeam.Phase2TestBeamGeometryXML_cfi import *    # DDGeometry

from Geometry.TrackerPhase2TestBeam.telescopeGeometryNumbering_cfi import *   # Sorting and Numbering of DDGeometry. Produced by DDTelescopeGeometryESProducer.

from Geometry.TrackerPhase2TestBeam.telescopeParameters_cfi import *          # Parameters from DD.

from Geometry.TrackerPhase2TestBeam.telescopeTopology_cfi import *            # Allow to get the layer, or plane, or whether a sensor is inner or outer, etc.. from a given DetId.

from Geometry.TrackerPhase2TestBeam.telescopeGeometry_cfi import *            # Full geometry, as used by the Digitizer.


#from SLHCUpgradeSimulations.Geometry.fakeConditions_phase2TkTilted4025_cff import *
#from Geometry.HcalCommonData.hcalParameters_cfi      import *
#from Geometry.HcalCommonData.hcalDDDSimConstants_cfi import *
#from Geometry.HGCalCommonData.hgcalV6ParametersInitialization_cfi import *
#from Geometry.HGCalCommonData.hgcalV6NumberingInitialization_cfi import *
