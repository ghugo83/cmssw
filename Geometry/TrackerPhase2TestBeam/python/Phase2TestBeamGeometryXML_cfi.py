import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config 

# TO DO: update generate2023Geometry.py, or actually, create a different script.

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring(
         # World volume creation and default CMS materials
        'Geometry/CMSCommonData/data/materials.xml',
        'Geometry/CMSCommonData/data/rotations.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/cmsextent.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/cms.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/cmsMother.xml',
        
         # Define standalone Phase 1 BPIX module and associated materials
        'Geometry/TrackerCommonData/data/PhaseI/pixbarmaterial.xml',
        'Geometry/TrackerCommonData/data/Run2/trackermaterial.xml',   #sometimes included via Run2, sometimes via PhaseI, which one to choose?
        'Geometry/TrackerCommonData/data/PhaseI/pixfwdMaterials.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/Phase1BPIXLayer4Module.xml',
        
         # Define DUT and telescope
        'Geometry/TrackerPhase2TestBeam/data/Common/DUT.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/telescope.xml',
        
         # Tunable geometry constants
        'Geometry/TrackerPhase2TestBeam/data/Common/Phase2BeamTestConstants.xml',
        
        # Sim files
        'Geometry/TrackerPhase2TestBeam/data/Sim/pixelTelescopeTopology.xml',
        'Geometry/TrackerPhase2TestBeam/data/Sim/pixelTelescopeSensitive.xml',
        'Geometry/TrackerPhase2TestBeam/data/Sim/pixelTelescopeProdCuts.xml',
        
        # Reco file
        'Geometry/TrackerPhase2TestBeam/data/Reco/pixelTelescopeRecoMaterial.xml',
        
        # DetId Scheme file
        #'Geometry/TrackerPhase2TestBeam/data/Common/telescopeDetIdScheme.xml'
        'Geometry/TrackerCommonData/data/PhaseII/trackerParameters.xml'
      
    ),
    rootNodeName = cms.string('cms:OCMS')
)
