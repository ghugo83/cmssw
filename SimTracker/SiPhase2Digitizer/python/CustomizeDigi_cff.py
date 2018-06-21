import FWCore.ParameterSet.Config as cms

def DigitizeTracker(process):
    print "!!! Customised Digitizer to Digitize only Tracker Hits !!!"
    if hasattr(process,'tkonlyDigitization_step'):
        process=customise_TkOnlyDigi(process)

    return process

def customise_TkOnlyDigi(process):
    process.load('Configuration.StandardSequences.Digi_cff')
    process.doAllDigi = cms.Sequence()
    process.addPileupInfo = cms.Sequence()
    process.genPUProtons = cms.Sequence()
    process.load('SimGeneral.MixingModule.mixObjects_cfi')
    process.tkonlyDigitization_step.remove(process.mix.mixObjects.mixCH)

    for name in process.mix.digitizers.parameterNames_():
        if name in ['pixel', 'mergedtruth']: continue
        delattr(process.mix.digitizers, name)
    delattr(process.mix.digitizers.mergedtruth.simHitCollections, 'muon')
#    print process.mix.digitizers
    

    del process.simCastorDigis
    del process.simEcalUnsuppressedDigis
    del process.simHcalUnsuppressedDigis
    del process.simSiStripDigis

#    del process.mix.digitizers.ecal
#    del process.mix.digitizers
#    process.tkonlyDigitization_step.remove(process.mix.digitizers.ecal)
#    process.tkonlyDigitization_step.remove(process.mix.digitizers.hcal)
#    process.tkonlyDigitization_step.remove(process.mix.digitizers.castor)

    # keep new digis
    alist=['FEVTDEBUG','FEVTDEBUGHLT','FEVT']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            getattr(process,b).outputCommands.append('keep Phase2TrackerDigiedmDetSetVector_*_*_*')
    return process

