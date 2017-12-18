import FWCore.ParameterSet.Config as cms

# Raw data
from DQM.SiPixelPhase1RawData.SiPixelPhase1RawData_cfi import *
# Pixel Digi Monitoring
from DQM.SiPixelPhase1Digis.SiPixelPhase1Digis_cfi import *
# Cluster (track-independent) monitoring
from DQM.SiPixelPhase1Clusters.SiPixelPhase1Clusters_cfi import *
# RecHit (clusters)
from DQM.SiPixelPhase1RecHits.SiPixelPhase1RecHits_cfi import *
# Residuals
from DQM.SiPixelPhase1TrackResiduals.SiPixelPhase1TrackResiduals_cfi import *
# Clusters ontrack/offtrack (also general tracks)
from DQM.SiPixelPhase1TrackClusters.SiPixelPhase1TrackClusters_cfi import *
# Hit Efficiencies
from DQM.SiPixelPhase1TrackEfficiency.SiPixelPhase1TrackEfficiency_cfi import *
# FED/RAW Data
from DQM.SiPixelPhase1RawData.SiPixelPhase1RawData_cfi import *
#Summary maps
from DQM.SiPixelPhase1Summary.SiPixelPhase1Summary_cfi import *

PerModule.enabled = False
IsOffline.enabled=True

siPixelPhase1OfflineDQM_source = cms.Sequence(SiPixelPhase1RawDataAnalyzer
                                            + SiPixelPhase1DigisAnalyzer
                                            + SiPixelPhase1ClustersAnalyzer
                                            + SiPixelPhase1RecHitsAnalyzer
                                            + SiPixelPhase1TrackResidualsAnalyzer
                                            + SiPixelPhase1TrackClustersAnalyzer
                                            + SiPixelPhase1TrackEfficiencyAnalyzer
                                            )


#Cosmics config

siPixelPhase1OfflineDQM_source_cosmics = siPixelPhase1OfflineDQM_source.copyAndExclude([
    SiPixelPhase1TrackEfficiencyAnalyzer 
])

SiPixelPhase1TrackResidualsAnalyzer_cosmics = SiPixelPhase1TrackResidualsAnalyzer.clone()
SiPixelPhase1TrackResidualsAnalyzer_cosmics.Tracks = "ctfWithMaterialTracksP5"
SiPixelPhase1TrackResidualsAnalyzer_cosmics.trajectoryInput = "ctfWithMaterialTracksP5"
SiPixelPhase1TrackResidualsAnalyzer_cosmics.VertexCut =cms.untracked.bool(False) # don't cuts based on the primary vertex position for cosmics


siPixelPhase1OfflineDQM_source_cosmics.replace(SiPixelPhase1TrackResidualsAnalyzer,
                                               SiPixelPhase1TrackResidualsAnalyzer_cosmics)

SiPixelPhase1RecHitsAnalyzer_cosmics = SiPixelPhase1RecHitsAnalyzer.clone()
SiPixelPhase1RecHitsAnalyzer_cosmics.onlyValidHits = True # In Cosmics the efficiency plugin will not run, so we monitor only valid hits
SiPixelPhase1RecHitsAnalyzer_cosmics.src = "ctfWithMaterialTracksP5"
SiPixelPhase1RecHitsAnalyzer_cosmics.VertexCut = cms.untracked.bool(False)

siPixelPhase1OfflineDQM_source_cosmics.replace(SiPixelPhase1RecHitsAnalyzer,
                                               SiPixelPhase1RecHitsAnalyzer_cosmics)

SiPixelPhase1TrackClustersAnalyzer_cosmics = SiPixelPhase1TrackClustersAnalyzer.clone()
SiPixelPhase1TrackClustersAnalyzer_cosmics.tracks = "ctfWithMaterialTracksP5"
SiPixelPhase1TrackClustersAnalyzer_cosmics.VertexCut = cms.untracked.bool(False)

siPixelPhase1OfflineDQM_source_cosmics.replace(SiPixelPhase1TrackClustersAnalyzer,
                                               SiPixelPhase1TrackClustersAnalyzer_cosmics)


#heavy ions config

siPixelPhase1OfflineDQM_source_hi = siPixelPhase1OfflineDQM_source.copyAndExclude([
    SiPixelPhase1RecHitsAnalyzer,
    SiPixelPhase1TrackResidualsAnalyzer 
])


#SiPixelPhase1TrackResidualsAnalyzer_hi = SiPixelPhase1TrackResidualsAnalyzer.clone()
#SiPixelPhase1TrackResidualsAnalyzer_hi.Tracks = "hiGeneralTracks"
#SiPixelPhase1TrackResidualsAnalyzer_hi.trajectoryInput = "hiGeneralTracks"
#SiPixelPhase1TrackResidualsAnalyzer_hi.vertices = "hiSelectedVertex"
#
#siPixelPhase1OfflineDQM_source_hi.replace(SiPixelPhase1TrackResidualsAnalyzer,
#                                               SiPixelPhase1TrackResidualsAnalyzer_hi)

SiPixelPhase1TrackClustersAnalyzer_hi = SiPixelPhase1TrackClustersAnalyzer.clone()
SiPixelPhase1TrackClustersAnalyzer_hi.tracks = "hiGeneralTracks"
SiPixelPhase1TrackClustersAnalyzer_hi.vertices = "hiSelectedVertex"

siPixelPhase1OfflineDQM_source_hi.replace(SiPixelPhase1TrackClustersAnalyzer,
                                               SiPixelPhase1TrackClustersAnalyzer_hi)

SiPixelPhase1TrackEfficiencyAnalyzer_hi = SiPixelPhase1TrackEfficiencyAnalyzer.clone()
SiPixelPhase1TrackEfficiencyAnalyzer_hi.tracks = "hiGeneralTracks"
SiPixelPhase1TrackEfficiencyAnalyzer_hi.primaryvertices = "hiSelectedVertex"

siPixelPhase1OfflineDQM_source_hi.replace(SiPixelPhase1TrackEfficiencyAnalyzer,
                                               SiPixelPhase1TrackEfficiencyAnalyzer_hi)
