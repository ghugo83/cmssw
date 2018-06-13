import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config

from Geometry.TrackerPhase2TestBeam.Phase2TestBeamGeometryXML_cfi import *    # Telescope DDGeometry
from Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi import *   # KEY POINT: The Tracker geometry numbering builder is called on the Telescope DD geometry.


# Related to the ex-clasSes dedicated to the telescope. In the end, using the tracker geometry classes directly on the telescope DD decsription is favored.
#from Geometry.TrackerPhase2TestBeam.telescopeParameters_cfi import *          # Parameters from DD.
#from Geometry.TrackerPhase2TestBeam.telescopeTopology_cfi import *            # Allow to get the layer, or plane, or whether a sensor is inner or outer, etc.. from a given DetId.
#from Geometry.TrackerPhase2TestBeam.telescopeGeometry_cfi import *            # Full geometry, as used by the Digitizer.




#from SLHCUpgradeSimulations.Geometry.fakeConditions_phase2TkTiltedBase_cff import *  // IMPORTANT!!!! TO DO: Beam test conditions









#from Geometry.HcalCommonData.hcalParameters_cfi      import *
#from Geometry.HcalCommonData.hcalDDDSimConstants_cfi import *
#from Geometry.HGCalCommonData.hgcalV6ParametersInitialization_cfi import *
#from Geometry.HGCalCommonData.hgcalV6NumberingInitialization_cfi import *
