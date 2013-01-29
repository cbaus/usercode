#define MAXEVT 5000

#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "iostream"

using namespace std;

void GainAna()
{
  //**********************************************INPUT******************************
  TFile* data_file = NULL; //****FILE
  data_file = TFile::Open("root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/PA2013_HiForest_Express_r210658_autoforest_v51.root");
  if (!data_file) cerr << "Cannot find data file" << endl;
  TTree* data_tree = NULL; //****TREE1
  ((TDirectory*)data_file->Get("rechitanalyzer"))->GetObject("castor",data_tree);
  if (!data_tree) cerr << "Cannot find data tree" << endl;
  TTree* data_friend_tree1 = NULL; //****TREE2
  ((TDirectory*)data_file->Get("hltanalysis"))->GetObject("HltTree",data_friend_tree1);
  if (!data_friend_tree1) cerr << "Cannot find data friend tree" << endl;
  data_tree->AddFriend(data_friend_tree1);
  TTree* data_friend_tree2 = NULL; //****TREE3
  ((TDirectory*)data_file->Get("pixelTrack"))->GetObject("trackTree",data_friend_tree2);
  if (!data_friend_tree2) cerr << "Cannot find data friend tree" << endl;
  data_tree->AddFriend(data_friend_tree2);

  TFile* mc_file = NULL; //****FILE
  mc_file = TFile::Open("root://eoscms//eos/cms//store/caf/user/dgulhan/pPb_Hijing_MB/HiForest_v03_mergedv02/merged_forest_0.root");
  if (!mc_file) cerr << "Cannot find mc file" << endl;
  TTree* mc_tree = NULL; //****TREE1
  ((TDirectory*)mc_file->Get("rechitanalyzer"))->GetObject("castor",mc_tree);
  if (!mc_tree) cerr << "Cannot find mc tree" << endl;
  TTree* mc_friend_tree1 = NULL; //****TREE2
  ((TDirectory*)mc_file->Get("hltanalysis"))->GetObject("HltTree",mc_friend_tree1);
  if (!mc_friend_tree1) cerr << "Cannot find mc friend tree" << endl;
  mc_tree->AddFriend(mc_friend_tree1);
  TTree* mc_friend_tree2 = NULL; //****TREE3
  ((TDirectory*)mc_file->Get("pixelTrack"))->GetObject("trackTree",mc_friend_tree2);
  if (!mc_friend_tree2) cerr << "Cannot find mc friend tree" << endl;
  mc_tree->AddFriend(mc_friend_tree2);

  const int n_cas_rechits = 224;
  float mc_cas_e[n_cas_rechits];
  int mc_cas_sat[n_cas_rechits];
  int mc_cas_depth[n_cas_rechits];
  int mc_cas_iphi[n_cas_rechits];

  int mc_nTrk;

  int mc_HLT_PAMinBiasHF_v1;
  int mc_algo_99;

  float data_cas_e[n_cas_rechits];
  int data_cas_sat[n_cas_rechits];
  int data_cas_depth[n_cas_rechits];
  int data_cas_iphi[n_cas_rechits];

  int data_nTrk;

  int data_HLT_PAMinBiasHF_v1;
  int data_algo_99;

  mc_tree->SetBranchAddress("e",mc_cas_e);
  mc_tree->SetBranchAddress("saturation",mc_cas_sat);
  mc_tree->SetBranchAddress("depth",mc_cas_depth);
  mc_tree->SetBranchAddress("iphi",mc_cas_iphi);

  mc_tree->SetBranchAddress("nTrk",mc_nTrk);

  mc_tree->SetBranchAddress("HLT_PAMinBiasHF_v1",&mc_HLT_PAMinBiasHF_v1);
  mc_tree->SetBranchAddress("L1_CastorEm_TotemLowMultiplicity",&mc_algo_99);


  data_tree->SetBranchAddress("e",data_cas_e);
  data_tree->SetBranchAddress("saturation",data_cas_sat);
  data_tree->SetBranchAddress("depth",data_cas_depth);
  data_tree->SetBranchAddress("iphi",data_cas_iphi);

  data_tree->SetBranchAddress("nTrk",data_nTrk);

  data_tree->SetBranchAddress("HLT_PAMinBiasHF_v1",&data_HLT_PAMinBiasHF_v1);
  data_tree->SetBranchAddress("L1_CastorEm_TotemLowMultiplicity",&data_algo_99);

  //*********************************OUTPUT************************+++

  TFile* out_file = new TFile("histos.root","RECREATE");

  TH2D* mc_h_cas_e              = new TH2D("mc_h_cas_e","",14,0.5,14.5,16,0.5,16.5);
  TH2D* mc_h_cas_n              = new TH2D("mc_h_cas_n","",14,0.5,14.5,16,0.5,16.5);
  TH2D* data_h_cas_e            = new TH2D("data_h_cas_e","",14,0.5,14.5,16,0.5,16.5);
  TH2D* data_h_cas_n            = new TH2D("data_h_cas_n","",14,0.5,14.5,16,0.5,16.5);

  TH2D* div_h_cas_e             ;

  TH1D* mc_h_cas_e_dist         = new TH1D("mc_h_cas_e_dist","",100,-50,500);
  TH1D* data_h_cas_e_dist       = new TH1D("data_h_cas_e_dist","",100,-50,500);

  TH1D* mc_h_hlt                = new TH1D("mc_h_hlt","",10,-0.5,9.5);
  TH1D* data_h_hlt              = new TH1D("data_h_hlt","",10,-0.5,9.5);

  TH1D* mc_nTrk                 = new TH1D("mc_nTrk",";pixel tracks",30,0,200);
  TH1D* data_nTrk               = new TH1D("data_nTrk",";pixel tracks",30,0,200);

  TH1D* mc_nTrk_99              = new TH1D("mc_nTrk_99",";pixel tracks",30,0,200);
  TH1D* data_nTrk_99            = new TH1D("data_nTrk_99",";pixel tracks",30,0,200);
  TH1D* mc_had_em_ratio_99      = new TH1D("mc_had_em_ratio_99",";Castor EM/(HAD+EM)",30,0,1);
  TH1D* data_had_em_ratio_99    = new TH1D("data_had_em_ratio_99",";Castor EM/(HAD+EM)",30,0,1);

  //**********************************LOOP*******************

  for(int i=0; i<mc_tree->GetEntries(); i++)
    {
      if(i==MAXEVT) break;
      if(i % 10000 == 0) cout << "MC Entry: " << i << " / " << mc_tree->GetEntries() << endl;
      mc_tree->GetEntry(i);

      mc_h_hlt->Fill(mc_HLT_PAMinBiasHF_v1);

      double mc_sum_cas_e_em = 0;
      double mc_sum_cas_e_had = 0;
      for(int ch=0; ch<224; ch++)
        {
          //cout << "mc " << mc_cas_depth[ch] << "," << mc_cas_iphi[ch] << ": E=" << mc_cas_e[ch] << endl;
          mc_h_cas_n->Fill(mc_cas_depth[ch],mc_cas_iphi[ch]);
          mc_h_cas_e->Fill(mc_cas_depth[ch],mc_cas_iphi[ch],mc_cas_e[ch]);
          if(mc_cas_depth[ch] < 6)
            mc_h_cas_e_dist->Fill(mc_cas_e[ch]);
          if(mc_cas_depth[ch] < 3)
              mc_sum_cas_e_em += mc_cas_e[ch];
          if(mc_cas_depth[ch] >= 3)
              mc_sum_cas_e_had += mc_cas_e[ch];
        }
      //trigger
      if(mc_algo_99)
      {
        mc_had_em_ratio_99->Fill(mc_sum_cas_e_em/(mc_sum_cas_e_em+mc_sum_cas_e_had));
        mc_nTrk_99->Fill(mc_nTrk);
      }
      mc_nTrk->Fill(mc_nTrk);
    }
  for(int i=0; i<data_tree->GetEntries(); i++)
    {
      if(i==MAXEVT) break;
      if(i % 10000 == 0) cout << "DATA Entry: " << i << " / " << data_tree->GetEntries() << endl;
      data_tree->GetEntry(i);

      data_h_hlt->Fill(data_HLT_PAMinBiasHF_v1);

      double data_sum_cas_e_em = 0;
      double data_sum_cas_e_had = 0;
      for(int ch=0; ch<224; ch++)
        {
          data_h_cas_n->Fill(data_cas_depth[ch],data_cas_iphi[ch]);
          data_h_cas_e->Fill(data_cas_depth[ch],data_cas_iphi[ch],data_cas_e[ch]);
          if(data_cas_depth[ch] < 6)
            data_h_cas_e_dist->Fill(data_cas_e[ch]);
          if(data_cas_depth[ch] < 3)
              data_sum_cas_e_em += data_cas_e[ch];
          if(data_cas_depth[ch] >= 3)
              data_sum_cas_e_had += data_cas_e[ch];
        }
      //trigger
      if(data_algo_99)
      {
        data_had_em_ratio_99->Fill(data_sum_cas_e_em/(data_sum_cas_e_em+data_sum_cas_e_had));
        data_nTrk_99->Fill(data_nTrk);
      }
      data_nTrk->Fill(data_nTrk);
    }

  //******************After LOOP*******************

  mc_h_cas_e->Divide(mc_h_cas_n);
  data_h_cas_e->Divide(data_h_cas_n);
  mc_h_cas_e->Scale(1./mc_h_cas_e->GetBinContent(mc_h_cas_e->GetBin(1,8)));
  data_h_cas_e->Scale(1./data_h_cas_e->GetBinContent(data_h_cas_e->GetBin(1,8)));

  div_h_cas_e = new TH2D(*mc_h_cas_e);
  div_h_cas_e->SetName("div_h_cas_e");
  div_h_cas_e->Divide(data_h_cas_e);

  mc_h_cas_e_dist->Scale(1./mc_h_cas_e_dist->GetEntries());
  data_h_cas_e_dist->Scale(1./data_h_cas_e_dist->GetEntries());

  mc_nTrk->Scale(1./mc_nTrk->GetEntries());
  data_nTrk->Scale(1./data_nTrk->GetEntries());
  mc_nTrk_99->Scale(1./mc_nTrk_99->GetEntries());
  data_nTrk_99->Scale(1./data_nTrk_99->GetEntries());

  mc_had_em_ratio_99->Scale(1./mc_had_em_ratio_99->GetEntries());
  data_had_em_ratio_99->Scale(1./data_had_em_ratio_99->GetEntries());

  //******************END************************

  out_file->Write();
  out_file->Save();
}
