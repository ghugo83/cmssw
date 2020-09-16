import FWCore.ParameterSet.Config as cms

DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Geometry/VeryForwardGeometry/data/dd4hep/geometryRPFromDD_2021.xml'),
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

ctppsGeometryESModule = cms.ESProducer("CTPPSGeometryESModule",
    isRun2 = cms.untracked.bool(False),
    fromDD4hep = cms.untracked.bool(True),
    verbosity = cms.untracked.uint32(1),
    compactViewTag = cms.string('XMLIdealGeometryESSource_CTPPS')
)

