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
// $Id: RHAnalyser.cc,v 1.2 2013/01/18 21:08:04 cbaus Exp $
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

    
  };

  edm::Service<TFileService> fs_;

  const CaloGeometry* geo;

  bool _ShowDebug;

  TTree* rhtree_;

  MonVariables treeVariables_;


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
  _ShowDebug ( iConfig.getUntrackedParameter<bool>("ShowDebug",true) )
{
  //now do what ever initialization is needed

  rhtree_ = fs_->make<TTree>("RHTree","RHTree"); 

  rhtree_->Branch("nbCasRecHits",&treeVariables_.nbCasRecHits,"nbCasRecHits/i");
  rhtree_->Branch("casRecHitEnergy",treeVariables_.casRecHitEnergy,"casRecHitEnergy[nbCasRecHits]/D");
  rhtree_->Branch("casRecHitIphi",treeVariables_.casRecHitIphi,"casRecHitIphi[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitIdepth",treeVariables_.casRecHitIdepth,"casRecHitIdepth[nbCasRecHits]/I");
  rhtree_->Branch("casRecHitSaturation",treeVariables_.casRecHitSaturation,"casRecHitSaturation[nbCasRecHits]/I");

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
RHAnalyser::beginRun(edm::Run const&, edm::EventSetup const&)
{
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

//define this as a plug-in
DEFINE_FWK_MODULE(RHAnalyser);
