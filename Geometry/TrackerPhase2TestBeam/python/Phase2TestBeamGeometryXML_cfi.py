import FWCore.ParameterSet.Config as cms

# NB: This was hand-made.
# TO DO: update Configuration/Geometry/python/dict2023Geometry.py and create workflow?

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring(
         # World volume creation and default CMS materials
        'Geometry/CMSCommonData/data/materials.xml',
        'Geometry/CMSCommonData/data/rotations.xml',
        'Geometry/TrackerPhase2TestBeam/data/Common/cms.xml',
        
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
        'Geometry/TrackerCommonData/data/PhaseII/trackerParameters.xml'
      
    ),
    rootNodeName = cms.string('cms:OCMS')
)
