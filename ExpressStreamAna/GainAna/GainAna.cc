#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "iostream"

using namespace std;

void GainAna()
{
  TFile* data_file = NULL;
  data_file = TFile::Open("root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/PA2013_HiForest_Express_r210658_autoforest_v51.root");
  if (!data_file) cerr << "Cannot find data file" << endl;
  TTree* data_tree = NULL; 
  ((TDirectory*)data_file->Get("rechitanalyzer"))->GetObject("castor",data_tree);
  if (!data_tree) cerr << "Cannot find data tree" << endl;
  
  TFile* mc_file = NULL;
  mc_file = TFile::Open("root://eoscms//eos/cms/store/group/phys_heavyions/azsigmon/HiForest_pPb_Epos_336800.root");
  if (!mc_file) cerr << "Cannot find mc file" << endl;
  TTree* mc_tree = NULL; 
  ((TDirectory*)mc_file->Get("rechitanalyzer"))->GetObject("castor",mc_tree);
  if (!mc_tree) cerr << "Cannot find mc tree" << endl;
  
  const int n_cas_rechits = 224;
  float mc_cas_e[n_cas_rechits];
  int mc_cas_sat[n_cas_rechits];
  int mc_cas_depth[n_cas_rechits];
  int mc_cas_iphi[n_cas_rechits];

  float data_cas_e[n_cas_rechits];
  int data_cas_sat[n_cas_rechits];
  int data_cas_depth[n_cas_rechits];
  int data_cas_iphi[n_cas_rechits];

  mc_tree->SetBranchAddress("e",mc_cas_e);
  mc_tree->SetBranchAddress("saturation",mc_cas_sat);
  mc_tree->SetBranchAddress("depth",mc_cas_depth);
  mc_tree->SetBranchAddress("iphi",mc_cas_iphi);

  data_tree->SetBranchAddress("e",data_cas_e);
  data_tree->SetBranchAddress("saturation",data_cas_sat);
  data_tree->SetBranchAddress("depth",data_cas_depth);
  data_tree->SetBranchAddress("iphi",data_cas_iphi);

  TFile* out_file = new TFile("histos.root","RECREATE");

  TH2D* mc_h_cas_e              = new TH2D("mc_h_cas_e","",14,0.5,14.5,16,0.5,16.5);
  TH2D* mc_h_cas_n              = new TH2D("mc_h_cas_n","",14,0.5,14.5,16,0.5,16.5);
  TH2D* data_h_cas_e            = new TH2D("data_h_cas_e","",14,0.5,14.5,16,0.5,16.5);
  TH2D* data_h_cas_n            = new TH2D("data_h_cas_n","",14,0.5,14.5,16,0.5,16.5);

  TH1D* mc_h_cas_e_dist         = new TH1D("mc_h_cas_e_dist","",100,-50,500);
  TH1D* data_h_cas_e_dist       = new TH1D("data_h_cas_e_dist","",100,-50,500);
  
  //**********************************LOOP*******************

  for(int i=0; i<mc_tree->GetEntries(); i++)
    {
      if(i==-1)break;
      if(i % 10000 == 0) cout << "MC Entry: " << i << " / " << mc_tree->GetEntries() << endl;
      mc_tree->GetEntry();
      for(int ch=0; ch<224; ch++)
        {
          if(mc_cas_e[ch] > 0.5)
            {
              mc_h_cas_n->Fill(mc_cas_depth[ch],mc_cas_iphi[ch]);
              mc_h_cas_e->Fill(mc_cas_depth[ch],mc_cas_iphi[ch],mc_cas_e[ch]);
            }
          if(mc_cas_depth[ch] < 6)
            mc_h_cas_e_dist->Fill(mc_cas_e[ch]);
        }
    }
  for(int i=0; i<data_tree->GetEntries(); i++)
    {
      if(i % 10000 == 0) cout << "DATA Entry: " << i << " / " << data_tree->GetEntries() << endl;
      data_tree->GetEntry();
      for(int ch=0; ch<224; ch++)
        {
          if(data_cas_e[ch] > 0.5)
            {
              data_h_cas_n->Fill(data_cas_depth[ch],data_cas_iphi[ch]);
              data_h_cas_e->Fill(data_cas_depth[ch],data_cas_iphi[ch],data_cas_e[ch]);
            }
          if(data_cas_depth[ch] < 6)
            data_h_cas_e_dist->Fill(data_cas_e[ch]);
        }
    }

  //******************After LOOP*******************

  mc_h_cas_e->Divide(mc_h_cas_n);
  data_h_cas_e->Divide(data_h_cas_n);
  mc_h_cas_e->Divide(data_h_cas_e);

  mc_h_cas_e_dist->Scale(1./mc_h_cas_e_dist->GetEntries());
  data_h_cas_e_dist->Scale(1./data_h_cas_e_dist->GetEntries());

  //******************END************************

  out_file->Save();
}
