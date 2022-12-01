//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Aug 31 15:12:12 2022 by ROOT version 6.24/06
// from TTree tree/
// found on file: /eos/user/g/gkopp/QIEscans/HcalTupleMaker_QIE11_TP_13TeV_QIEphasescan_relNominal_run356815_1.root
//////////////////////////////////////////////////////////

#ifndef HBStudy_plotByQIE_h
#define HBStudy_plotByQIE_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class HBStudy_plotByQIE {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   std::vector<double>  *QIE11DigiTimeFC;
   std::vector<double>  *QIE11DigiTotFC;
   std::vector<std::vector<double> > *QIE10DigiFC;
   std::vector<std::vector<double> > *QIE11DigiFC;
   std::vector<float>   *QIE11DigiTimeTDC;
   Int_t           laserType;
   std::vector<int>     *QIE10DigiDepth;
   std::vector<int>     *QIE10DigiFlags;
   std::vector<int>     *QIE10DigiHFDepth;
   std::vector<int>     *QIE10DigiIEta;
   std::vector<int>     *QIE10DigiIPhi;
   std::vector<int>     *QIE10DigiLinkError;
   std::vector<int>     *QIE10DigiRawID;
   std::vector<int>     *QIE10DigiSubdet;
   std::vector<int>     *QIE11DigiCapIDError;
   std::vector<int>     *QIE11DigiDepth;
   std::vector<int>     *QIE11DigiFlags;
   std::vector<int>     *QIE11DigiIEta;
   std::vector<int>     *QIE11DigiIPhi;
   std::vector<int>     *QIE11DigiLinkError;
   std::vector<int>     *QIE11DigiNTDC;
   std::vector<int>     *QIE11DigiRawID;
   std::vector<int>     *QIE11DigiSubdet;
   std::vector<int>     *HcalTriggerPrimitiveCompressedEtSOI;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI0;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI1;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI2;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI3;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI4;
   std::vector<int>     *HcalTriggerPrimitiveFineGrainSOI5;
   std::vector<int>     *HcalTriggerPrimitiveIEta;
   std::vector<int>     *HcalTriggerPrimitiveIPhi;
   std::vector<int>     *HcalTriggerPrimitivePresamples;
   std::vector<int>     *HcalTriggerPrimitiveSize;
   std::vector<std::vector<int> > *QIE10DigiADC;
   std::vector<std::vector<int> > *QIE10DigiCapID;
   std::vector<std::vector<int> > *QIE10DigiLETDC;
   std::vector<std::vector<int> > *QIE10DigiOK;
   std::vector<std::vector<int> > *QIE10DigiSOI;
   std::vector<std::vector<int> > *QIE10DigiTETDC;
   std::vector<std::vector<int> > *QIE11DigiADC;
   std::vector<std::vector<int> > *QIE11DigiCapID;
   std::vector<std::vector<int> > *QIE11DigiSOI;
   std::vector<std::vector<int> > *QIE11DigiTDC;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveCompressedEt;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain0;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain1;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain2;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain3;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain4;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveFineGrain5;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveHBHEDigiIndex;
   std::vector<std::vector<int> > *HcalTriggerPrimitiveHFDigiIndex;
   UInt_t          bx;
   UInt_t          event;
   UInt_t          ls;
   UInt_t          orbit;
   UInt_t          run;

   // List of branches
   TBranch        *b_QIE11DigiTimeFC;   //!
   TBranch        *b_QIE11DigiTotFC;   //!
   TBranch        *b_QIE10DigiFC;   //!
   TBranch        *b_QIE11DigiFC;   //!
   TBranch        *b_QIE11DigiTimeTDC;   //!
   TBranch        *b_laserType;   //!
   TBranch        *b_QIE10DigiDepth;   //!
   TBranch        *b_QIE10DigiFlags;   //!
   TBranch        *b_QIE10DigiHFDepth;   //!
   TBranch        *b_QIE10DigiIEta;   //!
   TBranch        *b_QIE10DigiIPhi;   //!
   TBranch        *b_QIE10DigiLinkError;   //!
   TBranch        *b_QIE10DigiRawID;   //!
   TBranch        *b_QIE10DigiSubdet;   //!
   TBranch        *b_QIE11DigiCapIDError;   //!
   TBranch        *b_QIE11DigiDepth;   //!
   TBranch        *b_QIE11DigiFlags;   //!
   TBranch        *b_QIE11DigiIEta;   //!
   TBranch        *b_QIE11DigiIPhi;   //!
   TBranch        *b_QIE11DigiLinkError;   //!
   TBranch        *b_QIE11DigiNTDC;   //!
   TBranch        *b_QIE11DigiRawID;   //!
   TBranch        *b_QIE11DigiSubdet;   //!
   TBranch        *b_HcalTriggerPrimitiveCompressedEtSOI;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI0;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI1;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI2;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI3;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI4;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrainSOI5;   //!
   TBranch        *b_HcalTriggerPrimitiveIEta;   //!
   TBranch        *b_HcalTriggerPrimitiveIPhi;   //!
   TBranch        *b_HcalTriggerPrimitivePresamples;   //!
   TBranch        *b_HcalTriggerPrimitiveSize;   //!
   TBranch        *b_QIE10DigiADC;   //!
   TBranch        *b_QIE10DigiCapID;   //!
   TBranch        *b_QIE10DigiLETDC;   //!
   TBranch        *b_QIE10DigiOK;   //!
   TBranch        *b_QIE10DigiSOI;   //!
   TBranch        *b_QIE10DigiTETDC;   //!
   TBranch        *b_QIE11DigiADC;   //!
   TBranch        *b_QIE11DigiCapID;   //!
   TBranch        *b_QIE11DigiSOI;   //!
   TBranch        *b_QIE11DigiTDC;   //!
   TBranch        *b_HcalTriggerPrimitiveCompressedEt;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain0;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain1;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain2;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain3;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain4;   //!
   TBranch        *b_HcalTriggerPrimitiveFineGrain5;   //!
   TBranch        *b_HcalTriggerPrimitiveHBHEDigiIndex;   //!
   TBranch        *b_HcalTriggerPrimitiveHFDigiIndex;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_event;   //!
   TBranch        *b_ls;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_run;   //!

   HBStudy_plotByQIE(TTree *tree=0);
   virtual ~HBStudy_plotByQIE();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef HBStudy_plotByQIE_cxx
HBStudy_plotByQIE::HBStudy_plotByQIE(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/user/s/shoienko/HcalTupleMaker_QIE11_TP_13TeV_QIEphasescan_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/user/s/shoienko/HcalTupleMaker_QIE11_TP_13TeV_QIEphasescan_0.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/user/s/shoienko/HcalTupleMaker_QIE11_TP_13TeV_QIEphasescan_0.root:/hcalTupleTree");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

HBStudy_plotByQIE::~HBStudy_plotByQIE()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HBStudy_plotByQIE::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HBStudy_plotByQIE::LoadTree(Long64_t entry)
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

void HBStudy_plotByQIE::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   QIE11DigiTimeFC = 0;
   QIE11DigiTotFC = 0;
   QIE10DigiFC = 0;
   QIE11DigiFC = 0;
   QIE11DigiTimeTDC = 0;
   QIE10DigiDepth = 0;
   QIE10DigiFlags = 0;
   QIE10DigiHFDepth = 0;
   QIE10DigiIEta = 0;
   QIE10DigiIPhi = 0;
   QIE10DigiLinkError = 0;
   QIE10DigiRawID = 0;
   QIE10DigiSubdet = 0;
   QIE11DigiCapIDError = 0;
   QIE11DigiDepth = 0;
   QIE11DigiFlags = 0;
   QIE11DigiIEta = 0;
   QIE11DigiIPhi = 0;
   QIE11DigiLinkError = 0;
   QIE11DigiNTDC = 0;
   QIE11DigiRawID = 0;
   QIE11DigiSubdet = 0;
   HcalTriggerPrimitiveCompressedEtSOI = 0;
   HcalTriggerPrimitiveFineGrainSOI0 = 0;
   HcalTriggerPrimitiveFineGrainSOI1 = 0;
   HcalTriggerPrimitiveFineGrainSOI2 = 0;
   HcalTriggerPrimitiveFineGrainSOI3 = 0;
   HcalTriggerPrimitiveFineGrainSOI4 = 0;
   HcalTriggerPrimitiveFineGrainSOI5 = 0;
   HcalTriggerPrimitiveIEta = 0;
   HcalTriggerPrimitiveIPhi = 0;
   HcalTriggerPrimitivePresamples = 0;
   HcalTriggerPrimitiveSize = 0;
   QIE10DigiADC = 0;
   QIE10DigiCapID = 0;
   QIE10DigiLETDC = 0;
   QIE10DigiOK = 0;
   QIE10DigiSOI = 0;
   QIE10DigiTETDC = 0;
   QIE11DigiADC = 0;
   QIE11DigiCapID = 0;
   QIE11DigiSOI = 0;
   QIE11DigiTDC = 0;
   HcalTriggerPrimitiveCompressedEt = 0;
   HcalTriggerPrimitiveFineGrain0 = 0;
   HcalTriggerPrimitiveFineGrain1 = 0;
   HcalTriggerPrimitiveFineGrain2 = 0;
   HcalTriggerPrimitiveFineGrain3 = 0;
   HcalTriggerPrimitiveFineGrain4 = 0;
   HcalTriggerPrimitiveFineGrain5 = 0;
   HcalTriggerPrimitiveHBHEDigiIndex = 0;
   HcalTriggerPrimitiveHFDigiIndex = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("QIE11DigiTimeFC", &QIE11DigiTimeFC, &b_QIE11DigiTimeFC);
   fChain->SetBranchAddress("QIE11DigiTotFC", &QIE11DigiTotFC, &b_QIE11DigiTotFC);
   fChain->SetBranchAddress("QIE10DigiFC", &QIE10DigiFC, &b_QIE10DigiFC);
   fChain->SetBranchAddress("QIE11DigiFC", &QIE11DigiFC, &b_QIE11DigiFC);
   fChain->SetBranchAddress("QIE11DigiTimeTDC", &QIE11DigiTimeTDC, &b_QIE11DigiTimeTDC);
   fChain->SetBranchAddress("laserType", &laserType, &b_laserType);
   fChain->SetBranchAddress("QIE10DigiDepth", &QIE10DigiDepth, &b_QIE10DigiDepth);
   fChain->SetBranchAddress("QIE10DigiFlags", &QIE10DigiFlags, &b_QIE10DigiFlags);
   fChain->SetBranchAddress("QIE10DigiHFDepth", &QIE10DigiHFDepth, &b_QIE10DigiHFDepth);
   fChain->SetBranchAddress("QIE10DigiIEta", &QIE10DigiIEta, &b_QIE10DigiIEta);
   fChain->SetBranchAddress("QIE10DigiIPhi", &QIE10DigiIPhi, &b_QIE10DigiIPhi);
   fChain->SetBranchAddress("QIE10DigiLinkError", &QIE10DigiLinkError, &b_QIE10DigiLinkError);
   fChain->SetBranchAddress("QIE10DigiRawID", &QIE10DigiRawID, &b_QIE10DigiRawID);
   fChain->SetBranchAddress("QIE10DigiSubdet", &QIE10DigiSubdet, &b_QIE10DigiSubdet);
   fChain->SetBranchAddress("QIE11DigiCapIDError", &QIE11DigiCapIDError, &b_QIE11DigiCapIDError);
   fChain->SetBranchAddress("QIE11DigiDepth", &QIE11DigiDepth, &b_QIE11DigiDepth);
   fChain->SetBranchAddress("QIE11DigiFlags", &QIE11DigiFlags, &b_QIE11DigiFlags);
   fChain->SetBranchAddress("QIE11DigiIEta", &QIE11DigiIEta, &b_QIE11DigiIEta);
   fChain->SetBranchAddress("QIE11DigiIPhi", &QIE11DigiIPhi, &b_QIE11DigiIPhi);
   fChain->SetBranchAddress("QIE11DigiLinkError", &QIE11DigiLinkError, &b_QIE11DigiLinkError);
   fChain->SetBranchAddress("QIE11DigiNTDC", &QIE11DigiNTDC, &b_QIE11DigiNTDC);
   fChain->SetBranchAddress("QIE11DigiRawID", &QIE11DigiRawID, &b_QIE11DigiRawID);
   fChain->SetBranchAddress("QIE11DigiSubdet", &QIE11DigiSubdet, &b_QIE11DigiSubdet);
   fChain->SetBranchAddress("HcalTriggerPrimitiveCompressedEtSOI", &HcalTriggerPrimitiveCompressedEtSOI, &b_HcalTriggerPrimitiveCompressedEtSOI);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI0", &HcalTriggerPrimitiveFineGrainSOI0, &b_HcalTriggerPrimitiveFineGrainSOI0);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI1", &HcalTriggerPrimitiveFineGrainSOI1, &b_HcalTriggerPrimitiveFineGrainSOI1);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI2", &HcalTriggerPrimitiveFineGrainSOI2, &b_HcalTriggerPrimitiveFineGrainSOI2);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI3", &HcalTriggerPrimitiveFineGrainSOI3, &b_HcalTriggerPrimitiveFineGrainSOI3);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI4", &HcalTriggerPrimitiveFineGrainSOI4, &b_HcalTriggerPrimitiveFineGrainSOI4);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrainSOI5", &HcalTriggerPrimitiveFineGrainSOI5, &b_HcalTriggerPrimitiveFineGrainSOI5);
   fChain->SetBranchAddress("HcalTriggerPrimitiveIEta", &HcalTriggerPrimitiveIEta, &b_HcalTriggerPrimitiveIEta);
   fChain->SetBranchAddress("HcalTriggerPrimitiveIPhi", &HcalTriggerPrimitiveIPhi, &b_HcalTriggerPrimitiveIPhi);
   fChain->SetBranchAddress("HcalTriggerPrimitivePresamples", &HcalTriggerPrimitivePresamples, &b_HcalTriggerPrimitivePresamples);
   fChain->SetBranchAddress("HcalTriggerPrimitiveSize", &HcalTriggerPrimitiveSize, &b_HcalTriggerPrimitiveSize);
   fChain->SetBranchAddress("QIE10DigiADC", &QIE10DigiADC, &b_QIE10DigiADC);
   fChain->SetBranchAddress("QIE10DigiCapID", &QIE10DigiCapID, &b_QIE10DigiCapID);
   fChain->SetBranchAddress("QIE10DigiLETDC", &QIE10DigiLETDC, &b_QIE10DigiLETDC);
   fChain->SetBranchAddress("QIE10DigiOK", &QIE10DigiOK, &b_QIE10DigiOK);
   fChain->SetBranchAddress("QIE10DigiSOI", &QIE10DigiSOI, &b_QIE10DigiSOI);
   fChain->SetBranchAddress("QIE10DigiTETDC", &QIE10DigiTETDC, &b_QIE10DigiTETDC);
   fChain->SetBranchAddress("QIE11DigiADC", &QIE11DigiADC, &b_QIE11DigiADC);
   fChain->SetBranchAddress("QIE11DigiCapID", &QIE11DigiCapID, &b_QIE11DigiCapID);
   fChain->SetBranchAddress("QIE11DigiSOI", &QIE11DigiSOI, &b_QIE11DigiSOI);
   fChain->SetBranchAddress("QIE11DigiTDC", &QIE11DigiTDC, &b_QIE11DigiTDC);
   fChain->SetBranchAddress("HcalTriggerPrimitiveCompressedEt", &HcalTriggerPrimitiveCompressedEt, &b_HcalTriggerPrimitiveCompressedEt);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain0", &HcalTriggerPrimitiveFineGrain0, &b_HcalTriggerPrimitiveFineGrain0);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain1", &HcalTriggerPrimitiveFineGrain1, &b_HcalTriggerPrimitiveFineGrain1);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain2", &HcalTriggerPrimitiveFineGrain2, &b_HcalTriggerPrimitiveFineGrain2);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain3", &HcalTriggerPrimitiveFineGrain3, &b_HcalTriggerPrimitiveFineGrain3);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain4", &HcalTriggerPrimitiveFineGrain4, &b_HcalTriggerPrimitiveFineGrain4);
   fChain->SetBranchAddress("HcalTriggerPrimitiveFineGrain5", &HcalTriggerPrimitiveFineGrain5, &b_HcalTriggerPrimitiveFineGrain5);
   fChain->SetBranchAddress("HcalTriggerPrimitiveHBHEDigiIndex", &HcalTriggerPrimitiveHBHEDigiIndex, &b_HcalTriggerPrimitiveHBHEDigiIndex);
   fChain->SetBranchAddress("HcalTriggerPrimitiveHFDigiIndex", &HcalTriggerPrimitiveHFDigiIndex, &b_HcalTriggerPrimitiveHFDigiIndex);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("run", &run, &b_run);
   Notify();
}

Bool_t HBStudy_plotByQIE::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HBStudy_plotByQIE::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HBStudy_plotByQIE::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef HBStudy_plotByQIE_cxx
