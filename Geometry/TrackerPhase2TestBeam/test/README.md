### Visualize DDGeometry on Fireworks

    cmsRun Fireworks/Geometry/python/dumpSimGeometry_cfg.py tag=Telescope out=simGeoTelescope.root
    cmsShow --sim-geom-file simGeoTelescope.root -c $CMSSW_RELEASE_BASE/src/Fireworks/Core/macros/simGeo.fwc


### Print DetIds and associated coordinates. 

    cmsRun Geometry/TrackerPhase2TestBeam/test/printDetIds_cfg.py


### Produce the DetIds, numRows, numCols mapping file 

    cmsRun SLHCUpgradeSimulations/Geometry/test/printGeometry_pixelTelescope_cfg.py


### Test topology 

    cmsRun Geometry/TrackerPhase2TestBeam/test/printTelescopeTopology_cfg.py


### Test TelescopeDigiGeometry   

This is obviously not the DIGI step! But this geometry is the one directly used by the Digitizer.    

    cmsRun Geometry/TrackerPhase2TestBeam/test/telescope_cfg.py


### GEM_SIM step 

    cmsRun Geometry/TrackerPhase2TestBeam/python/TrackerPhase2TestBeam_GEN_SIM_cfg.py


### Produce SimHit map

    cmsRun Geometry/TrackerPhase2TestBeam/python/TrackerPhase2TestBeam_GEN_SIM_cfg.py    
NB: GEM_SIM step required, obviously.   

    cmsRun Geometry/TrackerPhase2TestBeam/test/plotSimHitMap_cfg.py                 
NB: Analyzer in charge of actually plotting the SimHit map.



### Digis (on progress)
cmsRun SimTracker/SiPixelDigitizer/test/testPixelDigitizer.py  SimHit -> Digis

cmsRun SimTracker/SiPhase2Digitizer/test/DigiTest_cfg.py       Monitor Digis which are obtained at the previous step



### Debug with Fireworks (on progress)
 cmsShow -i TrackerPhase2TestBeam_DIGI.root  --sim-geom-file  simGeoTelescope.root  -c $CMSSW_RELEASE_BASE/src/Fireworks/Core/macros/simGeo.fwc
 # replace by
 cmsRun Fireworks/Geometry/python/dumpRecoGeometry_cfg.py tag=Telescope out=recoGeoTelescope.root
 cmsShow -i TrackerPhase2TestBeam_DIGI.root  --geom-file  recoGeoTelescope.root  -c $CMSSW_RELEASE_BASE/src/Fireworks/Core/macros/simGeo.fwc <- or miniaod.fwc ?
