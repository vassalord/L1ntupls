// Script for calculating efficiency and resolution histograms
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TChain.h"

#include <TLatex.h>
#include <TEfficiency.h>

#include <iostream>
#include <fstream>
#include <string>
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoVertexDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisCaloTPDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1CaloTowerDataFormat.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoJetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetFilterDataFormat.h"

#include <filesystem>

using std::filesystem::directory_iterator;

/* TODO: put errors in rates...
creates the rates and distributions for l1 trigger objects
How to use:
1. input the number of bunches in the run (~line 35)
2. change the variables "newConditionsNtuples" and "oldConditionsNtuples" to ntuple paths
3. If good run JSON is not applied during ntuple production, modify isGoodLumiSection()

Optionally, if you want to rescale to a given instantaneous luminosity:
1. input the instantaneous luminosity of the run (~line 32) [only if we scale to 2016 nominal]
2. select whether you rescale to L=1.5e34 (~line606??...) generally have it setup to rescale
nb: for 2&3 I have provided the info in runInfoForRates.txt
*/

// configurable parameters
double numBunch = 1537; //the number of bunches colliding for the run of interest
double runLum = 0.02; // 0.44: 275783  0.58:  276363 //luminosity of the run of interest (*10^34)
double expectedLum = 1.15; //expected luminosity of 2016 runs (*10^34)

void jetanalysis(bool newConditions, const std::string& inputFileDirectory);

int main(int argc, char *argv[])
{
  bool newConditions = true;
  std::string ntuplePath("");

  if (argc != 3) {
    std::cout << "Usage: l1jetanalysis.exe [new/def] [path to ntuples]\n"
	      << "[new/def] indicates new or default (existing) conditions" << std::endl;
    exit(1);
  }
  else {
    std::string par1(argv[1]);
    std::transform(par1.begin(), par1.end(), par1.begin(), ::tolower);
    if(par1.compare("new") == 0) newConditions = true;
    else if(par1.compare("def") == 0) newConditions = false;
    else {
      std::cout << "First parameter must be \"new\" or \"def\"" << std::endl;
      exit(1);
    }
    ntuplePath = argv[2];
  }

  jetanalysis(newConditions, ntuplePath);

  return 0;
}

// only need to edit this section if good run JSON
// is not used during ntuple production
bool isGoodLumiSection(int lumiBlock)
{
  if (lumiBlock >= 1
      || lumiBlock <= 10000) {
    return true;
  }

  return false;
}

double deltaPhi(double phi1, double phi2) {
  double result = phi1 - phi2;
  if(fabs(result) > 9999) return result;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

double deltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return sqrt(deta*deta + dphi*dphi);
}

void jetanalysis(bool newConditions, const std::string& inputFileDirectory){
  
  bool hwOn = true;   //are we using data from hardware? (upgrade trigger had to be running!!!)
  bool emuOn = true; //true;  //are we using data from emulator?
  
  if (hwOn==false && emuOn==false){
    std::cout << "exiting as neither hardware or emulator selected" << std::endl;
    return;
  }
  std::string inputFile(inputFileDirectory);
  inputFile += "/L1Ntuple*.root";

  std::string inputFile_m4(inputFileDirectory);
  inputFile_m4 += "/L1Ntuple_QIEdelay-4*.root";
  std::string inputFile_m2(inputFileDirectory);
  inputFile_m2 += "/L1Ntuple_QIEdelay-2*.root";
  std::string inputFile_0(inputFileDirectory);
  inputFile_0 += "/L1Ntuple_QIEdelay0*.root";
  std::string inputFile_2(inputFileDirectory);
  inputFile_2 += "/L1Ntuple_QIEdelay2*.root";
  std::string inputFile_4(inputFileDirectory);
  inputFile_4 += "/L1Ntuple_QIEdelay4*.root";
  std::string inputFile_6(inputFileDirectory);
  inputFile_6 += "/L1Ntuple_QIEdelay6*.root";
  std::string inputFile_8(inputFileDirectory);
  inputFile_8 += "/L1Ntuple_QIEdelay8*.root";
  std::cout << inputFile_m4 << std::endl;
  std::cout << inputFile_m2 << std::endl;
  std::cout << inputFile_0 << std::endl;
  std::cout << inputFile_2 << std::endl;
  std::cout << inputFile_4 << std::endl;
  std::cout << inputFile_6 << std::endl;
  std::cout << inputFile_8 << std::endl;
  std::string outputDirectory = "emu";  //***runNumber, triggerType, version, hw/emu/both***MAKE SURE IT EXISTS
  std::string outputFilename = "l1analysis_def.root";
  if(newConditions) outputFilename = "l1analysis_new_cond.root";
  TFile* kk = TFile::Open( outputFilename.c_str() , "recreate");
  // if (kk!=0){
  //   cout << "TERMINATE: not going to overwrite file " << outputFilename << endl;
  //   return;
  // }

  /*  int Total_nentries = 0;
  std::vector<std::tuple<int,std::tuple<int,int>>> QIEdelay_entry;
  for (const auto & file : directory_iterator(inputFileDirectory)) {
    //    std::cout << file.path() << std::endl;
    int QIEdelay;
    if (std::string(std::string(file.path()).substr(33,1).c_str()) == "-") QIEdelay = std::stoi(std::string(file.path()).substr(33,2).c_str()); //std::cout << std::string(file.path()).substr(33,2).c_str() << std::endl;
    else QIEdelay = std::stoi(std::string(file.path()).substr(33,1).c_str()); // std::cout << std::string(file.path()).substr(33,1).c_str() << std::endl;
    TChain * countEvents = new TChain("l1EventTree/L1EventTree");
    countEvents->Add(file.path().c_str());
    int nentries;
    nentries = countEvents->GetEntries();
    Total_nentries += nentries;
    QIEdelay_entry.push_back(std::make_tuple(QIEdelay, std::make_tuple(nentries,Total_nentries)));
  }
  for (uint i=0; i<QIEdelay_entry.size(); i++) std::cout << std::get<0>(QIEdelay_entry[i]) << " = QIE delay, which has nentries = " << std::get<0>(std::get<1>(QIEdelay_entry[i])) << " out of total entries = " << std::get<1>(std::get<1>(QIEdelay_entry[i])) << std::endl; */

  std::vector<std::tuple<int,std::tuple<int,int>>> QIEdelay_order;
  // make trees
  std::cout << "Loading up the TChain..." << std::endl;
  TChain * eventTree = new TChain("l1EventTree/L1EventTree");
  eventTree->Add(inputFile_m4.c_str());
  int nentries_m4 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(-4, std::make_tuple(nentries_m4, nentries_m4)));
  eventTree->Add(inputFile_m2.c_str());
  int nentries_m2 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(-2, std::make_tuple(nentries_m2 - nentries_m4, nentries_m2)));
  eventTree->Add(inputFile_0.c_str());
  int nentries_0 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(0, std::make_tuple(nentries_0 - nentries_m2, nentries_0)));
  eventTree->Add(inputFile_2.c_str());
  int nentries_2 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(2, std::make_tuple(nentries_2 - nentries_0, nentries_2)));
  eventTree->Add(inputFile_4.c_str());
  int nentries_4 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(4, std::make_tuple(nentries_4 - nentries_2, nentries_4)));
  eventTree->Add(inputFile_6.c_str());
  int nentries_6 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(6, std::make_tuple(nentries_6 - nentries_4, nentries_6)));
  eventTree->Add(inputFile_8.c_str());
  int nentries_8 = eventTree->GetEntries();
  QIEdelay_order.push_back(std::make_tuple(8, std::make_tuple(nentries_8 - nentries_6, nentries_8)));
  for (uint i=0; i<QIEdelay_order.size(); i++) std::cout << std::get<0>(QIEdelay_order[i]) << " = QIE delay, which has nentries = " << std::get<0>(std::get<1>(QIEdelay_order[i])) << " out of total entries = " << std::get<1>(std::get<1>(QIEdelay_order[i])) << std::endl;

  // L1 jets
  TChain * treeL1emu = new TChain("l1UpgradeEmuTree/L1UpgradeTree");
  if (emuOn){
    treeL1emu->Add(inputFile_m4.c_str());
    treeL1emu->Add(inputFile_m2.c_str());
    treeL1emu->Add(inputFile_0.c_str());
    treeL1emu->Add(inputFile_2.c_str());
    treeL1emu->Add(inputFile_4.c_str());
    treeL1emu->Add(inputFile_6.c_str());
    treeL1emu->Add(inputFile_8.c_str());
  }
  TChain * treeL1hw = new TChain("l1UpgradeTree/L1UpgradeTree");
  if (hwOn){
    //    treeL1hw->Add(inputFile.c_str());
    treeL1hw->Add(inputFile_m4.c_str());
    treeL1hw->Add(inputFile_m2.c_str());
    treeL1hw->Add(inputFile_0.c_str());
    treeL1hw->Add(inputFile_2.c_str());
    treeL1hw->Add(inputFile_4.c_str());
    treeL1hw->Add(inputFile_6.c_str());
    treeL1hw->Add(inputFile_8.c_str());
  }
  // Calo towers
  TChain * treeL1TPemu = new TChain("l1CaloTowerEmuTree/L1CaloTowerTree");
  if (emuOn) {
    //    treeL1TPemu->Add(inputFile.c_str());
    treeL1TPemu->Add(inputFile_m4.c_str());
    treeL1TPemu->Add(inputFile_m2.c_str());
    treeL1TPemu->Add(inputFile_0.c_str());
    treeL1TPemu->Add(inputFile_2.c_str());
    treeL1TPemu->Add(inputFile_4.c_str());
    treeL1TPemu->Add(inputFile_6.c_str());
    treeL1TPemu->Add(inputFile_8.c_str());
  }
  TChain * treeL1TPhw = new TChain("l1CaloTowerTree/L1CaloTowerTree");
  if (hwOn) {
    //    treeL1TPhw->Add(inputFile.c_str());
    treeL1TPhw->Add(inputFile_m4.c_str());
    treeL1TPhw->Add(inputFile_m2.c_str());
    treeL1TPhw->Add(inputFile_2.c_str());
    treeL1TPhw->Add(inputFile_0.c_str());
    treeL1TPhw->Add(inputFile_4.c_str());
    treeL1TPhw->Add(inputFile_6.c_str());
    treeL1TPhw->Add(inputFile_8.c_str());
  }
  TChain * treeL1CTemu = new TChain("l1CaloTowerEmuTree/L1CaloTowerTree");
  if (emuOn) {
    //    treeL1CTemu->Add(inputFile.c_str());
    treeL1CTemu->Add(inputFile_m4.c_str());
    treeL1CTemu->Add(inputFile_m2.c_str());
    treeL1CTemu->Add(inputFile_2.c_str());
    treeL1CTemu->Add(inputFile_0.c_str());
    treeL1CTemu->Add(inputFile_4.c_str());
    treeL1CTemu->Add(inputFile_6.c_str());
    treeL1CTemu->Add(inputFile_8.c_str());
  }
  TChain * treeL1CThw = new TChain("l1CaloTowerTree/L1CaloTowerTree");
  if (hwOn) {
    //    treeL1CThw->Add(inputFile.c_str());
    treeL1CThw->Add(inputFile_m4.c_str());
    treeL1CThw->Add(inputFile_m2.c_str());
    treeL1CThw->Add(inputFile_0.c_str());
    treeL1CThw->Add(inputFile_2.c_str());
    treeL1CThw->Add(inputFile_4.c_str());
    treeL1CThw->Add(inputFile_6.c_str());
    treeL1CThw->Add(inputFile_8.c_str()); 
  }

  L1Analysis::L1AnalysisEventDataFormat    *event_ = new L1Analysis::L1AnalysisEventDataFormat();
  eventTree->SetBranchAddress("Event", &event_);
  // L1 jets
  L1Analysis::L1AnalysisL1UpgradeDataFormat    *l1emu_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  treeL1emu->SetBranchAddress("L1Upgrade", &l1emu_);
  L1Analysis::L1AnalysisL1UpgradeDataFormat    *l1hw_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  treeL1hw->SetBranchAddress("L1Upgrade", &l1hw_);
  // Calo towers
  L1Analysis::L1AnalysisCaloTPDataFormat    *l1TPemu_ = new L1Analysis::L1AnalysisCaloTPDataFormat();
  treeL1TPemu->SetBranchAddress("CaloTP", &l1TPemu_);
  L1Analysis::L1AnalysisCaloTPDataFormat    *l1TPhw_ = new L1Analysis::L1AnalysisCaloTPDataFormat();
  treeL1TPhw->SetBranchAddress("CaloTP", &l1TPhw_);
  L1Analysis::L1AnalysisL1CaloTowerDataFormat    *l1CTemu_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
  treeL1CTemu->SetBranchAddress("L1CaloTower", &l1CTemu_);
  L1Analysis::L1AnalysisL1CaloTowerDataFormat    *l1CThw_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
  treeL1CThw->SetBranchAddress("L1CaloTower", &l1CThw_);

  // get number of entries
  Long64_t nentries;
  if (emuOn) nentries = treeL1emu->GetEntries();
  else nentries = treeL1hw->GetEntries();
  int goodLumiEventCount = 0;

  std::string outputTxtFilename = "output_rates/" + outputDirectory + "/extraInfo.txt";
  std::ofstream myfile; // save info about the run, including rates for a given lumi section, and number of events we used.
  myfile.open(outputTxtFilename.c_str());
  eventTree->GetEntry(0);
  myfile << "run number = " << event_->run << std::endl;

  // set parameters for histograms
  // jet bins
  int nJetBins = 200;
  float jetLo = 0.;
  float jetHi = 500.;
  //  float jetBinWidth = (jetHi-jetLo)/nJetBins;

  // htSum bins
  int nHtSumBins = 800;
  float htSumLo = 0.;
  float htSumHi = 800.;

  // mhtSum bins
  int nMhtSumBins = 500;
  float mhtSumLo = 0.;
  float mhtSumHi = 500.;

  // etSum bins
  int nEtSumBins = 1000;
  float etSumLo = 0.;
  float etSumHi = 1000.;

  // metSum bins
  int nMetSumBins = 500;
  float metSumLo = 0.;
  float metSumHi = 500.;

  // tp bins
  int nTpBins = 100;
  float tpLo = 0.;
  float tpHi = 100.;

  std::string axR = ";Threshold E_{T} (GeV);rate (Hz)";
  std::string axD = ";E_{T} (GeV);events/bin";
  std::string metD = ";MET (GeV);events/bin";
  
  //make histos
  // Jets
  TH1F *jetET50   = new TH1F( "JetEt50" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *jetET64   = new TH1F( "JetEt64" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *jetET76   = new TH1F( "JetEt76" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *jetET92   = new TH1F( "JetEt92" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *jetET112  = new TH1F( "JetEt112" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *jetET180  = new TH1F( "JetEt180" , axD.c_str(),nJetBins, jetLo, jetHi);

  TH1F *jetET_hwQualEMU = new TH1F( "JetEt_hwQualEMU" , axD.c_str(),nJetBins/10, jetLo, jetHi);
  TH1F *llp_QIEdelayEMU = new TH1F( "LLPqieDelayEMU" , "LLP Jet Efficiency;QIE Delay (ns);", 11, -2, 9);
  TH1F *llp_QIEdelay_centralEMU = new TH1F( "LLPqieDelay_centralEMU" , "LLP Jet Efficiency (central HB);QIE Delay (ns)", 11, -2, 9);  

  TH1F *l1jetET1 = new TH1F( "singleJet" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *l1jetET2 = new TH1F( "doubleJet" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *l1jetET3 = new TH1F( "tripleJet" , axD.c_str(),nJetBins, jetLo, jetHi);
  TH1F *l1jetET4 = new TH1F( "quadJet" , axD.c_str(),nJetBins, jetLo, jetHi);

  // and Sums
  TH1F *l1ET = new TH1F("etSum","L1 sumET (GeV)",nEtSumBins,etSumLo,etSumHi);
  TH1F *l1MET = new TH1F("metSum","L1 MET (GeV)",nMetSumBins,metSumLo,metSumHi);
  TH1F *l1METHF = new TH1F("metHFSum","L1 METHF (GeV)",nMetSumBins,metSumLo,metSumHi);
  TH1F *l1HT = new TH1F("htSum","L1 HT (GeV)",nHtSumBins,htSumLo,htSumHi);
  TH1F *l1MHT = new TH1F("mhtSum","L1 MHT (GeV)",nMhtSumBins,mhtSumLo,mhtSumHi);

  // hcal/ecal TPs
  TH1F* hcalTP_emu = new TH1F("hcalTP_emu", ";TP E_{T}; # Entries", nTpBins, tpLo, tpHi);
  TH1F* ecalTP_emu = new TH1F("ecalTP_emu", ";TP E_{T}; # Entries", nTpBins, tpLo, tpHi);

  // calo towers
  TH1F *CT_LLPhwQual_HB_emu = new TH1F( "TT_LLPhwQual_HB_emu", "TPs with hwQual set (HB);QIE Delay (ns); # Entries", 11, -2, 9);
  TH1F *CT_LLPhwQual_HE_emu = new TH1F( "TT_LLPhwQual_HE_emu", "TPs with hwQual set (HE);QIE Delay (ns); # Entries", 11, -2, 9);
  TH1F *CT_hwQual_HB_emu = new TH1F( "TT_hwQual_HB_emu", "TPs with hwQual set (HB);Tower hwQual; # Entries", 16, 0, 16);
  TH1F *CT_hwQual_HE_emu = new TH1F( "TT_hwQual_HE_emu", "TPs with hwQual set (HE);Tower hwQual; # Entries", 16, 0, 16);

  // hw
  // for jet efficiencies normalizations
  std::map<int, TH1F*> jetET_all;
  std::map<int, TH1F*> jetET_all_central;
  //  std::map<int, TH1F*> llp_all;
  //  std::map<int, TH1F*> llp_all_central;
  //  TH1F *jetET_all = new TH1F( "JetEt_all" , axD.c_str(),nJetBins/10, jetLo, jetHi);
  TH1F *llp_all = new TH1F( "LLPqieDelay_all" , "LLP Jet Efficiency;QIE Delay (ns);", 11, -2, 9);
  TH1F *llp_all_central = new TH1F( "LLPqieDelay_central_all" , "LLP Jet Efficiency (central HB);QIE Delay (ns);", 11, -2, 9);
  // l1 jets
  std::map<int, TH1F*> jetET_hwQual;
  std::map<int, TH1F*> jetET_hwQual_central;
  //  std::map<int, TH1F*> llp_QIEdelay;
  //  std::map<int, TH1F*> llp_QIEdelay_central;
  //  TH1F *jetET_hwQual = new TH1F( "JetEt_hwQual" , axD.c_str(),nJetBins/10, jetLo, jetHi);
  TH1F *llp_QIEdelay = new TH1F( "LLPqieDelay" , "LLP Jet Efficiency;QIE Delay (ns);", 11, -2, 9);
  TH1F *llp_QIEdelay_central = new TH1F( "LLPqieDelay_central" , "LLP Jet Efficiency (central HB);QIE Delay (ns);", 11, -2, 9);

  // calo towers
  TH1F *CT_LLPhwQual_HB = new TH1F( "TT_LLPhwQual_HB", "TPs with hwQual set (HB);QIE Delay (ns); # Entries", 11, -2, 9);
  TH1F *CT_LLPhwQual_HE = new TH1F( "TT_LLPhwQual_HE", "TPs with hwQual set (HE);QIE Delay (ns); # Entries", 11, -2, 9);
  TH1F *CT_hwQual_HB = new TH1F( "TT_hwQual_HB", "TPs with hwQual set (HB);Tower hwQual; # Entries", 16, 0, 16);
  TH1F *CT_hwQual_HE = new TH1F( "TT_hwQual_HE", "TPs with hwQual set (HE);Tower hwQual; # Entries", 16, 0, 16);

  TH1F *TP_FG_HB = new TH1F ( "TP_FG_HB", "Finegrain bits for TPs (HB);fg3, fg2, fg1, fg0;# Entries",16,0,16);
  TH1F *TP_FG_HE = new TH1F ( "TP_FG_HE", "Finegrain bits for TPs (HE);fg3, fg2, fg1, fg0;# Entries",16,0,16);

  /////////////////////////////////
  // loop through all the entries//
  /////////////////////////////////
  for (Long64_t jentry=0; jentry<nentries; jentry++){
    if((jentry%10000)==0) std::cout << "Done " << jentry  << " events of " << nentries << std::endl;
    //int iphi_emucalo = -999;
    int ieta_number = -999;
    // determine what the QIE delay is based on the jentry of the files (loaded in order)
    int QIEdelay = -99;
    if (QIEdelay_order.size() == 1) QIEdelay = std::get<0>(QIEdelay_order[0]); // only one delay to handle in this case
    for (uint i=0; i<QIEdelay_order.size()-1; i++) {
      if (jentry < std::get<1>(std::get<1>(QIEdelay_order[i]))) {
	QIEdelay = std::get<0>(QIEdelay_order[i]); // in first file
	break;
      }
      else if (jentry < std::get<1>(std::get<1>(QIEdelay_order[i+1])) && jentry >= std::get<1>(std::get<1>(QIEdelay_order[i]))) QIEdelay = std::get<0>(QIEdelay_order[i+1]);
      else if (jentry >= std::get<1>(std::get<1>(QIEdelay_order[i+1]))) QIEdelay = std::get<0>(QIEdelay_order[i+1]); // in last file
    }

    //lumi break clause
    eventTree->GetEntry(jentry);
    //skip the corresponding event
    if (!isGoodLumiSection(event_->lumi)) continue;
    goodLumiEventCount++;

    //do routine for L1 emulator quantites
    if (emuOn){

      treeL1TPemu->GetEntry(jentry);
      double tpEt(0.);
      
      for(int i=0; i < l1TPemu_->nHCALTP; i++){
	tpEt = l1TPemu_->hcalTPet[i];
	hcalTP_emu->Fill(tpEt);
      }
      for(int i=0; i < l1TPemu_->nECALTP; i++){
	tpEt = l1TPemu_->ecalTPet[i];
	ecalTP_emu->Fill(tpEt);
      }

      treeL1emu->GetEntry(jentry);
      // get jetEt*, egEt*, tauEt, htSum, mhtSum, etSum, metSum
      // ALL EMU OBJECTS HAVE BX=0...
      double jetEt_1(0.);
      if (l1emu_->nJets>0) {
	jetEt_1 = l1emu_->jetEt[0];
	l1jetET1->Fill(jetEt_1);
      }
      double jetEt_2(0.);
      if (l1emu_->nJets>1) {
	jetEt_2 = l1emu_->jetEt[1];
	l1jetET2->Fill(jetEt_2);
      }
      double jetEt_3(0.);
      if (l1emu_->nJets>2) {
	jetEt_3 = l1emu_->jetEt[2];
	l1jetET3->Fill(jetEt_3);
      }
      double jetEt_4(0.);
      if (l1emu_->nJets>3) {
	jetEt_4 = l1emu_->jetEt[3];
	l1jetET4->Fill(jetEt_4);
      }

      int LLP_jet_ieta = -999; // fill with jet ieta if LLP flagged jet found
      int LLP_jet_iphi = -999;
      std::ofstream out ("L1Ntuple_Event_EMU.txt", std::ios::app);
      //std::ofstream out ("L1Ntuple_jet_EMU.txt", std::ios::app);
      if (out.is_open()){

      uint nJetemu = l1emu_->nJets; // number of jets per event
      for (uint jetIt = 0; jetIt < nJetemu; jetIt++) {
	if (l1emu_->jetEt[jetIt]>50.) jetET50->Fill(l1emu_->jetEt[jetIt]);
	if (l1emu_->jetEt[jetIt]>64.) jetET64->Fill(l1emu_->jetEt[jetIt]);
	if (l1emu_->jetEt[jetIt]>76.) jetET76->Fill(l1emu_->jetEt[jetIt]);
	if (l1emu_->jetEt[jetIt]>92.) jetET92->Fill(l1emu_->jetEt[jetIt]);
	if (l1emu_->jetEt[jetIt]>112.) jetET112->Fill(l1emu_->jetEt[jetIt]);
	if (l1emu_->jetEt[jetIt]>180.) jetET180->Fill(l1emu_->jetEt[jetIt]);
	 
	int jet_ieta = (abs(l1emu_->jetIEta[jetIt])+1)/2*(l1emu_->jetIEta[jetIt]/abs(l1emu_->jetIEta[jetIt]));
	int jet_iphi = (l1emu_->jetIPhi[jetIt] + 1) / 2;
	//out << "L1 Event " << jentry << ": EMU L1 jet has HwQual set ieta, iphi: " << jet_ieta << ", " << jet_iphi << " Energy "<< l1emu_->jetEt[jetIt] << std::endl;

	if (l1emu_->jetHwQual[jetIt] == 1 && abs(jet_ieta) <= 29) { // look at jets with hwQual set, and jets must be in HB
	LLP_jet_ieta = jet_ieta;
	LLP_jet_iphi = jet_iphi;
	std::cout << "L1 Event " << jentry << ": EMU L1 jet has HwQual set ieta, iphi: " << LLP_jet_ieta << ", " << LLP_jet_iphi << " Energy "<< l1emu_->jetEt[jetIt] << std::endl;
	out << "L1 Event " << jentry << ": EMU L1 jet has HwQual set ieta, iphi: " << LLP_jet_ieta << ", " << LLP_jet_iphi <<  " Energy "<< l1emu_->jetEt[jetIt] << std::endl;
	  if (QIEdelay == 8) jetET_hwQualEMU->Fill(l1emu_->jetEt[jetIt]); // distribution of jet ET for jets with hwQual set, for one value of QIE delay
	  llp_QIEdelayEMU->Fill(QIEdelay,1); // what QIE delay are LLP jets found at
	  if (abs(l1emu_->jetIEta[jetIt]) <= 8) llp_QIEdelay_centralEMU->Fill(QIEdelay,1);
	}
      }

      double htSum(0.0);
      double mhtSum(0.0);
      double etSum(0.0);
      double metSum(0.0);
      double metHFSum(0.0);
      for (unsigned int c=0; c<l1emu_->nSums; c++){
          if( l1emu_->sumBx[c] != 0 ) continue;
          if( l1emu_->sumType[c] == L1Analysis::kTotalEt ) etSum = l1emu_->sumEt[c];
          if( l1emu_->sumType[c] == L1Analysis::kTotalHt ) htSum = l1emu_->sumEt[c];
          if( l1emu_->sumType[c] == L1Analysis::kMissingEt ) metSum = l1emu_->sumEt[c];
	  if( l1emu_->sumType[c] == L1Analysis::kMissingEtHF ) metHFSum = l1emu_->sumEt[c];
          if( l1emu_->sumType[c] == L1Analysis::kMissingHt ) mhtSum = l1emu_->sumEt[c];
      }

      l1ET->Fill(etSum);
      l1MET->Fill(metSum);
      l1METHF->Fill(metHFSum);
      l1HT->Fill(htSum);
      l1MHT->Fill(mhtSum);
	
	
      
      
      treeL1CTemu->GetEntry(jentry); // Calo tower
      int nTower = l1CTemu_->nTower;
      for (int towers = 0; towers < nTower; towers++) {
        if (abs(l1CTemu_->ieta[towers]) < 16) CT_hwQual_HB_emu->Fill(l1CTemu_->iqual[towers],1);
        if (abs(l1CTemu_->ieta[towers]) >= 16 && abs(l1CTemu_->ieta[towers]) < 29) CT_hwQual_HE_emu->Fill(l1CTemu_->iqual[towers],1);
        if ( (l1CTemu_->iqual[towers] & 0b0100) >> 2 == 1) { // if tower hwQual is set
          if (abs(l1CTemu_->ieta[towers]) < 29) {
          ieta_number = l1CTemu_->ieta[towers];
          std::cout << "L1 Event " << jentry << ": EMU CaloTower flagged ieta, iphi: " << l1CTemu_->ieta[towers] << ", " << l1CTemu_->iphi[towers] << " Energy " <<  l1CTemu_->iet[towers] <<  std::endl;
          out << "L1 Event " << jentry << ": EMU CaloTower flagged ieta, iphi: " << l1CTemu_->ieta[towers] << ", " << l1CTemu_->iphi[towers]<< " Energy " <<  l1CTemu_->iet[towers] <<std::endl;
           //iphi_emucalo = l1CTemu_->iphi[towers];
	    CT_LLPhwQual_HB_emu->Fill(QIEdelay,1);
	    //std::cout << "Event " << jentry << ": EMU calo tower with hwqual = " << l1CTemu_->iqual[towers] << " at position ieta, iphi = " << l1CTemu_->ieta[towers] << ", " << l1CTemu_->iphi[towers] << std::endl;
	  }
          if (abs(l1CTemu_->ieta[towers]) >= 16 && abs(l1CTemu_->ieta[towers]) < 29) CT_LLPhwQual_HE_emu->Fill(QIEdelay,1);
          //if (abs(l1CTemu_->ieta[towers] - LLP_jet_ieta) < 5 && abs(l1CTemu_->iphi[towers] - LLP_jet_iphi) < 5) std::cout << "Event " << jentry << ": 9 and EMU flagged Calo tower is close to a EMU flagged L1 LLP jet which is at ieta, iphi " << l1CTemu_->ieta[towers] << ", " << l1CTemu_->iphi[towers] << std::endl;
	  //if (abs(l1CTemu_->ieta[towers] - LLP_jet_ieta) < 5 && abs(l1CTemu_->iphi[towers] - LLP_jet_iphi) > 4 && abs(l1CTemu_->iphi[towers] - LLP_jet_iphi) < 7) std::cout << "Event " << jentry << ": 13 and EMU flagged Calo tower is close to a EMU flagged L1 LLP jet which is at ieta, iphi " << l1CTemu_->ieta[towers] << ", " << l1CTemu_->iphi[towers] << std::endl;
	}
      }
      
       //int TPenergy[32][72];
       int nHCALTP = l1TPemu_->nHCALTP;
//    for (int tps = 0; tps < nHCALTP; tps++) {
//        int energy = l1TPemu_->hcalTPcompEt[tps];
//        int ieta = l1TPemu_->hcalTPieta[tps];
//        if (abs(l1TPemu_->hcalTPieta[tps]) <= 29 ){
//           if (ieta > 0) ieta += 28;
//           if (ieta < 0) ieta += 29;
//           int iphi = 72 - l1TPemu_->hcalTPiphi[tps] + 18;
//           if (iphi > 72) iphi -= 72;
//           TPenergy[ieta][iphi] = energy;
//          }
//     }
      
      for (int tps = 0; tps < nHCALTP; tps++) {
        int fg0 = l1TPemu_->hcalTPfineGrain0[tps];
        int fg1 = l1TPemu_->hcalTPfineGrain1[tps];
        int fg2 = l1TPemu_->hcalTPfineGrain2[tps];
        int fg3 = l1TPemu_->hcalTPfineGrain3[tps];
        //int fg4 = l1TPemu_->hcalTPfineGrain4[tps];
        //int fg5 = l1TPemu_->hcalTPfineGrain5[tps];
        int TPenergy = l1TPemu_->hcalTPcompEt[tps];
        int adj_iphi = 72 - l1TPemu_->hcalTPiphi[tps] + 18;
        if (adj_iphi > 72) adj_iphi -= 72;
        if (abs(l1TPemu_->hcalTPieta[tps]) < 16) TP_FG_HB->Fill(fg0 + (fg1 << 1) + (fg2 << 2) + (fg3 << 3));
        if (abs(l1TPemu_->hcalTPieta[tps]) >= 16 && abs(l1TPemu_->hcalTPieta[tps]) < 29) TP_FG_HE->Fill(fg0 + (fg1 << 1) + (fg2 << 2) + (fg3 << 3)); // 0 if over 16
        if (abs(l1TPemu_->hcalTPieta[tps]) <= 29 ){
           int ieta = l1TPemu_->hcalTPieta[tps];
           if (ieta > 0) ieta += 28;
           if (ieta < 0) ieta += 29;
        if ((fg0 + fg1 + fg2 + fg3 ) > 0) std::cout << "L1 Event " << jentry << ": EMU HCAL TP tower with set FG bits (" << fg0 << fg1 << fg2 << fg3<< "), with TP corrected ieta, iphi " << l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
        if ((fg0 + fg1 + fg2 + fg3 ) > 0) out << "L1 Event " << jentry << ": EMU HCAL TP tower with set FG bits (" << fg0 << fg1 << fg2 << fg3<< "), with TP corrected ieta, iphi " << l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi<< " Energy " << TPenergy << std::endl;
        if (fg0 || (!fg1 && (fg2 || fg3))) {
        std::cout << "L1 Event " << jentry << ": EMU flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP corrected ieta, iphi "<< l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
        out << "L1 Event " << jentry << ": EMU flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP corrected ieta, iphi "<< l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
	  //std::cout << "Event " << jentry << "in FG bit flagged tower logic, TP ieta, iphi = " << l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi << " and ieta, iphi of L1 jet = " << LLP_jet_ieta << ", " << LLP_jet_iphi << std::endl;
          //if (LLP_jet_ieta != -999) std::cout << "Event " << jentry << ": EMU flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP ieta, iphi " << l1TPemu_->hcalTPieta[tps] << ", " << l1TPemu_->hcalTPiphi[tps] << " (adjusted to match with HCAL ntuple iphi " << adj_iphi << ")" << std::endl;
	            if (abs(l1TPemu_->hcalTPieta[tps] - LLP_jet_ieta) < 5 && abs(adj_iphi - LLP_jet_iphi) >4 && abs(adj_iphi - LLP_jet_iphi) < 7) {
	    //std::cout << "Event " << jentry << ": 13 and EMU flagged HCAL TP is close to a EMU flagged L1 LLP jet which is at ieta, iphi " << l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi << std::endl;
	             }
	             if (abs(l1TPemu_->hcalTPieta[tps] - LLP_jet_ieta) < 5 && abs(adj_iphi - LLP_jet_iphi) < 5) {
	    //std::cout << "Event " << jentry << ": 9 and EMU flagged HCAL TP is close to a EMU jet, TP ieta, iphi " << l1TPemu_->hcalTPieta[tps] << ", " << adj_iphi <<  " LLP Jet ieta , iphi  " << LLP_jet_ieta << " , " << LLP_jet_iphi <<  std::endl;
	              }
        } // FG bit 6:1 LUT logic
        }
      }
      } // out endl
      out << std::endl;
      out.close();
    }// closes if 'emuOn' is true

    if (hwOn) {
      treeL1hw->GetEntry(jentry); // L1 jets
      treeL1CThw->GetEntry(jentry); // Calo tower
      treeL1TPhw->GetEntry(jentry); // CaloTP

      uint nJet = l1hw_->nJets; // number of jets per event

      int LLP_jet_ieta = -999;
      int LLP_jet_iphi = -999;
      int jet_ieta[nJet] = {-999};
      int jet_iphi[nJet] = {-999};

      if (jetET_all.find(QIEdelay) == jetET_all.end()) jetET_all[QIEdelay] = new TH1F(Form("JetEt_all_delay%d",QIEdelay),axD.c_str(),nJetBins/10, jetLo, jetHi);
      if (jetET_hwQual.find(QIEdelay) == jetET_hwQual.end()) jetET_hwQual[QIEdelay] = new TH1F(Form("JetEt_hwQual_delay%d",QIEdelay),axD.c_str(),nJetBins/10, jetLo, jetHi);
      if (jetET_all_central.find(QIEdelay) == jetET_all_central.end()) jetET_all_central[QIEdelay] = new TH1F(Form("JetEt_all_central_delay%d",QIEdelay),axD.c_str(),nJetBins/10, jetLo, jetHi);
      if (jetET_hwQual_central.find(QIEdelay) == jetET_hwQual_central.end()) jetET_hwQual_central[QIEdelay] = new TH1F(Form("JetEt_hwQual_central_delay%d",QIEdelay),axD.c_str(),nJetBins/10, jetLo, jetHi);

	std::ofstream out ("L1Ntuple_Event_RAW.txt", std::ios::app);
	//std::ofstream out ("L1Ntuple_jet_RAW.txt", std::ios::app);
	if (out.is_open()){
	
      for (uint jetIt = 0; jetIt < nJet; jetIt++) {
	jet_ieta[jetIt] = l1hw_->jetIEta[jetIt];
	jet_iphi[jetIt] = l1hw_->jetIPhi[jetIt];
	LLP_jet_ieta = (abs(l1hw_->jetIEta[jetIt])+1)/2*(l1hw_->jetIEta[jetIt]/abs(l1hw_->jetIEta[jetIt]));
	LLP_jet_iphi = (l1hw_->jetIPhi[jetIt] + 1) / 2;
	//out << "L1 Event " << jentry << ": RAW L1 jet has HwQual set, ieta, iphi: " << LLP_jet_ieta << ", " << LLP_jet_iphi << " Energy "<< l1hw_->jetEt[jetIt] << std::endl;

	if (abs(jet_ieta[jetIt]) < 29) { // look at jets with hwQual set, and jets must be in HB
	  jetET_all[QIEdelay]->Fill(l1hw_->jetEt[jetIt]);
	  llp_all->Fill(QIEdelay,1); // what QIE delay are LLP jets found at
	  if (abs(jet_ieta[jetIt]) <= 8) {
	    llp_all_central->Fill(QIEdelay,1);
	    jetET_all_central[QIEdelay]->Fill(l1hw_->jetEt[jetIt]);
	  }
	  if (l1hw_->jetHwQual[jetIt] == 1) {
	    //std::cout << "Event " << jentry << " ieta correct " << LLP_jet_ieta<< " ieta" << l1hw_->jetIEta[jetIt] << std::endl;
	    out << "L1 Event " << jentry << ": RAW L1 jet has HwQual set, ieta, iphi: " << LLP_jet_ieta << ", " << LLP_jet_iphi << " Energy " << l1hw_->jetEt[jetIt] << std::endl;
	    //std::cout << "L1 Event " << jentry << ": RAW L1 jet has HwQual set, ieta, iphi: " << LLP_jet_ieta << ", " << LLP_jet_iphi << std::endl;
	    std::cout << "L1 Event " << jentry << ": RAW L1 jet has HwQual set, in HB and the ieta, iphi is " << l1hw_->jetIEta[jetIt] << ", " << l1hw_->jetIPhi[jetIt] << " corrected ieta, iphi = " << LLP_jet_ieta << ", " << LLP_jet_iphi << std::endl;
	    jetET_hwQual[QIEdelay]->Fill(l1hw_->jetEt[jetIt]); // distribution of jet ET for jets with hwQual set, for one value of QIE delay
	    llp_QIEdelay->Fill(QIEdelay,1); // what QIE delay are LLP jets found at
	    if (abs(jet_ieta[jetIt]) <= 8) {
	      llp_QIEdelay_central->Fill(QIEdelay,1);
	      jetET_hwQual_central[QIEdelay]->Fill(l1hw_->jetEt[jetIt]);
	    }
	  }
	}
      } // jet loop
      int nTower = l1CThw_->nTower;
      //if (nTower > 0 ) std::cout << "Event " << jentry << ": nTower = " << nTower << std::endl;
      for (int towers = 0; towers < nTower; towers++) {
		for (uint jetIt = 0; jetIt < nJet; jetIt++) {
	  	  //if (abs(l1CThw_->ieta[towers] - jet_ieta[jetIt]) < 5 && abs(l1CThw_->iphi[towers] - jet_iphi[jetIt]) < 5)  std::cout << "calo tower found near a L1 jet (event " << jentry << ") at jet ieta, iphi = " << jet_ieta[jetIt] << ", " << jet_iphi[jetIt] << " with CT energy " << l1CThw_->iet[towers] << std::endl; // this finds calo towers near a jet
		} // end of jet loop
	//if (abs(l1CThw_->ieta[towers] - LLP_jet_ieta) < 5 && abs(l1CThw_->iphi[towers] - LLP_jet_iphi) < 5) std::cout <<  "Event " << jentry << ": RAW calo tower found near a LLP L1 jet with jet ieta, iphi = " << LLP_jet_ieta << ", " << LLP_jet_iphi << " with CT energy " << l1CThw_->iet[towers] << std::endl; // this finds calo towers near a LLP jet

	if (abs(l1CThw_->ieta[towers]) < 16) CT_hwQual_HB->Fill(l1CThw_->iqual[towers],1);
	if (abs(l1CThw_->ieta[towers]) >= 16 && abs(l1CThw_->ieta[towers]) < 29) CT_hwQual_HE->Fill(l1CThw_->iqual[towers],1); // 0 if over 16
	if ( (l1CThw_->iqual[towers] & 0b0100) >> 2 == 1) { // if tower hwQual is set
	  if (abs(l1CThw_->ieta[towers]) < 29) {
	    CT_LLPhwQual_HB->Fill(QIEdelay,1);
	     std::cout << "L1 Event " << jentry << ": RAW CaloTower flagged ieta, iphi: " << l1CThw_->ieta[towers] << ", " << l1CThw_->iphi[towers] << " Energy " << l1CThw_->iet[towers] << std::endl;
	     out << "L1 Event " << jentry << ": RAW CaloTower flagged ieta, iphi: " << l1CThw_->ieta[towers] << ", " << l1CThw_->iphi[towers] <<" Energy " << l1CThw_->iet[towers] << std::endl;
	    //if (abs(l1CThw_->ieta[towers] - LLP_jet_ieta) < 5 && abs(l1CThw_->iphi[towers] - LLP_jet_iphi) < 5) std::cout << "Event " << jentry << ": and RAW calo tower flagged with hwqual close to RAW nJets with ieta, iphi = " << l1CThw_->ieta[towers] << ", " << l1CThw_->iphi[towers] << std::endl;
	 if ( ieta_number == l1CThw_->ieta[towers] ){
	  //  std::cout << "delta_phi = EMU calo tower - RAW calo tower " << iphi_emucalo - l1CThw_->iphi[towers] << " ieta " << l1CThw_->ieta[towers] << " iphi emucalo " << iphi_emucalo << " iphi RAW calo " << l1CThw_->iphi[towers] << std::endl;
		}
	  }
	  if (abs(l1CThw_->ieta[towers]) >= 16 && abs(l1CThw_->ieta[towers]) < 29) CT_LLPhwQual_HE->Fill(QIEdelay,1); // 0 if over 16
	}
      }
      
//      int TPenergy[32][72];
      int nHCALTP = l1TPhw_->nHCALTP;
//    for (int tps = 0; tps < nHCALTP; tps++) {
//      int energy = l1TPhw_->hcalTPcompEt[tps];
//      int ieta = l1TPhw_->hcalTPieta[tps];
//      if (abs(l1TPhw_->hcalTPieta[tps]) <= 29 ){
//      if (ieta > 0) ieta += 28;
//      if (ieta < 0) ieta += 29;
//      int iphi = 72 - l1TPhw_->hcalTPiphi[tps] + 18;
//      if (iphi > 72) iphi -= 72;
//      TPenergy[ieta][iphi] = energy;
//        }
//    }

      for (int tps = 0; tps < nHCALTP; tps++) {
	int fg0 = l1TPhw_->hcalTPfineGrain0[tps]; 
	int fg1 = l1TPhw_->hcalTPfineGrain1[tps];
	int fg2 = l1TPhw_->hcalTPfineGrain2[tps];
	int fg3 = l1TPhw_->hcalTPfineGrain3[tps];
	//int fg4 = l1TPhw_->hcalTPfineGrain4[tps];
	//int fg5 = l1TPhw_->hcalTPfineGrain5[tps];
	int TPenergy = l1TPhw_->hcalTPcompEt[tps];
	int adj_iphi = 72 - l1TPhw_->hcalTPiphi[tps] + 18;
	if (adj_iphi > 72) adj_iphi -= 72;
	if (abs(l1TPhw_->hcalTPieta[tps]) < 16) TP_FG_HB->Fill(fg0 + (fg1 << 1) + (fg2 << 2) + (fg3 << 3));
        if (abs(l1TPhw_->hcalTPieta[tps]) >= 16 && abs(l1TPhw_->hcalTPieta[tps]) < 29) TP_FG_HE->Fill(fg0 + (fg1 << 1) + (fg2 << 2) + (fg3 << 3));  // 0 if over 16
        if (l1TPhw_->hcalTPieta[tps] >= 9 && l1TPhw_->hcalTPieta[tps] <= 12 && adj_iphi == 53) continue;
        if (abs(l1TPhw_->hcalTPieta[tps]) <= 29 ){
        // find the energy for this tower
           int ieta = l1TPhw_->hcalTPieta[tps];
           if (ieta > 0) ieta += 28;
           if (ieta < 0) ieta += 29;
           int adj_iphi = l1TPhw_->hcalTPiphi[tps]; // add the iphi correction for FG bits here
           
           adj_iphi = 72 - l1TPhw_->hcalTPiphi[tps] + 18;
           if (adj_iphi > 72) adj_iphi -= 72;
        
        if ((fg0 + fg1 + fg2 + fg3 ) > 0) std::cout << "L1 Event " << jentry << ": RAW HCAL TP tower with set FG bits (" << fg0 << fg1 << fg2 << fg3<<  "), with TP ieta, iphi " << l1TPhw_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
        if ((fg0 + fg1 + fg2 + fg3 ) > 0) out << "L1 Event " << jentry << ": RAW HCAL TP tower with set FG bits (" << fg0 << fg1 << fg2 << fg3<<  "), with TP ieta, iphi " << l1TPhw_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
	if (fg0 || (!fg1 && (fg2 || fg3))) {
          std::cout << "L1 Event " << jentry << ": RAW flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP ieta, iphi "<< l1TPhw_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
          out << "L1 Event " << jentry << ": RAW flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP ieta, iphi "<< l1TPhw_->hcalTPieta[tps] << ", " << adj_iphi << " Energy " << TPenergy << std::endl;
	  //if (LLP_jet_ieta != -999) std::cout << "Event " << jentry << ": RAW flagged HCAL TP tower based on FG logic (" << fg0 << fg1 << fg2 << fg3<< "), with TP ieta, iphi " << l1TPhw_->hcalTPieta[tps] << ", " << l1TPhw_->hcalTPiphi[tps] << " (adjusted to match with HCAL ntuple iphi " << adj_iphi << ")" << std::endl;
	  if (abs(l1TPhw_->hcalTPieta[tps] - LLP_jet_ieta) < 5 && abs(adj_iphi - LLP_jet_iphi) < 5) {
	    //std::cout << "Event " << jentry << ": and RAW flagged HCAL TP is close to a RAW flagged L1 LLP jet which is at ieta, iphi " << l1TPhw_->hcalTPieta[tps] << ", " << adj_iphi << std::endl;
	  }
	} // FG bit 6:1 LUT logic
      }
      }
      } //end file 
      out << std::endl;
      out.close();
    }

  }// closes loop through events

  TString cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2022 13.6 TeV Collisions}}";
  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextColor(kBlack);

  //  TFile g( outputFilename.c_str() , "new");
  kk->cd();
  // normalisation factor for rate histograms (11kHz is the orbit frequency)
  double norm = 11246*(numBunch/goodLumiEventCount); // no lumi rescale
  //  double norm = 11246*(numBunch/goodLumiEventCount)*(expectedLum/runLum); //scale to nominal lumi

  if (emuOn){
    // ecal/hcal TPs
    hcalTP_emu->Write();
    ecalTP_emu->Write();
    // l1 quantities
    l1jetET1->Write(); l1jetET2->Write(); l1jetET3->Write(); l1jetET4->Write();
    l1ET->Write(); l1MET->Write(); l1METHF->Write(); l1HT->Write(); l1MHT->Write();
    // efficiencies
    jetET50->Write(); jetET64->Write(); jetET76->Write(); jetET92->Write(); jetET112->Write(); jetET180->Write();
    jetET_hwQualEMU->Write();
    llp_QIEdelayEMU->Write(); llp_QIEdelay_centralEMU->Write();
    CT_hwQual_HB_emu->Write(); CT_hwQual_HE_emu->Write();
    CT_LLPhwQual_HB_emu->Write(); CT_LLPhwQual_HE_emu->Write();
  }
  if (hwOn) { /* commenting out plots because just testing one QIE delay input now
    for (uint i=0; i<QIEdelay_order.size(); i++) jetET_hwQual[std::get<0>(QIEdelay_order[i])]->Write();
    for (uint i=0; i<QIEdelay_order.size(); i++) jetET_hwQual_central[std::get<0>(QIEdelay_order[i])]->Write();
    for (uint i=0; i<QIEdelay_order.size(); i++) jetET_all[std::get<0>(QIEdelay_order[i])]->Write();
    for (uint i=0; i<QIEdelay_order.size(); i++) jetET_all_central[std::get<0>(QIEdelay_order[i])]->Write();
    llp_QIEdelay->Write(); llp_QIEdelay_central->Write();
    llp_all->Write(); llp_all_central->Write();
    CT_hwQual_HB->Write(); CT_hwQual_HE->Write();
    CT_LLPhwQual_HB->Write(); CT_LLPhwQual_HE->Write();
    TP_FG_HB->Write(); TP_FG_HE->Write();

    if (TEfficiency::CheckConsistency(*llp_QIEdelay, *llp_all)) {
      TEfficiency *effLLPjet = new TEfficiency(*llp_QIEdelay, *llp_all);
      effLLPjet->SetTitle(Form("LLP flagged jets vs QIE delay"));
      effLLPjet->SetLineWidth(2.);
      effLLPjet->SetLineColor(kBlack);
      effLLPjet->Draw();
      latex->DrawLatex(0.12, 0.85, cmsLabel);
      //      latex->DrawLatex(commentaryXpos - 0.45, 0.7, Form("#scale[0.8]{i#eta=%d}",it->first));
      //      gPad->Update();
      effLLPjet->Write();
    }
    if (TEfficiency::CheckConsistency(*llp_QIEdelay_central, *llp_all_central)) {
      TEfficiency *effLLPjet = new TEfficiency(*llp_QIEdelay_central, *llp_all_central);
      effLLPjet->SetTitle(Form("LLP flagged jets vs QIE delay (central HB)"));
      effLLPjet->SetLineWidth(2.);
      effLLPjet->SetLineColor(kBlack);
      effLLPjet->Draw();
      latex->DrawLatex(0.12, 0.85, cmsLabel);
      effLLPjet->Write();
    }
    for (uint i=0; i<QIEdelay_order.size(); i++) {
      if (TEfficiency::CheckConsistency(*jetET_hwQual[std::get<0>(QIEdelay_order[i])], *jetET_all[std::get<0>(QIEdelay_order[i])])) {
	TEfficiency *effLLPjet = new TEfficiency(*jetET_hwQual[std::get<0>(QIEdelay_order[i])], *jetET_all[std::get<0>(QIEdelay_order[i])]);
	effLLPjet->SetTitle(Form("LLP flagged jets vs Jet ET (QIE delay %d)",std::get<0>(QIEdelay_order[i])));
	effLLPjet->SetLineWidth(2.);
	effLLPjet->SetLineColor(kBlack);
	effLLPjet->Draw();
	latex->DrawLatex(0.12, 0.85, cmsLabel);
	effLLPjet->Write();
      }
      if (TEfficiency::CheckConsistency(*jetET_hwQual_central[std::get<0>(QIEdelay_order[i])], *jetET_all_central[std::get<0>(QIEdelay_order[i])])) {
        TEfficiency *effLLPjet = new TEfficiency(*jetET_hwQual_central[std::get<0>(QIEdelay_order[i])], *jetET_all_central[std::get<0>(QIEdelay_order[i])]);
        effLLPjet->SetTitle(Form("LLP flagged jets vs Jet ET (central HB, QIE delay %d)",std::get<0>(QIEdelay_order[i])));
        effLLPjet->SetLineWidth(2.);
        effLLPjet->SetLineColor(kBlack);
        effLLPjet->Draw();
        latex->DrawLatex(0.12, 0.85, cmsLabel);
        effLLPjet->Write();
      }
      } */
  }

  
  myfile << "using the following ntuple: " << inputFile << std::endl;
  myfile << "number of colliding bunches = " << numBunch << std::endl;
  myfile << "run luminosity = " << runLum << std::endl;
  myfile << "expected luminosity = " << expectedLum << std::endl;
  myfile << "norm factor used = " << norm << std::endl;
  myfile << "number of good events = " << goodLumiEventCount << std::endl;
  myfile.close(); 
}//closes the function 'rates'
