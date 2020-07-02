# **CMS Object-oriented Code for Optical Alignment**

## General COCOA documentation

General documentation is at [1].   
COCOA website is at [2].   
Notably, a user guide can be found at [3].   

[1] https://cds.cern.ch/record/1047121/files/p193.pdf   
[2] http://cmsdoc.cern.ch/cms/MUON/alignment/software/COCOA/doc/cocoa.html   
[3] http://cmsdoc.cern.ch/cms/MUON/alignment/software/COCOA/doc/tex/users_guide.ps



## Run COCOA
This does not pretend to be exhaustive.    
It is what I could quickly recover in the context of moving COCOA to dd4hep (checking against regressions).   

### Input XML file
An XML file describing the optical alignment system of interest needs to be provided as input to COCOA.      
   
Such an XML file can be generated from a .txt description, by an independent cocoa application in `Alignment/CocoaApplication/bin`.   
For this testing purposes, I could for example generate an XML optical alignment system description, by simply doing the following:

    cd Alignment/CocoaApplication/bin
    cocoa simple2D.txt

### Input DB OpticalAlignmentsRcd
I had to generate an example input DB `OpticalAlignmentsRcd`, which can now be found at `Alignment/CocoaApplication/test/OpticalAlignments.db`.   
In `CocoaAnalyzer::analyze`, after `readXMLFile` returns, I hacked the following to get any optical alignment system I wanted (from XML description) to be stored into DB:

    Model& model = Model::getInstance();
    model.BuildSystemDescriptionFromOA(oaList_);
    CocoaDBMgr::getInstance()->DumpCocoaResults();
Of course, this is for testing purposes only. One would need to call the OpticalAlignmentsRcd of interest to the desired alignment study.

### Run COCOA
I have added cocoaAnalyzer_cfg.py to the release. To run the COCOA analysis, you can now just do:


    cd Alignment/CocoaApplication/test
    cmsRun cocoaAnalyzer_cfg.py
This will perform the least-squared fit, and store all results in `Alignment/CocoaApplication/test/OpticalAlignments.db`.      
The `OpticalAlignmentsRcd` obtained after fit, is stored with `OpticalAlignmentsRcdOutput` tag.      
It should also be possible to dump the results in ROOT format. Though, each COCOA feature needed to be debugged & fixed, and I have not explored that path, as I was happy with the dump to DB.
