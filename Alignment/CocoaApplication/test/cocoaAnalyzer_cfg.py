import FWCore.ParameterSet.Config as cms

process = cms.Process("TestCocoa")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.source = cms.Source("EmptySource")   
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Alignment/CocoaApplication/test/table2DWithMirror_dd4hep.xml'),
                                            appendToDataLabel = cms.string('')
                                            )
                                            
process.DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                                 appendToDataLabel = cms.string('')
                                                )
                                                   
                         
#process.load("CondCore.CondDB.CondDB_cfi")
#process.CondDB.connect = 'sqlite_file:/home/ghugo/CMSSW/CMSSW_11_2_0_pre1/src/Alignment/CocoaApplication/test/OpticalAlignments.db'
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    process.CondDB,
#    DumpStat=cms.untracked.bool(True),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('IdealGeometryRecord'),
#        tag = cms.string("hey")
#    )),
#)


# Reading from DB
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    process.CondDBSetup,
#    toGet = cms.VPSet(
#       cms.PSet(
#            record = cms.string('OpticalAlignmentsRcd'),
#            tag = cms.string('OPTICALALIGNMENTS')
#        )),
#    connect = cms.string('sqlite_file:/home/ghugo/CMSSW/CMSSW_11_2_0_pre1/src/Alignment/CocoaApplication/test/OpticalAlignments.db')
#)
                            
                        
                                                 

process.cocoa = cms.EDAnalyzer('CocoaAnalyzer',
				maxEvents = cms.int32(1),
				cocoaDaqRootFile = cms.string("cocoaDaqTest.root"),
				DDDetector = cms.ESInputTag('','')
                              )

process.p = cms.Path(process.cocoa)

