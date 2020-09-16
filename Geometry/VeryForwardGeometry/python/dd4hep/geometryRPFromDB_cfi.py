import FWCore.ParameterSet.Config as cms

DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Geometry/VeryForwardGeometry/data/dd4hep/empty.xml'),
                                            label = cms.string('CTPPS'),
                                            fromDB = cms.bool(True),
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)


DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)                                


ctppsGeometryESModule = cms.ESProducer("CTPPSGeometryESModule",
    fromDD4hep = cms.untracked.bool(True),
    verbosity = cms.untracked.uint32(1),
    compactViewTag = cms.string('XMLIdealGeometryESSource_CTPPS')
)

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
ctpps_2016.toModify(ctppsGeometryESModule, isRun2=cms.untracked.bool(True))

from Configuration.Eras.Modifier_ctpps_2017_cff import ctpps_2017
ctpps_2017.toModify(ctppsGeometryESModule, isRun2=cms.untracked.bool(True))

from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
ctpps_2018.toModify(ctppsGeometryESModule, isRun2=cms.untracked.bool(True))

