import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config

from Geometry.TrackerPhase2TestBeam.Phase2TestBeamGeometryXML_cfi import *    # Telescope DDGeometry
from Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi import *   # KEY POINT: The Tracker geometry numbering builder is called on the Telescope DD geometry.

from SLHCUpgradeSimulations.Geometry.fakeConditions_phase2TkTiltedBase_cff import *  # IMPORTANT!!!! TO DO: Beam test conditions



#from Geometry.HcalCommonData.hcalParameters_cfi      import *
#from Geometry.HcalCommonData.hcalDDDSimConstants_cfi import *
#from Geometry.HGCalCommonData.hgcalV6ParametersInitialization_cfi import *
#from Geometry.HGCalCommonData.hgcalV6NumberingInitialization_cfi import *
