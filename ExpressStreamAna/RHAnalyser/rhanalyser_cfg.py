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
#'root://eoscms//eos/cms/store/hidata/HIRun2013/PAMinBiasUPC/RAW/v1/000/210/498/00000/FA42D32C-1763-E211-AA6B-BCAEC532970D.root'
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0026F5EA-1064-E211-923A-001D09F2447F.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/02063F77-1C64-E211-A850-5404A640A639.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0254AE5C-0664-E211-B2E1-001D09F295A1.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/02C3AF42-1464-E211-937C-5404A6388697.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0490691B-2B64-E211-A5F9-001D09F253C0.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/087CA99E-0464-E211-BE4E-5404A638869E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/089A2839-4A64-E211-B7BE-003048D2C01A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/08DE0440-1464-E211-BD5D-003048F11114.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0ABD57A2-1E64-E211-861D-003048F110BE.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0ADA3642-1464-E211-B190-5404A63886EF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0E2CCA75-0B64-E211-AAE3-001D09F2841C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/0E9C0388-3B64-E211-9164-BCAEC518FF63.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/10F9059C-1B64-E211-B78F-003048D2BEA8.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/12016D17-1664-E211-86C0-003048F1C424.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1292E6F4-0F64-E211-8136-003048D2BC52.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1294E21B-4064-E211-BFC9-001D09F29146.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/12BE2C85-1264-E211-87F4-001D09F251FE.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/12CC0F3D-0064-E211-A3AB-5404A6388697.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/14135917-1664-E211-8431-003048F117B6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/14E30B41-1464-E211-9D94-5404A63886B7.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/160918C3-0D64-E211-BD3F-003048F024DA.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1687073E-5264-E211-BA1A-001D09F24DA8.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/18350F85-1264-E211-8CDE-001D09F24664.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/18A3EC82-3764-E211-A7D5-5404A63886AC.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/18C59FA0-3064-E211-BFC5-001D09F2AF1E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/18CCD09B-1B64-E211-9E9A-001D09F295FB.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1A8F4785-3264-E211-9AA8-5404A63886B7.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1C2C0D0A-1764-E211-ACC1-BCAEC518FF7E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1CC019C3-0D64-E211-8F3F-BCAEC532971B.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1E28153B-4964-E211-A7E7-5404A640A642.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1E50E373-0B64-E211-A6F7-0019B9F72CE5.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1E522D85-1264-E211-A040-001D09F2462D.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/1E7FFF29-4664-E211-B53E-003048D3C932.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/20642D7E-2D64-E211-A286-E0CB4E553673.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/20BD1C0C-2064-E211-8D39-5404A63886AC.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/20D0D3BB-2364-E211-B220-002481E0D646.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/22E6413E-5264-E211-A652-001D09F2462D.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/24836FA4-1A64-E211-8F8F-003048F1C832.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/24BF4714-1964-E211-AC64-001D09F28D54.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/24FD711B-4064-E211-AEB5-003048D2BE06.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/261B5341-1464-E211-A46E-0025901D5D7E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2690AD1B-1564-E211-A91E-001D09F29114.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2856A784-1264-E211-A5FA-001D09F24D4E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/28ADBC1A-1564-E211-8425-0019B9F72D71.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2A53F227-4D64-E211-A905-0030486780EC.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2A97D2B2-3E64-E211-AAD0-003048D2BEAA.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2AD21176-0B64-E211-A6CE-001D09F2915A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2CB42242-5464-E211-8282-001D09F25109.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2CCAF56A-3364-E211-ACF1-001D09F242EF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/2CFF6833-2C64-E211-ACE9-0025901D624A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/32406794-3A64-E211-B108-BCAEC518FF54.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/34499C5B-0664-E211-A9E7-001D09F34488.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/349850E4-2464-E211-812F-BCAEC518FF65.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/34AFFCC7-1964-E211-93E6-485B3977172C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/34D5F76D-0264-E211-BC20-003048D2C16E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/360713FB-2064-E211-B368-003048F024FE.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/36F100CA-2E64-E211-891B-001D09F252E9.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3838913A-4964-E211-8D77-0025901D631E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/38BDF49F-1E64-E211-A779-003048F11C58.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/38C1F4C8-1964-E211-B1F8-BCAEC518FF62.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/38C9D031-2C64-E211-90F7-003048F1C58C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3A436233-2C64-E211-80E7-0025901D626C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3A541B83-1264-E211-B525-001D09F26509.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3AE4F134-2C64-E211-8F23-5404A63886D6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3CAF13EC-1764-E211-A689-003048F11112.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3CC1516A-0364-E211-A7EC-BCAEC5329727.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/3E951C03-5B64-E211-AC18-001D09F290BF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/40D60BE4-2464-E211-B251-E0CB4E553651.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/429B9738-5064-E211-852D-001D09F28EA3.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/445BF349-5364-E211-87B5-001D09F248F8.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/462A7E90-2E64-E211-B56B-0030486730C6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/46AFC3AA-2664-E211-B4D4-0025901D5D7E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/48332888-3B64-E211-9AB3-5404A63886AB.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/484AEEA3-1A64-E211-B5C8-003048F11C58.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/48C30B32-0A64-E211-BEC0-003048CFB40C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/48CEB484-1264-E211-9A7C-001D09F24FBA.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4A06640B-2064-E211-9C78-5404A63886AF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4A4A1475-5864-E211-B7C2-5404A63886AE.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4A99AD0A-1764-E211-B0F2-0025B32445E0.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4AC60B07-3564-E211-BEB3-0025901D5D9A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4CBB672B-4564-E211-BDA0-0025901D5D90.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/4E9754DD-1364-E211-9850-0019B9F4A1D7.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/507B7F52-4C64-E211-BDED-BCAEC5329720.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5099C102-4164-E211-ADCB-BCAEC518FF50.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/52747179-3D64-E211-A029-E0CB4E553673.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/541A7E76-3164-E211-9193-001D09F24EE3.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/562B940E-2064-E211-AE59-5404A63886D4.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/56B47E8D-3C64-E211-98ED-003048D2C0F0.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/56CE5A33-4B64-E211-90B3-5404A6388694.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/58DB37E0-3964-E211-90AE-001D09F24399.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5ADC9BA4-1D64-E211-BAD9-003048D2BA82.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5C62E92E-2264-E211-A730-BCAEC5364C62.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5CD82B0E-4E64-E211-8038-003048D2C174.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5CF59D88-5864-E211-A1F5-5404A63886A2.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5EA728AF-2564-E211-8AB6-BCAEC53296F7.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/5EB7EC14-1964-E211-B4AD-0019B9F4A1D7.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6036BE1B-1564-E211-B930-001D09F2B2CF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/606EEB95-3A64-E211-97E4-5404A63886C6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/623134A7-0C64-E211-96A8-5404A63886BB.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/624796BD-2364-E211-A99E-003048F11942.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/62A0EDAA-2664-E211-9C09-0025901D62A0.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6426D41B-1564-E211-8A5A-0019B9F72CE5.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/647C3DCE-4364-E211-A5CF-BCAEC518FF8D.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/64D27625-0864-E211-BA07-5404A63886D3.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6661C19B-2964-E211-8E1B-001D09F2437B.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/668B8FA0-2764-E211-AEBA-003048D2BD28.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/66EC7B2F-0A64-E211-92B0-001D09F24664.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/68669D41-1464-E211-84A0-0025901D629C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6889DE17-1664-E211-B4DE-0025901D629C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6C3C60A0-2764-E211-8056-003048D3750A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6CFB8646-5564-E211-9B39-BCAEC518FF41.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6E336D3C-4964-E211-AA76-003048F11C5C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6E669F86-5864-E211-AE44-5404A6388698.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6E8BB14A-5364-E211-B245-003048D2BED6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/6EB8A9B6-0D64-E211-94C1-001D09F295A1.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/703D5F09-5B64-E211-82E0-001D09F2AD4D.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/704C2ACE-4364-E211-AD62-BCAEC518FF7A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/72665F79-3D64-E211-ACC7-BCAEC5329719.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/72AA366E-0564-E211-A66E-E0CB4E553651.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/74A0BC30-4764-E211-A5D6-BCAEC518FF44.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7820FB75-1C64-E211-B117-0025901D626C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/786665C8-1964-E211-94F5-5404A640A639.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7A880E85-1264-E211-BE3C-001D09F23F2A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7C30E728-4664-E211-AC3B-003048D2BB58.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7CF60CC9-1964-E211-8BF5-5404A63886A2.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7E34252C-4564-E211-8934-0025901D5C80.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/7E706425-0864-E211-8C09-5404A6388694.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/8021660B-0964-E211-AB39-5404A63886AF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/80258A9D-3064-E211-A1D2-001D09F2915A.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/80349AA5-1D64-E211-9113-003048D2BC5C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/80C9194A-5564-E211-9A56-001D09F24399.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/80FA65A7-0C64-E211-9464-5404A63886CF.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/829930C3-2F64-E211-8C6A-001D09F23174.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/842112E5-2464-E211-AFF5-BCAEC518FF8D.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/84EE2281-2364-E211-9AEC-003048D2C16E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/86CA4725-3664-E211-A077-003048D3C982.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/880866D5-0E64-E211-ABAC-003048D2BC5C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/885040EB-1764-E211-B2F7-003048F117B6.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/8892912E-0A64-E211-9A0A-5404A6388697.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/8ABBB687-3864-E211-8E99-001D09F34488.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/8CD0E5D4-0E64-E211-8720-001D09F253D4.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/8E03932F-2264-E211-9C19-BCAEC532971F.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/902F7F7D-2D64-E211-A4D1-0025901D623C.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/90B4662B-4564-E211-ADA7-0025901D5D7E.root',
'root://eoscms//eos/cms/store/express/HIRun2013/ExpressPhysics/FEVT/Express-v1/000/210/614/00000/921BD91B-1564-E211-9CA0-001D09F28F25.root'

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
