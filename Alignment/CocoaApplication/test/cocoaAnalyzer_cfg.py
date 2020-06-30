import FWCore.ParameterSet.Config as cms

process = cms.Process("TestCocoa")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("EmptySource")   
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Alignment/CocoaApplication/test/cmsCocoaTable2DWithMirror.xml'),
                                            appendToDataLabel = cms.string('')
                                            )
                                            
process.DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                                 appendToDataLabel = cms.string('')
                                                )
                                                   
                         
#process.load("CondCore.CondDB.CondDB_cfi")
#process.CondDB.connect = 'sqlite_file:OpticalAlignments.db'
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    process.CondDB,
#    DumpStat=cms.untracked.bool(True),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('OpticalAlignmentsRcd'),
#        tag = cms.string("OpticalAlignmentsRcd")
#    )),
#)
                                                
process.cocoa = cms.EDAnalyzer('CocoaAnalyzer',
				maxEvents = cms.int32(1),
				cocoaDaqRootFile = cms.string("cocoaDaqTest.root"),
				DDDetector = cms.ESInputTag('','')
                              )

process.p = cms.Path(process.cocoa)

