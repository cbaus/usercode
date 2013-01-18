//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 18 14:12:20 2013 by ROOT version 5.32/00
// from TTree RHTree/RHTree
// found on file: gen_out.root
//////////////////////////////////////////////////////////

#ifndef RHTree_h
#define RHTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

TH1D* h_CAS_energy_module = new TH1D("h_Cas_energy_module","h_Cas_energy_module;module;#sumE in GeV",14,1,15);
TH1D* h_ZDC_energy_module = new TH1D("h_ZDC_energy_module","h_Cas_energy_module;module;#SumE in GeV",14,1,15);
TH1D* h_ZDC_pulse = new TH1D("h_ZDC_pulse","h_Cas_energy_module;TS;#SumE in GeV",10,0,10);

class RHTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          nbCasRecHits;
   Double_t        casRecHitEnergy[224];   //[nbCasRecHits]
   Int_t           casRecHitIphi[224];   //[nbCasRecHits]
   Int_t           casRecHitIdepth[224];   //[nbCasRecHits]
   Int_t           casRecHitSaturation[224];   //[nbCasRecHits]
   UInt_t          nbZDCRecHits;
   Double_t        zdcRecHitEnergy[18];   //[nbZDCRecHits]
   Int_t           zdcRecHitIside[18];   //[nbZDCRecHits]
   Int_t           zdcRecHitIsection[18];   //[nbZDCRecHits]
   Int_t           zdcRecHitIchannel[18];   //[nbZDCRecHits]
   Int_t           zdcRecHitSaturation[18];   //[nbZDCRecHits]
   UInt_t          nbZDCDigis;
   Int_t           zdcDigiIside[18];   //[nbZDCDigis]
   Int_t           zdcDigiIsection[18];   //[nbZDCDigis]
   Int_t           zdcDigiIchannel[18];   //[nbZDCDigis]
   Double_t        zdcDigiEnergyFC[18][10];   //[nbZDCDigis]
   Int_t           zdcDigiEnergyADC[18][10];   //[nbZDCDigis]

   // List of branches
   TBranch        *b_nbCasRecHits;   //!
   TBranch        *b_casRecHitEnergy;   //!
   TBranch        *b_casRecHitIphi;   //!
   TBranch        *b_casRecHitIdepth;   //!
   TBranch        *b_casRecHitSaturation;   //!
   TBranch        *b_nbZDCRecHits;   //!
   TBranch        *b_zdcRecHitEnergy;   //!
   TBranch        *b_zdcRecHitIside;   //!
   TBranch        *b_zdcRecHitIsection;   //!
   TBranch        *b_zdcRecHitIchannel;   //!
   TBranch        *b_zdcRecHitSaturation;   //!
   TBranch        *b_nbZDCDigis;   //!
   TBranch        *b_zdcDigiIside;   //!
   TBranch        *b_zdcDigiIsection;   //!
   TBranch        *b_zdcDigiIchannel;   //!
   TBranch        *b_zdcDigiEnergyFC;   //!
   TBranch        *b_zdcDigiEnergyADC;   //!

   RHTree(TTree *tree=0);
   virtual ~RHTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef RHTree_cxx
RHTree::RHTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("gen_out.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("gen_out.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("gen_out.root:/demo");
      dir->GetObject("RHTree",tree);

   }
   Init(tree);
}

RHTree::~RHTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RHTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RHTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RHTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nbCasRecHits", &nbCasRecHits, &b_nbCasRecHits);
   fChain->SetBranchAddress("casRecHitEnergy", casRecHitEnergy, &b_casRecHitEnergy);
   fChain->SetBranchAddress("casRecHitIphi", casRecHitIphi, &b_casRecHitIphi);
   fChain->SetBranchAddress("casRecHitIdepth", casRecHitIdepth, &b_casRecHitIdepth);
   fChain->SetBranchAddress("casRecHitSaturation", casRecHitSaturation, &b_casRecHitSaturation);
   fChain->SetBranchAddress("nbZDCRecHits", &nbZDCRecHits, &b_nbZDCRecHits);
   fChain->SetBranchAddress("zdcRecHitEnergy", zdcRecHitEnergy, &b_zdcRecHitEnergy);
   fChain->SetBranchAddress("zdcRecHitIside", zdcRecHitIside, &b_zdcRecHitIside);
   fChain->SetBranchAddress("zdcRecHitIsection", zdcRecHitIsection, &b_zdcRecHitIsection);
   fChain->SetBranchAddress("zdcRecHitIchannel", zdcRecHitIchannel, &b_zdcRecHitIchannel);
   fChain->SetBranchAddress("zdcRecHitSaturation", zdcRecHitSaturation, &b_zdcRecHitSaturation);
   fChain->SetBranchAddress("nbZDCDigis", &nbZDCDigis, &b_nbZDCDigis);
   fChain->SetBranchAddress("zdcDigiIside", zdcDigiIside, &b_zdcDigiIside);
   fChain->SetBranchAddress("zdcDigiIsection", zdcDigiIsection, &b_zdcDigiIsection);
   fChain->SetBranchAddress("zdcDigiIchannel", zdcDigiIchannel, &b_zdcDigiIchannel);
   fChain->SetBranchAddress("zdcDigiEnergyFC", zdcDigiEnergyFC, &b_zdcDigiEnergyFC);
   fChain->SetBranchAddress("zdcDigiEnergyADC", zdcDigiEnergyADC, &b_zdcDigiEnergyADC);
   Notify();
}

Bool_t RHTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RHTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RHTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef RHTree_cxx
