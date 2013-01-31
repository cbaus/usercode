import FWCore.ParameterSet.Config as cms

process = cms.Process("PEDS2013")

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
import EventFilter.L1GlobalTriggerRawToDigi.l1GtEvmUnpack_cfi 
process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
process.gtEvmDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtEvmUnpack_cfi.l1GtEvmUnpack.clone()
process.gtDigis.DaqGtInputTag = 'rawDataCollector'
process.gtEvmDigis.EvmGtInputTag = 'rawDataCollector'

process.load("EventFilter.CastorRawToDigi.CastorRawToDigi_cfi")

#******update pedestals******
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.es_castor_ascii = cms.ESSource("CastorTextCalibrations",
#   input = cms.VPSet(cms.PSet(
#       object = cms.string('ElectronicsMap'),
#       file = cms.FileInPath('emap_castor_cmssw_Sat_Jan_12_13_03_40_2013_209936.txt')
#   ),
#                    cms.PSet(
#       object = cms.string('Pedestals'),
#       file = cms.FileInPath('peds_cmssw_Jan_2013_209986.txt')
#   ),
#                    cms.PSet(
#       object = cms.string('PedestalWidths'),
#       file = cms.FileInPath('pedwidths_cmssw_Jan_2013_209986.txt')
#   )
#)
#)
#process.es_pool = cms.ESSource(
#   "PoolDBESSource",
#   process.CondDBSetup,
#   timetype = cms.string('runnumber'),
#   connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierPrep/CMS_COND_30X_HCAL'),
#   #connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'), 
#   authenticationMethod = cms.untracked.uint32(0),
#   toGet = cms.VPSet(
#
#       #cms.PSet(
#       #    record = cms.string('CastorPedestalsRcd'),
#       #    tag = cms.string('CastorPedestals_v2.0_offline')
#       #    ),
#       #cms.PSet(
#       #    record = cms.string('CastorPedestalWidthsRcd'),
#       #    tag = cms.string('CastorPedestalWidths_v2.0_offline')
#       #    ),
#       cms.PSet(
#           record = cms.string('CastorGainsRcd'),
#           tag = cms.string('castor_gains_v1.0_test') #CastorGains_v2.1_offline
#           ),
#       cms.PSet(
#           record = cms.string('CastorGainWidthsRcd'),
#           tag = cms.string('castor_gainwidths_v1.0_test') #CastorGainWidths_v2.1_offline
#           ),
#       cms.PSet(
#           record = cms.string('CastorQIEDataRcd'),
#           tag = cms.string('castor_qie_v1.0_test') #CastorQIEData_v2.0_offline???
#           ),
#       cms.PSet(
#           record = cms.string('CastorChannelQualityRcd'),
#           tag = cms.string('castor_channelquality_v1.0_test') #CastorChannelQuality_v2.2_offline #any channels marced as bad in rechits due to this ?
#           ),
#       #cms.PSet(
#       #    record = cms.string('CastorElectronicsMapRcd'),
#       #    tag = cms.string('CastorElectronicsMap_v2.0_offline')
#       #    )
###       cms.PSet(
###           record = cms.string('CastorPedestalsRcd'),
###           tag = cms.string('castor_pedestals_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorPedestalWidthsRcd'),
###           tag = cms.string('castor_pedestalwidths_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorGainsRcd'),
###           tag = cms.string('castor_gains_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorGainWidthsRcd'),
###           tag = cms.string('castor_gainwidths_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorQIEDataRcd'),
###           tag = cms.string('castor_qie_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorChannelQualityRcd'),
###           tag = cms.string('castor_channelquality_v1.0_test')
###           ),
###       cms.PSet(
###           record = cms.string('CastorElectronicsMapRcd'),
###           tag = cms.string('castor_emap_dcc_v1.0_test')
###           )
#   )
#)
##process.es_prefer_castor = cms.ESPrefer("CastorTextCalibrations","es_castor_ascii")


#process.CastorDbProducer = cms.ESProducer("CastorDbProducer",
#                                            dump = cms.untracked.vstring("Pedestals",
#                                                                         "PedestalWidths",
#                                                                         "Gains",
#                                                                         "QIEData",
#                                                                         "ElectronicsMap",
#                                                                         "ChannelQuality",
#                                                                         "GainWidths"
#                                                                         ),
#                                            file = cms.untracked.string('dumpI.txt')
#)


#******change to rawDataCollector*******
#for unpacking latest RAW files
process.castorDigis = cms.EDProducer("CastorRawToDigi",
                             # Optional filter to remove any digi with "data valid" off, "error" on,
                             # or capids not rotating
                             FilterDataQuality = cms.bool(True),
                             # Number of the first CASTOR FED.  If this is not specified, the
                             # default from FEDNumbering is used.
                             CastorFirstFED = cms.untracked.int32(690),
                             # FED numbers to unpack.  If this is not specified, all FEDs from
                             # FEDNumbering will be unpacked.
                             FEDs = cms.untracked.vint32( 690, 691, 692 ),
                             # Do not complain about missing FEDs
                             ExceptionEmptyData = cms.untracked.bool(False),
                             # Do not complain about missing FEDs
                             ComplainEmptyData = cms.untracked.bool(False),
                             # At most ten samples can be put into a digi, if there are more
                             # than ten, firstSample and lastSample select which samples
                             # will be copied to the digi
                             firstSample = cms.int32(0),
                             lastSample = cms.int32(9),
                             # castor technical trigger processor
                             UnpackTTP = cms.untracked.bool(True),
                             # report errors
                             silent = cms.untracked.bool(False),
                             #
                             InputLabel = cms.InputTag("rawDataCollector")
                             )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30000) )

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
#'root://eoscms//eos/cms/store/hidata/HIRun2013A/MinimumBias/RAW/v1/000/210/416/DABB728F-F461-E211-A9CD-5404A6388697.root' #ekatarina said this one for muon
#'root://eoscms//eos/cms/store/hidata/HIRun2013/PAMinBiasUPC/RAW/v1/000/210/498/00000/FA42D32C-1763-E211-AA6B-BCAEC532970D.root
    'root://EOSCMS//eos/cms/store/caf/user/dgulhan/pPb/HP03/prod09/Hijing_Pythia_pt15/HiForest_v55_v04_merged01/pt15_HP03_prod09_merged_forest_0.root'

),
#store/hidata/HIRun2013A/PAMinBiasUPC/RAW/v1/000/209/948/2C18FB73-FC5C-E211-95E9-001D09F2305C.root' :: LS1-10
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
process.p = cms.Path(process.castorDigis*process.gtDigis*process.demo)
