### Visualize DDGeometry on Fireworks

    cmsRun Fireworks/Geometry/python/dumpSimGeometry_cfg.py tag=Telescope out=simGeoTelescope.root
    cmsShow --sim-geom-file simGeoTelescope.root -c $CMSSW_RELEASE_BASE/src/Fireworks/Core/macros/simGeo.fwc


### Print DetIds and associated coordinates. 

    cmsRun  Geometry/TrackerNumberingBuilder/test/printDetIds_cfg.py


### Produce the DetIds, numRows, numCols mapping file 

    cmsRun  SLHCUpgradeSimulations/Geometry/test/writeFile_phase2TiltedPixel4021_cfg.py


### Test topology 

    cmsRun Geometry/TrackerPhase2TestBeam/test/printTelescopeTopology_cfg.py


### Test TrackerDigiGeometry   

This is obviously not the DIGI step! But this geometry is the one directly used by the Digitizer.    

    cmsRun  Geometry/TrackerGeometryBuilder/test/tracker_cfg.py 


### GEM_SIM step 

    cmsRun Geometry/TrackerPhase2TestBeam/python/TrackerPhase2TestBeam_GEN_SIM_cfg.py


### Produce SimHit map

    cmsRun Geometry/TrackerPhase2TestBeam/python/TrackerPhase2TestBeam_GEN_SIM_cfg.py    
NB: GEM_SIM step required, obviously.   

    cmsRun Geometry/TrackerPhase2TestBeam/test/plotSimHitMap_cfg.py                 
NB: Analyzer in charge of actually plotting the SimHit map.


### DIGI step 

    cmsRun Geometry/TrackerPhase2TestBeam/python/SinglePionE120GeV_DIGI_cfg.py
NB: produces step2_DIGI.root  


### Produce DIGI map  

    cmsRun SimTracker/SiPhase2Digitizer/test/DigiTest_cfg.py
NB: Reads step2_DIGI.root and produces step1_DigiTest.root

    cmsRun SimTracker/SiPhase2Digitizer/test/DigiTest_Harvest_cfg.py
NB: Reads step1_DigiTest.root and produces the DQM files.
