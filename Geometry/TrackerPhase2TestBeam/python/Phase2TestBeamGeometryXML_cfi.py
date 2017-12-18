import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2023Geometry.py
# If you notice a mistake, please update the generating script, not just this config

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring(
         # World volume creation and default CMS materials
        'Geometry/CMSCommonData/data/materials.xml',
        'Geometry/CMSCommonData/data/rotations.xml',
        'Geometry/TrackerPhase2TestBeam/data/cmsextent.xml',
        'Geometry/TrackerPhase2TestBeam/data/cms.xml',
        'Geometry/TrackerPhase2TestBeam/data/cmsMother.xml',
        
         # Define standalone Phase 1 BPIX module and associated materials
        'Geometry/TrackerCommonData/data/PhaseI/pixbarmaterial.xml',
        'Geometry/TrackerCommonData/data/Run2/trackermaterial.xml',   #sometimes included via Run2, sometimes via PhaseI, which one to choose?
        'Geometry/TrackerCommonData/data/PhaseI/pixfwdMaterials.xml',
        'Geometry/TrackerPhase2TestBeam/data/Phase1BPIXLayer4Module.xml',
        
         # Define DUT and telescope
        'Geometry/TrackerPhase2TestBeam/data/DUT.xml',
        'Geometry/TrackerPhase2TestBeam/data/telescope.xml',
        
         # Configurable parameters
        'Geometry/TrackerPhase2TestBeam/data/Phase2BeamTestConstants.xml'
      
    ),
    rootNodeName = cms.string('cms:OCMS')
)
