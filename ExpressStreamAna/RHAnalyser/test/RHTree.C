#define RHTree_cxx
#include "RHTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <vector>
#include <sstream>

TCanvas* cZDC = new TCanvas;
std::vector<TH1D*> vec_h_zdc_pulse;

TH1D* h_CAS_energy_module = new TH1D("h_Cas_energy_module","h_Cas_energy_module;module;#sumE in GeV",14,1,15);
TH1D* h_ZDC_energy_module = new TH1D("h_ZDC_energy_module","h_Cas_energy_module;module;#SumE in GeV",14,1,15);
TH1D* h_ZDC_pulse = new TH1D("h_ZDC_pulse","h_Cas_energy_module;TS;#SumE in GeV",10,0,10);

void RHTree::Begin()
{

cZDC->Divide(4);
for (int i=0; i<18; i++)
  {
    std::ostringstream name;
    std::ostringstream title;
    name << "h_ZDC_pulse_" << i;
    title << "Digi Number " << i << " ;TS;FC";
    vec_h_zdc_pulse.push_back(new TH1D(name.str().c_str(),title.str().c_str(),10,0,10));
  }
}

void RHTree::End()
{
  for(int i=0; i<4; i++)
    {
      cZDC->cd(i+1);
      vec_h_zdc_pulse[i]->Scale(10./vec_h_zdc_pulse[i]->GetEntries());
      vec_h_zdc_pulse[i]->Draw();
    }
  cZDC->SaveAs("c.root");
}

void RHTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L RHTree.C
//      Root > RHTree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      //************CASTOR************
      double e_mod[14];
      for(int i=0; i<nbCasRecHits; i++)
        {
          h_CAS_energy_module->Fill(casRecHitIdepth[i], casRecHitEnergy[i]);
        }

      //************ZDC*************
      for(int digi = 0; digi < nbZDCDigis; digi++)
        {
          for(int ts=0; ts<10; ts++)
            {
              std::cout << ts << " " << zdcDigiIchannel[0] << " " << zdcDigiEnergyFC[zdcDigiIchannel[0]][ts] << std::endl; 
              double energy = zdcDigiEnergyFC[digi][ts];
              vec_h_zdc_pulse[digi]->Fill(ts, energy);
            }
        }
   }
}
