#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <new>
#include<climits>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TEfficiency.h"
#include "TF1.h"
#include "TH2.h"
#include "TProfile.h"

#include "TStyle.h"
#include "TAttFill.h"
#include "TPaveStats.h"
#include "TMinuit.h"
#include "TPostScript.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TRandom.h"
#include "TPaletteAxis.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"
#include "TCanvas.h"

using namespace std;

void JetEffs() {
  TCanvas *c1 =new TCanvas("c1", " ", 0, 0,1000,1000);
  c1->Range(0,0,1,1);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetFrameBorderMode(0);
  c1->Draw();
  c1->SetGrid();
  gStyle->SetOptStat(0);

  TFile *f1 = TFile::Open("../l1analysis_new_cond.root");

  for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h1 = (TH1F*)f1->Get(Form("JetEt_all_delay%d",delay));
    TH1F *h2 = (TH1F*)f1->Get(Form("JetEt_hwQual_delay%d",delay));
    TH1F *h3 = (TH1F*)f1->Get(Form("JetEt_all_central_delay%d",delay));
    TH1F *h4 = (TH1F*)f1->Get(Form("JetEt_hwQual_central_delay%d",delay));
    TH1F *h10 = (TH1F*)f1->Get(Form("JetET_LLP_delay%d",delay));

    h1->SetLineColorAlpha(kWhite, 1.);
    h1->SetTitle(Form("Delayed Jet Efficiency vs Jet E_{T} for QIE delay %d",delay));
    h1->GetXaxis()->SetTitle("Jet E_{T} [GeV]");
    h1->GetXaxis()->SetTitleSize(0.045);
    h1->GetXaxis()->SetTitleOffset(1.1);
    h1->GetYaxis()->SetTitle("Jet Efficiency");
    h1->GetYaxis()->SetTitleSize(0.045);
    h1->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h1->GetYaxis()->SetRangeUser(0.,0.15);
    h1->GetXaxis()->SetRangeUser(0.,400);
    h1->Draw();
    
    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff2 = 0;
    TEfficiency* pEff5 = 0;
    
    if( TEfficiency::CheckConsistency(*h4,*h3) && TEfficiency::CheckConsistency(*h10,*h1))
      {
	pEff2 = new TEfficiency(*h4,*h3);
	pEff2->SetLineWidth(3.);
	pEff2->SetLineColor(kBlack);
	pEff2->Draw("same");
	pEff5 = new TEfficiency(*h10,*h1);
	pEff5->SetLineWidth(3.);
	pEff5->SetLineColor(kRed);
	pEff5->Draw("same");
      }
    
    TLegend *legend1 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend1->SetTextFont(42);
    legend1->SetLineColor(0);
    legend1->SetTextSize(0.04);
    legend1->SetFillColor(0);
    legend1->AddEntry(pEff2, "Delayed L1 jet, and jet i#eta#leq8 vs E_{T}", "l");
    legend1->AddEntry(pEff5, "Delayed L1 jet vs E_{T}", "l");
    
    legend1->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
  } // delay loop
  
  // pert of code for ieta -16 - 16
  
   for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h9 = (TH1F*)f1->Get(Form("JetIEta_all_delay%d",delay));
    TH1F *h11 = (TH1F*)f1->Get(Form("JetIEta_LLP_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Delayed Jet Efficiency vs Jet i#eta for QIE delay %d",delay));
    h9->GetXaxis()->SetTitle("Jet i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Jet Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h9->GetYaxis()->SetRangeUser(0.,0.02);
    h9->GetXaxis()->SetRangeUser(0 ,17);
    h9->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff6 = 0;

    if(TEfficiency::CheckConsistency(*h11,*h9))
      {
        pEff6 = new TEfficiency(*h11,*h9);
        pEff6->SetLineWidth(3.);
        pEff6->SetLineColor(kBlue);
        pEff6->Draw("same");
      }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Delayed L1 jet vs i#eta", "l");

    legend3->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE_LLPieta(0-17)%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }
    
 // pert of code for ieta 1-16
   for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h9 = (TH1F*)f1->Get(Form("JetIEta_all_delay%d",delay));
    TH1F *h11 = (TH1F*)f1->Get(Form("JetIEta_LLP_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Delayed Jet Efficiency vs Jet i#eta for QIE delay %d",delay));
    h9->GetXaxis()->SetTitle("Jet i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Jet Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h9->GetYaxis()->SetRangeUser(0.,0.02);
    h9->GetXaxis()->SetRangeUser(-17 ,17);
    h9->Draw();

    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff6 = 0;

    if(TEfficiency::CheckConsistency(*h11,*h9))
      {
        pEff6 = new TEfficiency(*h11,*h9);
        pEff6->SetLineWidth(3.);
        pEff6->SetLineColor(kBlue);
        pEff6->Draw("same");
      }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Delayed L1 jet vs i#eta", "l");

    legend3->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE_LLPieta(-17 - 17)%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
  } // delay loop 


// pert of code for ieta -16 - 0
 for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h9 = (TH1F*)f1->Get(Form("JetIEta_all_delay%d",delay));
    TH1F *h11 = (TH1F*)f1->Get(Form("JetIEta_LLP_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Delayed Jet Efficiency vs Jet i#eta for QIE delay %d",delay));
    h9->GetXaxis()->SetTitle("Jet i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Jet Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h9->GetYaxis()->SetRangeUser(0.,0.02);
    h9->GetXaxis()->SetRangeUser(-17 ,0);
    h9->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff7 = 0;

    if(TEfficiency::CheckConsistency(*h11,*h9))
      {
        pEff7 = new TEfficiency(*h11,*h9);
        pEff7->SetLineWidth(3.);
        pEff7->SetLineColor(kBlue);
        pEff7->Draw("same");
      }
    TLegend *legend4 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend4->SetTextFont(42);
    legend4->SetLineColor(0);
    legend4->SetTextSize(0.04);
    legend4->SetFillColor(0);
    legend4->AddEntry(pEff7, "Delayed L1 jet vs i#eta", "l");

    legend4->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE_LLPieta(-17-0)%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }
    
    // for depth

   for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h12 = (TH1F*)f1->Get(Form("JetFG_IEta_all_delay%d",delay));
    TH1F *h13 = (TH1F*)f1->Get(Form("JetFG_IEta_zero_delay%d",delay));
    TH1F *h14 = (TH1F*)f1->Get(Form("JetFG_IEta_other_delay%d",delay));

    h12->SetLineColorAlpha(kWhite, 1.);
    h12->SetTitle(Form("Flagged Tower Efficiency vs TP i#eta for QIE delay %d",delay));
    h12->GetXaxis()->SetTitle("TP i#eta");
    h12->GetXaxis()->SetTitleSize(0.045);
    h12->GetXaxis()->SetTitleOffset(1.1);
    h12->GetYaxis()->SetTitle("Tower Efficiency");
    h12->GetYaxis()->SetTitleSize(0.045);
    h12->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h12->GetYaxis()->SetRangeUser(0.,1.0);
    h12->GetXaxis()->SetRangeUser(-17 ,17);
    h12->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff6 = 0;
    TEfficiency* pEff7 = 0;

    if(TEfficiency::CheckConsistency(*h13,*h12) && TEfficiency::CheckConsistency(*h14,*h12))
      {
        pEff6 = new TEfficiency(*h13,*h12);
        pEff6->SetLineWidth(3.);
        pEff6->SetLineColor(kBlue);
        pEff6->Draw("same");
        pEff7 = new TEfficiency(*h14,*h12);
        pEff7->SetLineWidth(3.);
        pEff7->SetLineColor(kRed);
        pEff7->Draw("same");
             }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Depth flagged, fg0 = 1", "l");
    legend3->AddEntry(pEff7, "Time flagged, !fg1 && (fg2 || fg3)", "l");
    
    legend3->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE_FG_ieta%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }

    for (int delay = -2; delay <= 8; delay += 2) {
    if (delay == 0) continue;
    std::cout << delay << std::endl;
    
    TH1F *h9 = (TH1F*)f1->Get(Form("JetIEta_all_delay%d",delay));
    TH1F *h13 = (TH1F*)f1->Get(Form("JetFG_IEta_zero_delay%d",delay));
    TH1F *h14 = (TH1F*)f1->Get(Form("JetFG_IEta_other_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Tower Efficiency vs TP i#eta for QIE delay %d",delay));
    h9->GetXaxis()->SetTitle("TP i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Tower Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    h9->GetYaxis()->SetRangeUser(0.,0.2);
    h9->GetXaxis()->SetRangeUser(-17 ,17);
    h9->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff6 = 0;
    TEfficiency* pEff7 = 0;

    if(TEfficiency::CheckConsistency(*h13,*h9) && TEfficiency::CheckConsistency(*h14,*h9))
      {
        pEff6 = new TEfficiency(*h13,*h9);
        pEff6->SetLineWidth(3.);
        pEff6->SetLineColor(kBlue);
        pEff6->Draw("same");
        pEff7 = new TEfficiency(*h14,*h9);
        pEff7->SetLineWidth(3.);
        pEff7->SetLineColor(kRed);
        pEff7->Draw("same");
             }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Depth flagged, fg0 = 1", "l");
    legend3->AddEntry(pEff7, "Time flagged, !fg1 && (fg2 || fg3)", "l");

    legend3->Draw("same");

    char saveFile[100];
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot/QIE_Tower_ieta%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }

 /* TH1F *h5 = (TH1F*)f1->Get("LLPqieDelay_all");
  TH1F *h6 = (TH1F*)f1->Get("LLPqieDelay");
  TH1F *h7 = (TH1F*)f1->Get("LLPqieDelay_central_all");
  TH1F *h8 = (TH1F*)f1->Get("LLPqieDelay_central");
  // LLP jet vs QIE delay
  h5->SetLineColorAlpha(kWhite, 1.);
  h5->SetTitle(Form("Delayed Jet Efficiency vs QIE delay"));
  h5->GetXaxis()->SetTitle("QIE Delay [ns]");
  h5->GetXaxis()->SetTitleSize(0.045);
  h5->GetXaxis()->SetTitleOffset(1.1);
  h5->GetYaxis()->SetTitle("Jet Efficiency");
  h5->GetYaxis()->SetTitleSize(0.045);
  h5->GetYaxis()->SetTitleOffset(1.15);
  h5->GetYaxis()->SetRangeUser(0.,0.01);
  h5->GetXaxis()->SetRangeUser(-3.,10);
  h5->Draw();

  c1->Update();
  c1->RedrawAxis();

  TEfficiency* pEff3 = 0;
  TEfficiency* pEff4 = 0;

  if(TEfficiency::CheckConsistency(*h6,*h5) && TEfficiency::CheckConsistency(*h8,*h7))
    {
      std::cout << "about to do vs QIE plots" << std::endl;
      pEff3 = new TEfficiency(*h6,*h5);
      pEff3->SetLineWidth(3.);
      pEff3->SetLineColor(kGray);
      pEff3->Draw("same");
      pEff4 = new TEfficiency(*h8,*h7);
      pEff4->SetLineWidth(3.);
      pEff4->SetLineColor(kBlack);
      pEff4->Draw("same");
    }
  TLegend *legend2 = new TLegend(0.15, 0.7, 0.6, 0.8);
  legend2->SetTextFont(42);
  legend2->SetLineColor(0);
  legend2->SetTextSize(0.04);
  legend2->SetFillColor(0);
  legend2->AddEntry(pEff3, "Delayed L1 jet", "l");
  legend2->AddEntry(pEff4, "Delayed L1 jet, and jet i#eta#leq8", "l");

  legend2->Draw("same");

  char saveFile[100];
  sprintf(saveFile,"/eos/user/g/gkopp/www/L1plots/Jet_effs.pdf");
  c1->SaveAs(saveFile);*/
}
