import FWCore.ParameterSet.Config as cms

process = cms.Process("GenParticles")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
###process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
###process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#####process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('Tracking')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations   = cms.untracked.vstring('detailedInfo', 'critical', 'cerr'),
                                    critical       = cms.untracked.PSet(threshold = cms.untracked.string('ERROR'),filename = cms.untracked.string('critical.log')),
                                    detailedInfo   = cms.untracked.PSet(threshold = cms.untracked.string('INFO'),filename = cms.untracked.string('cinfo.log')),
                                    cerr           = cms.untracked.PSet(threshold = cms.untracked.string('WARNING'),filename = cms.untracked.string('cerr.log'))
                                    )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_E_V33A::All' #'START53_V11::All' #'GR_R_44_V10::All' -> low-pt tracking //START53_V11::All//STARTHI44_V11::All
# GR_P_V41::All -> /PAPhysics/PARun2012-PromptReco-v2/RECO
# START53_V11::All -> Colin's pPb EPOS

#******trigger*******
import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
process.gtDigis.DaqGtInputTag = '"source","","HLT"'

process.castorDigis = cms.EDProducer("CastorRawToDigi",
                                   CastorFirstFED = cms.untracked.int32(690),
                                   FilterDataQuality = cms.bool(True),
                                   ExceptionEmptyData = cms.untracked.bool(True),
#                                   InputLabel = cms.InputTag("rawDataRepacker"),
#                                   InputLabel = cms.InputTag("rawDataRepacker"),
                                   InputLabel = cms.InputTag("source","","HLT"),
                                   UnpackCalib = cms.untracked.bool(False),
                                   FEDs = cms.untracked.vint32(690,691,692),
                                   lastSample = cms.int32(9),
                                   firstSample = cms.int32(0)
                                     )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#        #'file:/usr/users/ikatkov/hiforest/CMSSW_4_4_2_patch7/test/skim_conformal_test.root'
#        'file:/storage/5/cbaus/RECO/PbPb_1380_eposLHC_HI44/job_N5_10.root'
#    )
#)

#!#!#readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",#,duplicateCheckMode = cms.untracked.string("checkEachRealDataFile"),
fileNames = cms.untracked.vstring(
#'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/209/948/00000/20032E91-D25C-E211-AC52-5404A63886CF.root' #LS13
#'root://eoscms//eos/cms/store/caf/user/vzhukova/EPOS_RECO/epos_reco_fix_1817_1_53Q.root'
#'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/355/00000/58A4172C-5261-E211-AF44-002481E0D524.root'
'root://eoscms//eos/cms/store/hidata/HIRun2013A/MinimumBias/RAW/v1/000/210/416/DABB728F-F461-E211-A9CD-5404A6388697.root' #ekatarina said this one for muon
),
#store/hidata/HIRun2013A/PAMinBiasUPC/RAW/v1/000/209/948/2C18FB73-FC5C-E211-95E9-001D09F2305C.root :: LS1-10
);

##vvvvv


process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string("gen_out.root"), #/afs/cern.ch/work/k/katkov/data/
                                    closeFileFast = cms.untracked.bool(True)
                                  )
#Trigger Selection
#import HLTrigger.HLTfilters.hltHighLevel_cfi
#process.hltMinBias = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
#process.hltMinBias.HLTPaths = ["HLT_PAL1Tech53_MB_v1"] #HLT_PAL1Tech53_MB_SingleTrack_v1

process.demo = cms.EDAnalyzer('RHAnalyser',
                              ShowDebug = cms.untracked.bool(True)
)

#process.p = cms.Path(process.hltMinBias*process.demo)
process.p = cms.Path(process.demo)
