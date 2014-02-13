#ifndef __analysisfiles__h__
#define __analysisfiles__h__

#include <string>
#include <map>
#include <vector>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <stdexcept>
#include <exception>

#include "HepMC/IO_GenEvent.h"

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "ProgressBar.h"

class HistoManager
{
private:
  int count_900GeV;
  int count_2760GeV;
  int count_7TeV;
  int count;

  TFile* theOutFile;
  TH1D* lhcf_pion_bin0;
  TH1D* lhcf_pion_bin1;
  TH1D* lhcf_pion_bin2;
  TH1D* lhcf_pion_bin3;
  TH1D* lhcf_pion_bin4;
  TH1D* lhcf_pion_bin5;

  TH1D* castor_E;
  TH1D* castor_composition;

public:
 HistoManager(TFile* theFile,std::string modelname) : count_900GeV(0), count_2760GeV(0), count_7TeV(0), count(0)
  {
    theOutFile = theFile;
    theOutFile->mkdir(modelname.c_str());
    theOutFile->cd(modelname.c_str());
    double bins0[] = {0.0, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6};
    lhcf_pion_bin0 = new TH1D((std::string("lhcf_pion_bin0")).c_str(),"8.9 - 9.0; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins0);
    double bins1[] = {0.0, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6};
    lhcf_pion_bin1 = new TH1D((std::string("lhcf_pion_bin1")).c_str(),"9.0 - 9.2; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins1);
    double bins2[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.5, 0.6};
    lhcf_pion_bin2 = new TH1D((std::string("lhcf_pion_bin2")).c_str(),"9.2 - 9.4; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins2);
    double bins3[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.5, 0.6};
    lhcf_pion_bin3 = new TH1D((std::string("lhcf_pion_bin3")).c_str(),"9.4 - 9.6; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins3);
    double bins4[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.45, 0.5, 0.6};
    lhcf_pion_bin4 = new TH1D((std::string("lhcf_pion_bin4")).c_str(),"9.6 - 10.0; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins4);
    double bins5[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6};
    lhcf_pion_bin5 = new TH1D((std::string("lhcf_pion_bin5")).c_str(),"10.0 - 11.0; p_{t} [GeV]; 1/#sigma_{inel}Ed^{3}#sigma/dp^{3} [GeV^{-2}]",10,bins5);
 
    castor_E = new TH1D((std::string("castor_E")).c_str(),"castor; ; E[GeV]",3,0.5,3.5);
    castor_composition = new TH1D((std::string("castor_composition")).c_str(),"castor; type; E[GeV]",6,-0.5,5.5); //all, pi0, pic, baryon, charge, other
  }

  void AddEvent(std::string energy)
  {
    if(energy=="7TeV") ++count_7TeV;
    else if(energy=="2760GeV") ++count_2760GeV;
    else if(energy=="900GeV") ++count_900GeV;
    else throw std::runtime_error ("unknown energy");
    ++count;
  }

 void FinaliseHistos()
   {
     double norm = castor_E->GetBinContent(2) / double(count_2760GeV);
     if(norm && count_900GeV && count_2760GeV && count_7TeV)
       {
	 castor_E->SetBinContent(1,castor_E->GetBinContent(1) / double(count_900GeV) / norm);
	 castor_E->SetBinContent(2,castor_E->GetBinContent(2) / double(count_2760GeV) / norm);
	 castor_E->SetBinContent(3,castor_E->GetBinContent(3) / double(count_7TeV) / norm);

	 castor_E->SetBinError(1,castor_E->GetBinError(1) / double(count_900GeV) / norm);
	 castor_E->SetBinError(2,castor_E->GetBinError(2) / double(count_2760GeV) / norm);
	 castor_E->SetBinError(3,castor_E->GetBinError(3) / double(count_7TeV) / norm);
       }

     if(count_7TeV) castor_composition->Scale(1./double(count_7TeV));
	
     if(count_7TeV) lhcf_pion_bin0->Scale(1./lhcf_pion_bin0->GetBinWidth(1)/double(count_7TeV));
     if(count_7TeV) lhcf_pion_bin1->Scale(1./lhcf_pion_bin1->GetBinWidth(1)/double(count_7TeV));
     if(count_7TeV) lhcf_pion_bin2->Scale(1./lhcf_pion_bin2->GetBinWidth(1)/double(count_7TeV));
     if(count_7TeV) lhcf_pion_bin3->Scale(1./lhcf_pion_bin3->GetBinWidth(1)/double(count_7TeV));
     if(count_7TeV) lhcf_pion_bin4->Scale(1./lhcf_pion_bin4->GetBinWidth(1)/double(count_7TeV));
     if(count_7TeV) lhcf_pion_bin5->Scale(1./lhcf_pion_bin5->GetBinWidth(1)/double(count_7TeV));
   }
  
  
  void FillLHCF(double rap, double pt)
  {
    if(8.9 < rap && rap < 9.0) lhcf_pion_bin0->Fill(pt,1./2./TMath::Pi()/pt/0.1); //later divide by bin width and rap width
    if(9.0 < rap && rap < 9.2) lhcf_pion_bin1->Fill(pt,1./2./TMath::Pi()/pt/0.2);
    if(9.2 < rap && rap < 9.4) lhcf_pion_bin2->Fill(pt,1./2./TMath::Pi()/pt/0.2);
    if(9.4 < rap && rap < 9.6) lhcf_pion_bin3->Fill(pt,1./2./TMath::Pi()/pt/0.2);
    if(9.6 < rap && rap < 10.) lhcf_pion_bin4->Fill(pt,1./2./TMath::Pi()/pt/0.4);
    if(10. < rap && rap < 11.) lhcf_pion_bin5->Fill(pt,1./2./TMath::Pi()/pt/1.0);
  }

  void FillCASTOR(double castor_e, std::string energy)
  {
    if(energy == "7TeV") castor_E->Fill(3,castor_e);
    if(energy == "2760GeV") castor_E->Fill(2,castor_e);
    if(energy == "900GeV") castor_E->Fill(1,castor_e);
  }

  void FillCASTORcomposition(double castor_e, double castor_e_pi0, double castor_e_pic, double castor_e_bar, double castor_e_gam, double castor_e_oth)
  {
    castor_composition->Fill(0.,castor_e);
    castor_composition->Fill(1,castor_e_pi0);
    castor_composition->Fill(2,castor_e_pic);
    castor_composition->Fill(3,castor_e_bar);
    castor_composition->Fill(4,castor_e_gam);
    castor_composition->Fill(5,castor_e_oth);
  }

};


class DataManager
{
private:
  TFile* theOutFile;
  std::map< std::string,std::vector<std::string> > models;
  std::map< std::string,HistoManager*> modelsHisto;
  std::map< std::string,std::vector<std::string> >::iterator current_model;
  int current_file;
  progress::ProgressBar theProgressBar;
  boost::iostreams::filtering_istream in;
  HepMC::IO_GenEvent* ascii_in;
  int count;

public:
  HepMC::GenEvent* evt;
  DataManager() : current_file(-1), ascii_in(0), count(0), evt(0)
  {
    std::cout << "DataManager::Opening output file" << std::endl;
    theOutFile = new TFile("histograms.root","RECREATE");

    std::vector<std::string> filessibyll;
    filessibyll.push_back("files/sibyll_7TeV_1.hepmc");
    filessibyll.push_back("files/sibyll_7TeV_2.hepmc");
    filessibyll.push_back("files/sibyll_7TeV_3.hepmc");
    filessibyll.push_back("files/sibyll_7TeV_4.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_5.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_6.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_7.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_8.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_9.hepmc.gz");
    filessibyll.push_back("files/sibyll_7TeV_100.hepmc.gz");
    filessibyll.push_back("files/sibyll_900GeV_1.hepmc");
    filessibyll.push_back("files/sibyll_2760GeV_1.hepmc");


    std::vector<std::string> filesepos199;
    filesepos199.push_back("files/epos199_2760GeV_7558.hepmc.gz");
    filesepos199.push_back("files/epos199_7TeV_27231.hepmc.gz");
    filesepos199.push_back("files/epos199_7TeV_4250.hepmc.gz");
    filesepos199.push_back("files/epos199_7TeV_4774.hepmc.gz");
    filesepos199.push_back("files/epos199_900GeV_7558.hepmc.gz");

    std::vector<std::string> filesqgsjet04;
    filesqgsjet04.push_back("files/qgsjetII-04_2760GeV_5633.hepmc.gz");
    filesqgsjet04.push_back("files/qgsjetII-04_7TeV_14783.hepmc.gz");
    filesqgsjet04.push_back("files/qgsjetII-04_7TeV_3122.hepmc.gz");
    filesqgsjet04.push_back("files/qgsjetII-04_7TeV_7132.hepmc.gz");
    filesqgsjet04.push_back("files/qgsjetII-04_900GeV_21579.hepmc.gz");

    std::vector<std::string> filesdpmjet;
    filesdpmjet.push_back("files/dpmjet_2760GeV_31792.hepmc.gz");
    filesdpmjet.push_back("files/dpmjet_7TeV_11026.hepmc.gz");
    filesdpmjet.push_back("files/dpmjet_7TeV_30176.hepmc.gz");
    filesdpmjet.push_back("files/dpmjet_7TeV_31280.hepmc.gz");
    filesdpmjet.push_back("files/dpmjet_900GeV_5527.hepmc.gz");

    std::vector<std::string> fileseposlhc;
    fileseposlhc.push_back("files/eposlhc_2760GeV_13849.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_7TeV_1067.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_7TeV_15206.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_7TeV_24887.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_7TeV_30250.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_7TeV_8521.hepmc.gz");
    fileseposlhc.push_back("files/eposlhc_900GeV_5302.hepmc.gz");

    std::vector<std::string> filesqgsjet04_ptdif05_ptndif29;
    filesqgsjet04_ptdif05_ptndif29.push_back("files/qgsjetII4_ptdif05_ptndif29_2760GeV_8775.hepmc.gz");
    filesqgsjet04_ptdif05_ptndif29.push_back("files/qgsjetII4_ptdif05_ptndif29_7TeV_12377.hepmc.gz");
    filesqgsjet04_ptdif05_ptndif29.push_back("files/qgsjetII4_ptdif05_ptndif29_7TeV_23207.hepmc.gz");
    filesqgsjet04_ptdif05_ptndif29.push_back("files/qgsjetII4_ptdif05_ptndif29_7TeV_3842.hepmc.gz");
    filesqgsjet04_ptdif05_ptndif29.push_back("files/qgsjetII4_ptdif05_ptndif29_900GeV_1996.hepmc.gz");

    models.insert(std::pair<std::string,std::vector<std::string> >("EposLHC",filesepos199));
    models.insert(std::pair<std::string,std::vector<std::string> >("Epos199",fileseposlhc));
    models.insert(std::pair<std::string,std::vector<std::string> >("Sibyll21",filessibyll));
    models.insert(std::pair<std::string,std::vector<std::string> >("QGSJetII04",filesqgsjet04));
    models.insert(std::pair<std::string,std::vector<std::string> >("QGSJetII04_ptdif05_ptndif29",filesqgsjet04_ptdif05_ptndif29));
    models.insert(std::pair<std::string,std::vector<std::string> >("DPMJet3-06",filesdpmjet));
    current_model = models.begin();

    for(std::map<std::string,std::vector<std::string> >::const_iterator i = models.begin(); i != models.end(); ++i)
      {
	modelsHisto.insert(std::pair<std::string,HistoManager*>(i->first,new HistoManager(theOutFile,i->first)));
      }

    //open first file
    if(!GetNextFile())
      throw std::runtime_error ("no file");

    theOutFile->mkdir("Data");
    theOutFile->cd("Data");
    TGraphAsymmErrors* lhcf_data_bin0 = new TGraphAsymmErrors(9); lhcf_data_bin0->SetName("lhcf_data_bin0");
    TGraphAsymmErrors* lhcf_data_bin1 = new TGraphAsymmErrors(9); lhcf_data_bin1->SetName("lhcf_data_bin1");
    TGraphAsymmErrors* lhcf_data_bin2 = new TGraphAsymmErrors(10); lhcf_data_bin2->SetName("lhcf_data_bin2");
    TGraphAsymmErrors* lhcf_data_bin3 = new TGraphAsymmErrors(9); lhcf_data_bin3->SetName("lhcf_data_bin3");
    TGraphAsymmErrors* lhcf_data_bin4 = new TGraphAsymmErrors(7); lhcf_data_bin4->SetName("lhcf_data_bin4");
    TGraphAsymmErrors* lhcf_data_bin5 = new TGraphAsymmErrors(5); lhcf_data_bin5->SetName("lhcf_data_bin5");
    lhcf_data_bin0->SetPoint(0,1.250000e-01,2.710000e-01); lhcf_data_bin0->SetPointError(0,2.500000e-02,2.500000e-02,1.410000e-01,1.580000e-01);
    lhcf_data_bin0->SetPoint(1,1.750000e-01,1.950000e-01); lhcf_data_bin0->SetPointError(1,2.500000e-02,2.500000e-02,8.500000e-02,9.950000e-02);
    lhcf_data_bin0->SetPoint(2,2.250000e-01,1.250000e-01); lhcf_data_bin0->SetPointError(2,2.500000e-02,2.500000e-02,4.980000e-02,5.660000e-02);
    lhcf_data_bin0->SetPoint(3,2.750000e-01,7.150000e-02); lhcf_data_bin0->SetPointError(3,2.500000e-02,2.500000e-02,2.540000e-02,2.900000e-02);
    lhcf_data_bin0->SetPoint(4,3.250000e-01,4.340000e-02); lhcf_data_bin0->SetPointError(4,2.500000e-02,2.500000e-02,4.210000e-03,4.220000e-03);
    lhcf_data_bin0->SetPoint(5,3.750000e-01,2.360000e-02); lhcf_data_bin0->SetPointError(5,2.500000e-02,2.500000e-02,3.650000e-03,3.660000e-03);
    lhcf_data_bin0->SetPoint(6,4.250000e-01,1.500000e-02); lhcf_data_bin0->SetPointError(6,2.500000e-02,2.500000e-02,3.250000e-03,3.260000e-03);
    lhcf_data_bin0->SetPoint(7,4.750000e-01,6.730000e-03); lhcf_data_bin0->SetPointError(7,2.500000e-02,2.500000e-02,2.150000e-03,2.160000e-03);
    lhcf_data_bin0->SetPoint(8,5.500000e-01,3.420000e-03); lhcf_data_bin0->SetPointError(8,5.000000e-02,5.000000e-02,1.270000e-03,1.270000e-03);
    lhcf_data_bin0->Write();

    lhcf_data_bin1->SetPoint(0,1.250000e-01,2.300000e-01); lhcf_data_bin1->SetPointError(0,2.500000e-02,2.500000e-02,8.060000e-02,8.120000e-02);
    lhcf_data_bin1->SetPoint(1,1.750000e-01,1.510000e-01); lhcf_data_bin1->SetPointError(1,2.500000e-02,2.500000e-02,3.990000e-02,4.190000e-02);
    lhcf_data_bin1->SetPoint(2,2.250000e-01,8.920000e-02); lhcf_data_bin1->SetPointError(2,2.500000e-02,2.500000e-02,3.140000e-03,3.140000e-03);
    lhcf_data_bin1->SetPoint(3,2.750000e-01,5.430000e-02); lhcf_data_bin1->SetPointError(3,2.500000e-02,2.500000e-02,3.350000e-03,3.350000e-03);
    lhcf_data_bin1->SetPoint(4,3.250000e-01,3.210000e-02); lhcf_data_bin1->SetPointError(4,2.500000e-02,2.500000e-02,3.310000e-03,3.320000e-03);
    lhcf_data_bin1->SetPoint(5,3.750000e-01,1.800000e-02); lhcf_data_bin1->SetPointError(5,2.500000e-02,2.500000e-02,2.720000e-03,2.730000e-03);
    lhcf_data_bin1->SetPoint(6,4.250000e-01,8.910000e-03); lhcf_data_bin1->SetPointError(6,2.500000e-02,2.500000e-02,1.830000e-03,1.840000e-03);
    lhcf_data_bin1->SetPoint(7,4.750000e-01,3.590000e-03); lhcf_data_bin1->SetPointError(7,2.500000e-02,2.500000e-02,1.010000e-03,1.010000e-03);
    lhcf_data_bin1->SetPoint(8,5.500000e-01,9.720000e-04); lhcf_data_bin1->SetPointError(8,5.000000e-02,5.000000e-02,3.660000e-04,3.650000e-04);
    lhcf_data_bin1->Write();

    lhcf_data_bin2->SetPoint(0,2.500000e-02,3.310000e-01); lhcf_data_bin2->SetPointError(0,2.500000e-02,2.500000e-02,1.580000e-01,1.840000e-01);
    lhcf_data_bin2->SetPoint(1,7.500000e-02,2.310000e-01); lhcf_data_bin2->SetPointError(1,2.500000e-02,2.500000e-02,9.050000e-02,1.070000e-01);
    lhcf_data_bin2->SetPoint(2,1.250000e-01,1.660000e-01); lhcf_data_bin2->SetPointError(2,2.500000e-02,2.500000e-02,1.960000e-02,1.960000e-02);
    lhcf_data_bin2->SetPoint(3,1.750000e-01,1.060000e-01); lhcf_data_bin2->SetPointError(3,2.500000e-02,2.500000e-02,3.760000e-03,3.760000e-03);
    lhcf_data_bin2->SetPoint(4,2.250000e-01,5.710000e-02); lhcf_data_bin2->SetPointError(4,2.500000e-02,2.500000e-02,2.690000e-03,2.690000e-03);
    lhcf_data_bin2->SetPoint(5,2.750000e-01,3.580000e-02); lhcf_data_bin2->SetPointError(5,2.500000e-02,2.500000e-02,2.970000e-03,2.980000e-03);
    lhcf_data_bin2->SetPoint(6,3.250000e-01,1.770000e-02); lhcf_data_bin2->SetPointError(6,2.500000e-02,2.500000e-02,2.290000e-03,2.290000e-03);
    lhcf_data_bin2->SetPoint(7,3.750000e-01,8.070000e-03); lhcf_data_bin2->SetPointError(7,2.500000e-02,2.500000e-02,1.490000e-03,1.490000e-03);
    lhcf_data_bin2->SetPoint(8,4.500000e-01,1.350000e-03); lhcf_data_bin2->SetPointError(8,5.000000e-02,5.000000e-02,3.710000e-04,3.720000e-04);
    lhcf_data_bin2->SetPoint(9,5.500000e-01,1.470000e-04); lhcf_data_bin2->SetPointError(9,5.000000e-02,5.000000e-02,9.170000e-05,9.180000e-05);
    lhcf_data_bin2->Write();
    
    lhcf_data_bin3->SetPoint(0,2.500000e-02,2.030000e-01); lhcf_data_bin3->SetPointError(0,2.500000e-02,2.500000e-02,3.090000e-02,3.090000e-02);
    lhcf_data_bin3->SetPoint(1,7.500000e-02,1.730000e-01); lhcf_data_bin3->SetPointError(1,2.500000e-02,2.500000e-02,1.640000e-02,1.640000e-02);
    lhcf_data_bin3->SetPoint(2,1.250000e-01,1.070000e-01); lhcf_data_bin3->SetPointError(2,2.500000e-02,2.500000e-02,4.610000e-03,4.600000e-03);
    lhcf_data_bin3->SetPoint(3,1.750000e-01,6.300000e-02); lhcf_data_bin3->SetPointError(3,2.500000e-02,2.500000e-02,2.450000e-03,2.450000e-03);
    lhcf_data_bin3->SetPoint(4,2.250000e-01,3.200000e-02); lhcf_data_bin3->SetPointError(4,2.500000e-02,2.500000e-02,2.630000e-03,2.640000e-03);
    lhcf_data_bin3->SetPoint(5,2.750000e-01,1.450000e-02); lhcf_data_bin3->SetPointError(5,2.500000e-02,2.500000e-02,2.140000e-03,2.150000e-03);
    lhcf_data_bin3->SetPoint(6,3.250000e-01,3.640000e-03); lhcf_data_bin3->SetPointError(6,2.500000e-02,2.500000e-02,9.280000e-04,9.290000e-04);
    lhcf_data_bin3->SetPoint(7,3.750000e-01,1.540000e-03); lhcf_data_bin3->SetPointError(7,2.500000e-02,2.500000e-02,6.200000e-04,6.210000e-04);
    lhcf_data_bin3->SetPoint(8,4.500000e-01,5.430000e-05); lhcf_data_bin3->SetPointError(8,5.000000e-02,5.000000e-02,6.410000e-05,6.410000e-05);
    lhcf_data_bin3->Write();

    lhcf_data_bin4->SetPoint(0,2.500000e-02,1.200000e-01); lhcf_data_bin4->SetPointError(0,2.500000e-02,2.500000e-02,9.660000e-03,9.680000e-03);
    lhcf_data_bin4->SetPoint(1,7.500000e-02,8.280000e-02); lhcf_data_bin4->SetPointError(1,2.500000e-02,2.500000e-02,2.890000e-03,2.900000e-03);
    lhcf_data_bin4->SetPoint(2,1.250000e-01,4.490000e-02); lhcf_data_bin4->SetPointError(2,2.500000e-02,2.500000e-02,1.880000e-03,1.880000e-03);
    lhcf_data_bin4->SetPoint(3,1.750000e-01,2.100000e-02); lhcf_data_bin4->SetPointError(3,2.500000e-02,2.500000e-02,1.280000e-03,1.280000e-03);
    lhcf_data_bin4->SetPoint(4,2.250000e-01,7.430000e-03); lhcf_data_bin4->SetPointError(4,2.500000e-02,2.500000e-02,9.730000e-04,9.760000e-04);
    lhcf_data_bin4->SetPoint(5,2.750000e-01,1.840000e-03); lhcf_data_bin4->SetPointError(5,2.500000e-02,2.500000e-02,5.150000e-04,5.160000e-04);
    lhcf_data_bin4->SetPoint(6,3.500000e-01,2.170000e-04); lhcf_data_bin4->SetPointError(6,5.000000e-02,5.000000e-02,1.330000e-04,1.330000e-04);
    lhcf_data_bin4->Write();

    lhcf_data_bin5->SetPoint(0,2.500000e-02,1.280000e-02); lhcf_data_bin5->SetPointError(0,2.500000e-02,2.500000e-02,1.420000e-03,1.420000e-03);
    lhcf_data_bin5->SetPoint(1,7.500000e-02,7.550000e-03); lhcf_data_bin5->SetPointError(1,2.500000e-02,2.500000e-02,8.880000e-04,8.850000e-04);
    lhcf_data_bin5->SetPoint(2,1.250000e-01,2.370000e-03); lhcf_data_bin5->SetPointError(2,2.500000e-02,2.500000e-02,3.770000e-04,3.760000e-04);
    lhcf_data_bin5->SetPoint(3,1.750000e-01,1.910000e-04); lhcf_data_bin5->SetPointError(3,2.500000e-02,2.500000e-02,6.990000e-05,6.980000e-05);
    lhcf_data_bin5->SetPoint(4,2.500000e-01,8.370000e-06); lhcf_data_bin5->SetPointError(4,5.000000e-02,5.000000e-02,1.050000e-05,1.050000e-05);
    lhcf_data_bin5->Write();
    
    TGraphErrors* castor_data = new TGraphErrors(3); castor_data->SetName("castor_data");//read off by eye 
    castor_data->SetPoint(0,1,0.475); castor_data->SetPointError(0,0,0.03);
    castor_data->SetPoint(1,2,1.000); castor_data->SetPointError(1,0,0.00);
    castor_data->SetPoint(2,3,1.640); castor_data->SetPointError(2,0,0.06);
    castor_data->Write();
  }
  ~DataManager()
  {
   for(std::map<std::string,HistoManager*>::iterator i = modelsHisto.begin(); i != modelsHisto.end(); ++i)
      {
	i->second->FinaliseHistos();			   
      }

    std::cout << "DataManager::Writing output file" << std::endl;
    theOutFile->Write();
    std::cout << "DataManager::Closing output file" << std::endl;
    theOutFile->Close();

    delete theOutFile;
    if(ascii_in) delete ascii_in;
  }

  bool GetNextFile()
  {
    if(++current_file >= int(current_model->second.size()))
      {
	current_file = 0;
	++current_model;
	if(current_model == models.end()) return 0;
      }


    std::string filename = current_model->second[current_file];
    if((filename).find("7TeV") != std::string::npos) energy = "7TeV";
    else if((filename).find("2760GeV") != std::string::npos) energy = "2760GeV";
    else if((filename).find("900GeV") != std::string::npos) energy = "900GeV";
    else throw std::runtime_error ("filename need to contain energy");
    bool use_compression = false;
    if((filename).find(".gz") != std::string::npos) use_compression = true;
    std::cout << '\033' << "[1A" << "! Opening file " << (filename) << " with" << (use_compression?" ":"out ") << "compression. Energy is " << energy << "." << std::endl;

    in.reset();
    if(use_compression)
      in.push (boost::iostreams::gzip_decompressor ());
    in.push (boost::iostreams::file_descriptor_source (filename));
    if(ascii_in) delete ascii_in;
    ascii_in = new HepMC::IO_GenEvent(in);
    return true;
  }

 
  bool GetNextEvent()
  {
    delete evt;
    evt = 0;
    try 
      {
        (*ascii_in) >> evt;
      }
    catch (std::exception& e)
      {
        evt = false;
        std::cout << "Event could not be read: " << e.what() << std::endl;
      }
    if (!evt)
      {
        std::cout << " read event empty getting next file" << std::endl;
	if (!GetNextFile()) return false;
	else // new file selected
	  {
            delete evt;
	    evt = NULL;
	    try
              {
                (*ascii_in) >> evt;
              }
            catch(std::exception& e)
              {
                evt = false;
                std::cout << "Event could not be read again: " << e.what() << std::endl;
              }
	    if (!evt)  return false;
	  }
      }

    theProgressBar.SetStatus(count / double(2.52e6));
    theProgressBar.Print();
    //std::cout << count << std::endl;

    modelsHisto[current_model->first]->AddEvent(energy);
    count++;
    return true;
  }
  void FillLHCF(double y, double pt)
  {
    if(energy == "7TeV") modelsHisto[current_model->first]->FillLHCF(y,pt);
    return;
  }
  void FillCASTOR(double e)
  {
    modelsHisto[current_model->first]->FillCASTOR(e,energy);
    return;
  }
  void FillCASTORcomposition(double castor_e, double castor_e_pi0, double castor_e_pic, double castor_e_bar, double castor_e_gam, double castor_e_oth)
  {
    if(energy == "7TeV") modelsHisto[current_model->first]->FillCASTORcomposition(castor_e, castor_e_pi0, castor_e_pic, castor_e_bar, castor_e_gam, castor_e_oth);
  }

  std::string energy;

};

#endif
