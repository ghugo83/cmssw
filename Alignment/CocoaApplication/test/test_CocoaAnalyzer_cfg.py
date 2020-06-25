import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

# Ideal geometry
#process.load('Geometry.CMSCommonData.cmsIdealGeometry2015XML_cfi')
#process.load("Geometry.CMSCommonData.cmsExtendedGeometry2026D51XML_cfi")

process.source = cms.Source("EmptySource")   
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


# GOOOOOOOOD = BANZAI 2
process.DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Alignment/CocoaApplication/test/table2DWithMirror_dd4hep.xml'),
                                            #confGeomXMLFiles = cms.FileInPath('Geometry/CMSCommonData/data/dd4hep/cmsExtendedGeometry2021.xml'),
                                            appendToDataLabel = cms.string('')
                                            )
                                            
process.DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                                 appendToDataLabel = cms.string('')
                                                )
                                                


# BANZAI 1
#XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
#    geomXMLFiles = cms.vstring("DetectorDescription/DDCMS/data/cmsWorld.xml",
#                               "Geometry/CMSCommonData/data/materials/2015/v1/materials.xml",
#                               "Alignment/CocoaApplication/test/table2DWithMirror_dd4hep.xml"),
#    rootNodeName = cms.string('cmsWorld:OCMS')
#)
                                          



#process.source = cms.Source("PoolSource",
                                #fileNames = cms.untracked.vstring(
            				#'file:/afs/cern.ch/cms/Tutorials/TWIKI_DATA/TTJets_8TeV_53X.root'
            				#'file:/eos/cms/store/relval/CMSSW_10_6_1_patch1/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU_106X_mcRun3_2021_realistic_v3-v1/10000/43AFE2E2-5D53-3440-9937-1E7EECBF7029.root'
            				#'file:/home/ghugo/CMSSW/CMSSW_11_2_0_pre1/src/Alignment/CocoaApplication/test/cocoaDaqTest.root'
            				#'file:/eos/cms/store/relval/CMSSW_11_2_0_pre1/RelValNuGun/GEN-SIM/110X_mcRun4_realistic_v3_2026D49noPU-v1/10000/F3227D3F-965E-9B42-9D7B-D80CB5B01FBE.root'
                		#)
                            #)

                        
                            
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.PoolDBESSourceGeometry = cms.ESSource("PoolDBESSource",
                               #process.CondDBSetup,
                               #timetype = cms.string('runnumber'),
                               #toGet = cms.VPSet(
                                            #cms.PSet(record = cms.string('IdealGeometryRecord'),tag = cms.string('TKRECO_Geometry_TagXX')),
                                            #),
                               #connect = cms.string('sqlite_file:OpticalAlignments.db')
                               #)                                                    
                         
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
                            
                        
                            
                            

process.demo = cms.EDAnalyzer('CocoaAnalyzer',
				maxEvents = cms.int32(1),
				cocoaDaqRootFile = cms.string("cocoaDaqTest.root"),
				DDDetector = cms.ESInputTag('','')
                              )

process.p = cms.Path(
process.demo)

