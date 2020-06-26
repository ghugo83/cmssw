import FWCore.ParameterSet.Config as cms

#XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
#    geomXMLFiles = cms.vstring(
#        'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml',
#        'Geometry/CMSCommonData/data/FieldParameters.xml'),
#    rootNodeName = cms.string('cms:OCMS')
#)

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring(
        'Geometry/CMSCommonData/data/materials.xml',
        'Alignment/CocoaApplication/test/table2DWithMirror.xml',
        'Geometry/CMSCommonData/data/cms.xml'),
    rootNodeName = cms.string('cms:OCMS')
    #rootNodeName = cms.string('table2DWithMirror:MotherOfAllBoxes_1')
)

