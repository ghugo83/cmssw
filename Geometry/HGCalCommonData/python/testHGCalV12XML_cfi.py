import FWCore.ParameterSet.Config as cms

# This config was generated automatically using generate2026Geometry.py
# If you notice a mistake, please update the generating script, not just this config

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring(
        'Geometry/CMSCommonData/data/materials.xml',
        'Geometry/CMSCommonData/data/rotations.xml',
        'Geometry/CMSCommonData/data/extend/cmsextent.xml',
        'Geometry/CMSCommonData/data/cms/2026/v4/cms.xml',
        'Geometry/CMSCommonData/data/eta3/etaMax.xml',
        'Geometry/CMSCommonData/data/cmsMother.xml',
        'Geometry/CMSCommonData/data/caloBase/2026/v3/caloBase.xml',
        'Geometry/CMSCommonData/data/cmsCalo.xml',
        'Geometry/CMSCommonData/data/beampipe/2026/v1/beampipe.xml',
        'Geometry/CMSCommonData/data/cmsBeam/2026/v1/cmsBeam.xml',
        'Geometry/CMSCommonData/data/cavernData/2021/v1/cavernData.xml',
        'Geometry/CMSCommonData/data/muonBase/2026/v4/muonBase.xml',
        'Geometry/CMSCommonData/data/cmsMuon.xml',
        'Geometry/CMSCommonData/data/mgnt.xml',
        'Geometry/CMSCommonData/data/muonMB.xml',
        'Geometry/CMSCommonData/data/muonMagnet.xml',
        'Geometry/EcalCommonData/data/eregalgo/2026/v2/eregalgo.xml',
        'Geometry/EcalCommonData/data/ectkcable/2026/v1/ectkcable.xml',
        'Geometry/EcalCommonData/data/ectkcablemat/2026/v1/ectkcablemat.xml',
        'Geometry/EcalCommonData/data/ebalgo.xml',
        'Geometry/EcalCommonData/data/ebcon.xml',
        'Geometry/EcalCommonData/data/ebrot.xml',
        'Geometry/HcalCommonData/data/hcalrotations.xml',
        'Geometry/HcalCommonData/data/hcal/v2/hcalalgo.xml',
        'Geometry/HcalCommonData/data/hcalbarrelalgo.xml',
        'Geometry/HcalCommonData/data/hcalcablealgo/v2/hcalcablealgo.xml',
        'Geometry/HcalCommonData/data/hcalouteralgo.xml',
        'Geometry/HcalCommonData/data/hcalforwardalgo.xml',
        'Geometry/HcalCommonData/data/hcalSimNumbering/NoHE/hcalSimNumbering.xml',
        'Geometry/HcalCommonData/data/hcalRecNumbering/NoHE/hcalRecNumbering.xml',
        'Geometry/HcalCommonData/data/average/hcalforwardmaterial.xml',
        'Geometry/HGCalCommonData/data/hgcalMaterial/v1/hgcalMaterial.xml',
        'Geometry/HGCalCommonData/data/hgcal/v12/hgcal.xml',
        'Geometry/HGCalCommonData/data/hgcalEE/v12/hgcalEE.xml',
        'Geometry/HGCalCommonData/data/hgcalcell/v9/hgcalcell.xml',
        'Geometry/HGCalCommonData/data/hgcalwafer/v9/hgcalwafer.xml',
        'Geometry/HGCalCommonData/data/hgcalHEsil/v12/hgcalHEsil.xml',
        'Geometry/HGCalCommonData/data/hgcalHEmix/v12/hgcalHEmix.xml',
        'Geometry/HGCalCommonData/data/hgcalCons/v12/hgcalCons.xml',
        'Geometry/ForwardCommonData/data/forwardshield/2017/v1/forwardshield.xml',
        'Geometry/MuonCommonData/data/mbCommon/2021/v1/mbCommon.xml',
        'Geometry/MuonCommonData/data/mb1/2015/v2/mb1.xml',
        'Geometry/MuonCommonData/data/mb2/2015/v2/mb2.xml',
        'Geometry/MuonCommonData/data/mb3/2015/v2/mb3.xml',
        'Geometry/MuonCommonData/data/mb4/2015/v2/mb4.xml',
        'Geometry/MuonCommonData/data/mb4Shield/2021/v1/mb4Shield.xml',
        'Geometry/MuonCommonData/data/muonYoke/2021/v3/muonYoke.xml',
        'Geometry/MuonCommonData/data/csc/2021/v2/csc.xml',
        'Geometry/MuonCommonData/data/mfshield/2017/v2/mfshield.xml',
        'Geometry/MuonCommonData/data/mf/2026/v5/mf.xml',
        'Geometry/MuonCommonData/data/rpcf/2026/v3/rpcf.xml',
        'Geometry/MuonCommonData/data/gemf/TDR_BaseLine/gemf.xml',
        'Geometry/MuonCommonData/data/gem11/TDR_BaseLine/gem11.xml',
        'Geometry/MuonCommonData/data/gem21/TDR_Dev/gem21.xml',
        'Geometry/MuonCommonData/data/mfshield/2026/v3/mfshield.xml',
        'Geometry/MuonCommonData/data/me0/TDR_Dev/v3/me0.xml',
    )+
    cms.vstring(
        'Geometry/MuonCommonData/data/muonNumbering/TDR_DeV/v1/muonNumbering.xml',
        'Geometry/EcalSimData/data/PhaseII/ecalsens.xml',
        'Geometry/HcalCommonData/data/hcalsens/NoHE/hcalsenspmf.xml',
        'Geometry/HcalSimData/data/hf.xml',
        'Geometry/HcalSimData/data/hfpmt.xml',
        'Geometry/HcalSimData/data/hffibrebundle.xml',
        'Geometry/HcalSimData/data/CaloUtil.xml',
        'Geometry/HGCalSimData/data/hgcsensv9.xml',
        'Geometry/MuonSimData/data/PhaseII/ME0EtaPart/muonSens.xml',
        'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml',
        'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml',
        'Geometry/CSCGeometryBuilder/data/cscSpecs.xml',
        'Geometry/RPCGeometryBuilder/data/2026/v1/RPCSpecs.xml',
        'Geometry/GEMGeometryBuilder/data/v7/GEMSpecsFilter.xml',
        'Geometry/GEMGeometryBuilder/data/v7/GEMSpecs.xml',
        'Geometry/HcalSimData/data/HcalProdCuts.xml',
        'Geometry/EcalSimData/data/EcalProdCuts.xml',
        'Geometry/HGCalSimData/data/hgcProdCutsv9.xml',
        'Geometry/CMSCommonData/data/FieldParameters.xml',
        'Geometry/MuonSimData/data/PhaseII/muonProdCuts.xml',
    ),
    rootNodeName = cms.string('cms:OCMS')
)
