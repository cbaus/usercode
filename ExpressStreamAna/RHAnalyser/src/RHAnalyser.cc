// -*- C++ -*-
//
// Package:    RHAnalyser
// Class:      RHAnalyser
// 
/**\class RHAnalyser RHAnalyser.cc Analysis/RHAnalyser/src/RHAnalyser.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Igor Katkov,32 4-A19,+41227676358,
//         Created:  Wed Jan 16 14:14:19 CET 2013
// $Id: RHAnalyser.cc,v 1.6 2013/01/20 19:25:36 cbaus Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <sstream>

//geometry
//
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"

//calorimeters
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"

// castor
//#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/CastorObjects/interface/CastorDbRecord.h"
#include "CalibFormats/CastorObjects/interface/CastorDbService.h"

//tfileservice
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//trigger
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
// menu Lite:                       
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"


// ROOT
#include "TROOT.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
//#include "TProfile.h"
//#include "TTree.h"
//#include "TFile.h"
#include "TLorentzVector.h"
//#include "DataFormats/Math/interface/LorentzVector.h"
#include "TText.h"
//
// class declaration
//

class RHAnalyser : public edm::EDAnalyzer {
public:
  explicit RHAnalyser(const edm::ParameterSet&);
  ~RHAnalyser();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  // ----------member data ---------------------------

  struct MonVariables
  {
    int nbCasRecHits;
    double casRecHitEnergy[224];
    int    casRecHitIphi[224];
    int    casRecHitIdepth[224];
    int    casRecHitSaturation[224];

    float casSignalRaw[14][16][10];
    float casSignal[14][16][10];
    float casCapID[14][16][10];

    int nbZDCRecHits;
    double zdcRecHitEnergy[18];
    int    zdcRecHitIside[18];
    int    zdcRecHitIsection [18];
    int    zdcRecHitIchannel[18];
    int    zdcRecHitSaturation[18];

    int nbZDCDigis;
    double zdcDigiEnergyFC[18][10];
    int    zdcDigiEnergyADC[18][10];
    int    zdcDigiIside[18];
    int    zdcDigiIsection[18];
    int    zdcDigiIchannel[18];

    int   L1[128];
    int   tt[64];
  };

  edm::Service<TFileService> fs_;
  // trigger stuff
  bool Trigger(const edm::Event&);
  edm::InputTag fl1GtTmLInputTag;      // input tag for L1GtTriggerMenuLite 
  edm::Handle<L1GtTriggerMenuLite> triggerMenuLite;

  // for ADC<->fC conversion from CastorDbRecord, initialized in beginRun 
  edm::ESHandle<CastorDbService> fConditions;

  // pedestal parameters from CastorPedestalsRcd, initialized in beginRun                    
  edm::ESHandle<CastorPedestals> fPedestals;

  // pedestal width averaged over capIDs, calculated in beginRun 
  //          aware of the difference between index[0..15][0..13]
  //          and sector/module numeration[1..16][1..14] 

  static const short unsigned int fNM = 14; // modules
  static const short unsigned int fNS = 16; // sectors
  static const short unsigned int fNTS = 10;


  const CaloGeometry* geo;

  bool _ShowDebug;

  TTree* rhtree_;

  MonVariables treeVariables_;

  int fNRun;
  int fNEvent;


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RHAnalyser::RHAnalyser(const edm::ParameterSet& iConfig) :
  _ShowDebug ( iConfig.getUntrackedParameter<bool>("ShowDebug",true) ) ,fNRun(0), fNEvent(0)
{
  //now do what ever initialization is needed

  rhtree_ = fs_->make<TTree>("RHTree","RHTree"); 

  rhtree_->Branch("nbCasRecHits",&treeVariables_.nbCasRecHits,"nbCasRecHits/i");
  rhtree_->Branch("casRecHitEnergy",treeVariables_.casRecHitEnergy,"casRecHitEnergy[nbCasRecHits]/D");
  rhtree_->Branch("casRecHitIphi",treeVariables_.casRecHitIphi,"casRecHitIphi[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitIdepth",treeVariables_.casRecHitIdepth,"casRecHitIdepth[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitSaturation",treeVariables_.casRecHitSaturation,"casRecHitSaturation[nbCasRecHits]/I");

  rhtree_->Branch("casSignalRaw",treeVariables_.casSignalRaw,"casSignalRaw[14]16][10]/F");
  rhtree_->Branch("casSignal",treeVariables_.casSignal,"casSignal[14]16][10]/F");
  rhtree_->Branch("casCapID",treeVariables_.casCapID,"casCapID[14]16][10]/F");

  rhtree_->Branch("nbZDCRecHits",&treeVariables_.nbZDCRecHits,"nbZDCRecHits/i");
  rhtree_->Branch("zdcRecHitEnergy",treeVariables_.zdcRecHitEnergy,"zdcRecHitEnergy[nbZDCRecHits]/D");
  rhtree_->Branch("zdcRecHitIside",treeVariables_.zdcRecHitIside,"zdcRecHitIside[nbZDCRecHits]/I");
  rhtree_->Branch("zdcRecHitIsection",treeVariables_.zdcRecHitIsection,"zdcRecHitIsection[nbZDCRecHits]/I");
  rhtree_->Branch("zdcRecHitIchannel",treeVariables_.zdcRecHitIchannel,"zdcRecHitIchannel[nbZDCRecHits]/I");
  rhtree_->Branch("zdcRecHitSaturation",treeVariables_.zdcRecHitSaturation,"zdcRecHitSaturation[nbZDCRecHits]/I");

  rhtree_->Branch("nbZDCDigis",&treeVariables_.nbZDCDigis,"nbZDCDigis/i");
  rhtree_->Branch("zdcDigiEnergyFC",treeVariables_.zdcDigiEnergyFC,"zdcDigiEnergyFC[nbZDCDigis][10]/D");
  rhtree_->Branch("zdcDigiEnergyADC",treeVariables_.zdcDigiEnergyADC,"zdcDigiEnergyADC[nbZDCDigis][10]/I");
  rhtree_->Branch("zdcDigiIside",treeVariables_.zdcDigiIside,"zdcDigiIside[nbZDCDigis]/I");
  rhtree_->Branch("zdcDigiIsection",treeVariables_.zdcDigiIsection,"zdcDigiIsection[nbZDCDigis]/I");
  rhtree_->Branch("zdcDigiIchannel",treeVariables_.zdcDigiIchannel,"zdcDigiIchannel[nbZDCDigis]/I");

}


RHAnalyser::~RHAnalyser()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
RHAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  fNEvent++; //iEvent.id().event() // in CMS parallel not ordered
  if (fNEvent % 1 == 0)
    std::cout << "Event: " << fNEvent << " LumiSec: " << iEvent.luminosityBlock() << " BX: " << iEvent.bunchCrossing() << std::endl;
  using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  // *************************** CASTOR RecHits ********************************

  edm::Handle<CastorRecHitCollection> casrechits;
  try{ iEvent.getByLabel("castorreco",casrechits); }
  catch(...) { edm::LogWarning(" CASTOR ") << " Cannot get Castor RecHits " << std::endl; }

  int nhits = 0;
  double energyCastor = 0;
  treeVariables_.nbCasRecHits = 0; 

  if(casrechits.failedToGet()!=0 || !casrechits.isValid()) {
    edm::LogWarning(" CASTOR ") << " Cannot read CastorRecHitCollection" << std::endl;
  } else {
    if (_ShowDebug) edm::LogVerbatim(" CastorRecHits ") << " CastorRecHitCollection size: " << casrechits->size() << std::endl;
    for(size_t i1 = 0; i1 < casrechits->size(); ++i1) {
      const CastorRecHit & rh = (*casrechits)[i1];
      HcalCastorDetId castorid = rh.id();
      energyCastor += rh.energy();
      if (nhits  < 224) {
	treeVariables_.nbCasRecHits = nhits + 1 ;
	treeVariables_.casRecHitEnergy[nhits] = rh.energy();
	treeVariables_.casRecHitIphi[nhits] = castorid.sector();
	treeVariables_.casRecHitIdepth[nhits] = castorid.module();
        treeVariables_.casRecHitSaturation[nhits] = static_cast<int>( rh.flagField(HcalCaloFlagLabels::ADCSaturationBit) );
      }
      nhits++;
    } // end loop castor rechits
    if (_ShowDebug && casrechits->size() > 0) edm::LogVerbatim(" CastorRecHits ") << " Castor energy: " << energyCastor << std::endl;
  }

  // *********************************** CASTOR Digis ****************************************
    const CastorQIEShape* converter = fConditions->getCastorShape();
    
    // castor digis

    edm::Handle<CastorDigiCollection> cas_digi_h;
    try{ iEvent.getByType(cas_digi_h); }
    catch(...) { edm::LogWarning("CAS ") << " Cannot get Castor Digi " << std::endl; }
    
    const CastorDigiCollection *cas_digi = cas_digi_h.failedToGet()? 0 : &*cas_digi_h;

    if(cas_digi && cas_digi_h.isValid())
      {
    
        double charge0, charge_correct;
        short unsigned int sec, mod;
        short unsigned int last_ts;
        
        for(int mm=0; mm<14; mm++)
          for(int ss=0; ss<16; ss++)
            for(int ttss=0; ttss<10; ttss++){
              treeVariables_.casSignal[mm][ss][ttss]    = -1000;
              treeVariables_.casSignalRaw[mm][ss][ttss] = -1000;
            };                                                                               
        for(CastorDigiCollection::const_iterator j=cas_digi->begin(); j!=cas_digi->end(); j++){
          const CastorDataFrame digi     = (const CastorDataFrame)(*j);
          const HcalCastorDetId CastorID = digi.id();
          sec = CastorID.sector() - 1;
          mod = CastorID.module() - 1;
          
          // converter object for ADC->fC   
          const CastorQIECoder * coder           = fConditions->getCastorCoder(digi.id().rawId());
          
          // pedestal object (db pedestal values)
          const CastorPedestal * pedestals_mean  = fPedestals->getValues(digi.id().rawId());
          
          last_ts = (fNTS<digi.size() ? fNTS:digi.size());
          //std::cout << "Event#" << iEvent.id().event()
          //              << "; m" << CastorID.module() << "s" << CastorID.sector() ;
          
          for(short unsigned int ts = 0; ts<last_ts; ts++){
            // charge [fC] 
            charge0       = coder->charge(*converter, digi.sample(ts).adc(), digi.sample(ts).capid());
            // charge corrected = charge [fC] - pedestal [fC] (from DB)
            charge_correct= charge0 - pedestals_mean->getValue(digi.sample(ts).capid());
            
            // fill the root tree
            treeVariables_.casSignal[mod][sec][ts]    = charge_correct;
            treeVariables_.casSignalRaw[mod][sec][ts] = digi.sample(ts).adc();
            treeVariables_.casCapID[mod][sec][ts]     = digi.sample(ts).capid();
            //std::cout << "\t" << digi.sample(ts).adc() << "\t" << charge0;      
          }
          //std::cout << std::endl;
        } // end castor collection loop
      }
    else
      edm::LogWarning("CAS") << " Castor digis not valid " << std::endl;


  // *********************************** Trigger      ****************************************

    bool trigger = Trigger(iEvent);

  // *********************************** ZDC RecHits ******************************************

  edm::Handle <ZDCRecHitCollection> zdcrechits_h; 
  try{ iEvent.getByLabel("zdcreco", zdcrechits_h); }
  catch(...) { edm::LogWarning("ZDC ") << " Cannot get ZDC RecHits " << std::endl;  }

  const ZDCRecHitCollection *zdcrechits = zdcrechits_h.failedToGet()? 0 : &*zdcrechits_h;
   
  int nZhits = 0; 
  double energyZDC = 0;
  if (zdcrechits) {
    if (_ShowDebug) edm::LogVerbatim("ZDCRecHits") << " ZDCRecHitCollection size: " << zdcrechits->size() << std::endl;
    for( size_t i1 = 0; i1<zdcrechits->size(); ++i1){
      const ZDCRecHit & zdcrechit = (*zdcrechits)[i1];
      energyZDC += zdcrechit.energy();
      if (nZhits < 18) {
	treeVariables_.nbZDCRecHits = nZhits + 1;
	treeVariables_.zdcRecHitEnergy[nZhits] = zdcrechit.energy();
	treeVariables_.zdcRecHitIside[nZhits] = zdcrechit.id().zside();
	treeVariables_.zdcRecHitIsection[nZhits] = zdcrechit.id().section();
	treeVariables_.zdcRecHitIchannel[nZhits] = zdcrechit.id().channel();
	//#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
	treeVariables_.zdcRecHitSaturation[nZhits] = static_cast<int>( zdcrechit.flagField(HcalCaloFlagLabels::ADCSaturationBit) );
      }
      nZhits++;
    } // enf of loop zdc rechits
    if (_ShowDebug && zdcrechits->size() > 0) edm::LogVerbatim(" ZDCRecHits ") << " ZDC energy: " << energyZDC << std::endl;
  }
  else { edm::LogVerbatim("ZDCRecHits") << " Empty ZDCRecHitCollection" << std::endl; }

  // *********************************     ZDC Digis ****************************

  edm::Handle<ZDCDigiCollection> zdc_digi_h;
  try{ iEvent.getByType(zdc_digi_h); }
  catch(...) { edm::LogWarning("ZDC ") << " Cannot get ZDC Digi " << std::endl; }

  const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

  int nZdigi = 0;
  if( zdc_digi ) {
    if( zdc_digi->size() > 0 ) {
      if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " ZDCDigiCollection  size: " << zdc_digi->size() << std::endl;
      for ( ZDCDigiCollection::const_iterator j=zdc_digi->begin(); j != zdc_digi->end(); ++j ) {
        double energyZDCDigi = 0;
	const ZDCDataFrame& digi = (const ZDCDataFrame)(*j);
        int lastTS = 0;
	if (nZdigi < 18) {
          treeVariables_.nbZDCDigis = nZdigi + 1;   
          treeVariables_.zdcDigiIside[nZdigi] = digi.id().zside();
          treeVariables_.zdcDigiIsection[nZdigi] = digi.id().section();
          treeVariables_.zdcDigiIchannel[nZdigi] = digi.id().channel();
          //CapIdTs0 = digi.sample(0).capid();
          lastTS = digi.size() <= 10 ? digi.size() : 10;
          for(int ts = 0; ts < lastTS; ts++){
            double energy = 2.6*digi.sample(ts).nominal_fC();     //2.6*digi[ts].nominal_fC();
            energyZDCDigi =+ energy;
            (treeVariables_.zdcDigiEnergyFC)[nZdigi][ts] = energy;
            (treeVariables_.zdcDigiEnergyADC)[nZdigi][ts] = digi.sample(ts).adc();
          } //end of loop zdc digi time slices
	}
        nZdigi++;
        if (_ShowDebug) edm::LogVerbatim(" ZDCDigi ") << " ZDC energy (all ts FC): " << energyZDCDigi << " TS#: " << lastTS << std::endl;
      } // end loop zdc digis

    } else {
      if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " ZDCDigiCollection pointer not empty; but nothing in it: " << zdc_digi->size()  << std::endl;
    }
  } else {
    if (_ShowDebug) edm::LogVerbatim("ZDCDigi") << " Empty ZDCDigiCollection pointer "  << std::endl;
  }
  
  // ******************************

  rhtree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
RHAnalyser::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
RHAnalyser::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
RHAnalyser::beginRun(edm::Run const&, const edm::EventSetup& iSetup)
{
  fNRun++;
  std::cout << "Run: " << fNRun << std::endl;
 //+++++++++++++++++++++++++ ADC<->fC converter  +++++++++++++++++++++++++++++++++++++++++//    
  iSetup.get<CastorDbRecord>().get(fConditions);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//            
    
  //+++++++++++++++++++++++ pedestal values   +++++++++++++++++++++++++++++++++++++++++++++//        
  iSetup.get<CastorPedestalsRcd>().get(fPedestals);
  if(!fPedestals.isValid())
    edm::LogWarning("CASTOR ") << "\t\t\t C A S T O R  has no CastorPedestals";
  if(fPedestals->isADC()){
    edm::LogError("CASTOR ") 
      << "\t\t\t DB pedestal values are in ADC while this analysis is supposed to work with pedestals in fC.\n";
    exit(1);
  }
}
  
// ------------ method called when ending the processing of a run  ------------
void 
RHAnalyser::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
RHAnalyser::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
RHAnalyser::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RHAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

////////////////////////////////////////////////////////////////////////////////////////////   
//                               T R I G G E R                                            //   
bool RHAnalyser::Trigger(const edm::Event& iEvent){//, const edm::EventSetup&){
  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
  iEvent.getByLabel( edm::InputTag("gtDigis"), gtReadoutRecord);
  if(!gtReadoutRecord.isValid())
    edm::LogWarning("CASTOR ") << "\t\t\t T R I G G E R  has no L1GlobalTriggerReadoutRecord";
  
  //
  // technical trigger bits (64 bits) :  typedef std::vector<bool> TechnicalTriggerWord;  
  TechnicalTriggerWord TechTrigg     = gtReadoutRecord->technicalTriggerWord();
  short unsigned int   TechTriggSize = (short unsigned int) TechTrigg.size();
  for(short unsigned int i=0; i<TechTriggSize; i++) treeVariables_.tt[i] = TechTrigg[i];

  //
  // algorithm bits: 128 bits         :  typedef std::vector<bool> DecisionWord;  
  DecisionWord         AlgoBits      = gtReadoutRecord->decisionWord();
  short unsigned int   AlgoBitsSize  = (short unsigned int) AlgoBits.size();
  for(short unsigned int i=0; i<AlgoBitsSize; i++) treeVariables_.L1[i] = AlgoBits[i];  

  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(RHAnalyser);
