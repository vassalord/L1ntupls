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

void JetEffs_emu() {

  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextColor(kBlack);
  
  TString cmsLabel = "#scale[0.6]{#bf{CMS}} #scale[0.4]{#it{2022 13.6 TeV Collisions 362085,87 }}";
  float commentaryXpos = 0.6;
  float depthXpos = 0.2;
  
  TCanvas *c1 =new TCanvas("c1", " ", 0, 0,1000,1000);
  c1->Range(0,0,1,1);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetFrameBorderMode(0);
  c1->Draw();
  c1->SetGrid();
  gStyle->SetOptStat(0);

  TFile *f1 = TFile::Open("../bin/l1analysis_new_cond.root");

  for (int delay = -4; delay <= 10; delay += 2) {
    std::cout << delay << std::endl;

    TH1F *h1 = (TH1F*)f1->Get(Form("JetEt_all_emu_delay%d",delay));
    TH1F *h2 = (TH1F*)f1->Get(Form("JetEt_hwQual_emu_delay%d",delay));
    TH1F *h3 = (TH1F*)f1->Get(Form("JetEt_all_central_emu_delay%d",delay));
    TH1F *h4 = (TH1F*)f1->Get(Form("JetEt_hwQual_central_emu_delay%d",delay));

    h1->SetLineColorAlpha(kWhite, 1.);
    h1->SetTitle(Form("Delayed Jet Efficiency vs Jet E_{T} for QIE delay %d (EMU)",delay));
    h1->GetXaxis()->SetTitle("Jet E_{T} [GeV]");
    h1->GetXaxis()->SetTitleSize(0.045);
    h1->GetXaxis()->SetTitleOffset(1.1);
    h1->GetYaxis()->SetTitle("Jet Efficiency");
    h1->GetYaxis()->SetTitleSize(0.045);
    h1->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h1->GetYaxis()->SetRangeUser(0.,1.2);
    h1->GetXaxis()->SetRangeUser(0.,400);
    h1->Draw();
    
    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff2 = 0;
    TEfficiency* pEff5 = 0;
    
    if( TEfficiency::CheckConsistency(*h4,*h3) && TEfficiency::CheckConsistency(*h2,*h1))
      {
	pEff2 = new TEfficiency(*h4,*h3);
	pEff2->SetLineWidth(3.);
	pEff2->SetLineColor(kBlack);
	pEff2->Draw("same");
	pEff5 = new TEfficiency(*h2,*h1);
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
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
  } // delay loop
  
  // pert of code for ieta -16 - 16
  
   for (int delay = -4; delay <= 10; delay += 2) {
    std::cout << delay << std::endl;

    TH1F *h9 = (TH1F*)f1->Get(Form("JetIEta_all_emu_delay%d",delay));
    TH1F *h11 = (TH1F*)f1->Get(Form("JetIEta_LLP_emu_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Delayed Jet Efficiency vs Jet i#eta for QIE delay %d, E > 20 GeV (EMU)",delay));
    h9->GetXaxis()->SetTitle("Jet i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Jet Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h9->GetYaxis()->SetRangeUser(0.,0.3);
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
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_LLPieta(-17-17)%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }



   for (int delay = -4; delay <= 10; delay += 2) {
    //if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h12 = (TH1F*)f1->Get(Form("JetFG_IEta_all_emu_delay%d",delay));
    TH1F *h13 = (TH1F*)f1->Get(Form("JetFG_IEta_zero_emu_delay%d",delay));
    TH1F *h14 = (TH1F*)f1->Get(Form("JetFG_IEta_other_emu_delay%d",delay));

    h12->SetLineColorAlpha(kWhite, 1.);
    h12->SetTitle(Form("Flagged Tower Efficiency vs TP i#eta for QIE delay %d (EMU)",delay));
    h12->GetXaxis()->SetTitle("TP i#eta");
    h12->GetXaxis()->SetTitleSize(0.045);
    h12->GetXaxis()->SetTitleOffset(1.1);
    h12->GetYaxis()->SetTitle("Tower Efficiency");
    h12->GetYaxis()->SetTitleSize(0.045);
    h12->GetYaxis()->SetTitleOffset(1.15);
    //  h1->GetYaxis()->SetRangeUser(0.,1.5);
    h12->GetYaxis()->SetRangeUser(0.,1.4);
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
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.9);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Depth flagged, fg0 = 1", "l");
    legend3->AddEntry(pEff7, "Time flagged, !fg1 && (fg2 || fg3) = 1", "l");
    
    legend3->Draw("same");

    char saveFile[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_FG_ieta%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }
    
    // eff with (fg1 && (fg2 || fg3)) and fg 0 == 1 ) / all

    for (int delay = -4; delay <= 10; delay += 2) {
    //if (delay == 0) continue;
    std::cout << delay << std::endl;
    
    TH1F *h9 = (TH1F*)f1->Get(Form("JetFG_IEta_allp_emu_delay%d",delay));
    TH1F *h13 = (TH1F*)f1->Get(Form("JetFG_IEta_zero_emu_delay%d",delay));
    TH1F *h14 = (TH1F*)f1->Get(Form("JetFG_IEta_other_emu_delay%d",delay));

    h9->SetLineColorAlpha(kWhite, 1.);
    h9->SetTitle(Form("Tower Efficiency vs TP i#eta for QIE delay %d (EMU)",delay));
    h9->GetXaxis()->SetTitle("TP i#eta");
    h9->GetXaxis()->SetTitleSize(0.045);
    h9->GetXaxis()->SetTitleOffset(1.1);
    h9->GetYaxis()->SetTitle("Tower Efficiency");
    h9->GetYaxis()->SetTitleSize(0.045);
    h9->GetYaxis()->SetTitleOffset(1.15);
    h9->GetYaxis()->SetRangeUser(0.,0.012);
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
    legend3->SetTextSize(0.03);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Depth flagged, fg0 = 1", "l");
    legend3->AddEntry(pEff7, "Time flagged, !fg1 && (fg2 || fg3) = 1", "l");

    legend3->Draw("same");

    char saveFile[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_Tower_ieta%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }

 // eff with (!fg1 && (fg2 || fg3))/ (fg1 || fg2 || fg3)

 for (int delay = -4; delay <= 10; delay += 2) {
    //if (delay == 0) continue;
    std::cout << delay << std::endl;

    TH1F *h18 = (TH1F*)f1->Get(Form("JetFG_IEta_fg123_emu_delay%d",delay));
    TH1F *h19 = (TH1F*)f1->Get(Form("JetFG_IEta_other_emu_delay%d",delay));

    h18->SetLineColorAlpha(kWhite, 1.);
    h18->SetTitle(Form("Tower Efficiency vs TP i#eta for QIE delay %d (EMU)",delay));
    h18->GetXaxis()->SetTitle("TP i#eta");
    h18->GetXaxis()->SetTitleSize(0.045);
    h18->GetXaxis()->SetTitleOffset(1.1);
    h18->GetYaxis()->SetTitle("Tower Efficiency");
    h18->GetYaxis()->SetTitleSize(0.045);
    h18->GetYaxis()->SetTitleOffset(1.15);
    h18->GetYaxis()->SetRangeUser(0.,1.3);
    h18->GetXaxis()->SetRangeUser(-17 ,17);
    h18->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff6 = 0;

    if(TEfficiency::CheckConsistency(*h19,*h18))
      {
        pEff6 = new TEfficiency(*h19,*h18);
        pEff6->SetLineWidth(3.);
        pEff6->SetLineColor(kBlue);
        pEff6->Draw("same");
             }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.03);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Time flagged,    #frac{!fg1 && (fg2 || fg3)}{fg1 || fg2 || fg3}", "l");

    legend3->Draw("same");

    char saveFile[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_Tower_ieta_fg123_%d_effs.pdf",delay));
    c1->SaveAs(saveFile);
    }
    
    // eff with (!fg1 && (fg2 || fg3))/ (fg1 || fg2 || fg3) for ieta
    
for (int ieta = -16; ieta <=16; ieta++){
    if (ieta == 0) continue;
    TH1F *h40 = (TH1F*)f1->Get(Form("LLPqieDelay_FG_fg123_emu_%d",ieta));
    TH1F *h41 = (TH1F*)f1->Get(Form("LLPqieDelay_FG_other_emu_%d",ieta));
    
    h40->SetLineColorAlpha(kWhite, 1.);
    h40->SetTitle(Form("Tower Efficiency vs QIE delay for TP i #eta %d (EMU)",ieta));
    h40->GetXaxis()->SetTitle("QIE delay");
    h40->GetXaxis()->SetTitleSize(0.045);
    h40->GetXaxis()->SetTitleOffset(1.1);
    h40->GetYaxis()->SetTitle("Tower Efficiency");
    h40->GetYaxis()->SetTitleSize(0.045);
    h40->GetYaxis()->SetTitleOffset(1.15);
    h40->GetYaxis()->SetRangeUser(0.,1.0);
    h40->GetXaxis()->SetRangeUser(-6 ,12);
    h40->Draw();
    
    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff6 = 0;
    
    if(TEfficiency::CheckConsistency(*h41,*h40))
      {
      pEff6 = new TEfficiency(*h41,*h40);
      pEff6->SetLineWidth(3.);
      pEff6->SetLineColor(kBlue);
      pEff6->Draw("same");
      }
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.03);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Time flagged,    #frac{!fg1 && (fg2 || fg3)}{fg1 || fg2 || fg3}", "l");
    
    legend3->Draw("same");
    
    char saveFile[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile, Form("/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_Tower_ieta_fg123_effs_for_ieta_%d.pdf",ieta));
    c1->SaveAs(saveFile);
}

  TH1F *h5 = (TH1F*)f1->Get("LLPqieDelay_all_emu");
  TH1F *h6 = (TH1F*)f1->Get("LLPqieDelay_emu");
  TH1F *h7 = (TH1F*)f1->Get("LLPqieDelay_central_all_emu");
  TH1F *h8 = (TH1F*)f1->Get("LLPqieDelay_central_emu");
  
  
  // LLP jet vs QIE delay
  
  
  h5->SetLineColorAlpha(kWhite, 1.);
  h5->SetTitle(Form("Delayed Jet Efficiency vs QIE delay (EMU)"));
  h5->GetXaxis()->SetTitle("QIE Delay [ns]");
  h5->GetXaxis()->SetTitleSize(0.045);
  h5->GetXaxis()->SetTitleOffset(1.1);
  h5->GetYaxis()->SetTitle("Jet Efficiency");
  h5->GetYaxis()->SetTitleSize(0.045);
  h5->GetYaxis()->SetTitleOffset(1.15);
  h5->GetYaxis()->SetRangeUser(0.,0.2);
  h5->GetXaxis()->SetRangeUser(-5.,12);
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
      pEff3->SetLineColor(kRed);
      pEff3->Draw("same");
      pEff4 = new TEfficiency(*h8,*h7);
      pEff4->SetLineWidth(3.);
      pEff4->SetLineColor(kBlack);
      pEff4->Draw("same");
    }
  TLegend *legend2 = new TLegend(0.15, 0.7, 0.6, 0.8);
  legend2->SetTextFont(42);
  legend2->SetLineColor(0);
  legend2->SetTextSize(0.03);
  legend2->SetFillColor(0);
  legend2->AddEntry(pEff3, "Delayed L1 jet", "l");
  legend2->AddEntry(pEff4, "Delayed L1 jet, and jet i#eta#leq8", "l");

  legend2->Draw("same");

  char saveFile1[100];
  latex->DrawLatex(0.5, 0.9, cmsLabel);
  sprintf(saveFile1,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Jet_effs.pdf");
  c1->SaveAs(saveFile1);
  
  
  // FG vs QIDdelay
  
  
  TH1F *h20 = (TH1F*)f1->Get("LLPqieDelay_FG_central_zero_emu");
  TH1F *h21 = (TH1F*)f1->Get("LLPqieDelay_FG_zero_emu");
  TH1F *h22 = (TH1F*)f1->Get("LLPqieDelay_FG_central_other_emu");
  TH1F *h23 = (TH1F*)f1->Get("LLPqieDelay_FG_other_emu");
  TH1F *h24 = (TH1F*)f1->Get("LLPqieDelay_FG_all_emu");
  TH1F *h25 = (TH1F*)f1->Get("LLPqieDelay_FG_all_central_emu");
  TH1F *h26 = (TH1F*)f1->Get("LLPqieDelay_FG_fg123_emu");
  TH1F *h27 = (TH1F*)f1->Get("LLPqieDelay_FG_fg0123_emu");
  
  
   TH1F *h44 = (TH1F*)f1->Get("LLPqieDelay_FG_all4_emu");
   TH1F *h45 = (TH1F*)f1->Get("LLPqieDelay_FG_all4_central_emu");
   
   // logic fg0 = 1 and ieta <= 8 for all E > 4GeV
    h21->SetLineColorAlpha(kWhite, 1.);
    h21->SetTitle(Form("Delayed Tower Efficiency vs QIE delay, E > 4GeV (EMU)"));
    h21->GetXaxis()->SetTitle("QIE Delay [ns]");
    h21->GetXaxis()->SetTitleSize(0.045);
    h21->GetXaxis()->SetTitleOffset(1.1);
    h21->GetYaxis()->SetTitle("Tower Efficiency");
    h21->GetYaxis()->SetTitleSize(0.045);
    h21->GetYaxis()->SetTitleOffset(1.15);
    h21->GetYaxis()->SetRangeUser(0.,0.02);
    h21->GetXaxis()->SetRangeUser(-5.,12);
    h21->Draw();
    
    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff21 = 0;
    TEfficiency* pEff22 = 0;
    
    if(TEfficiency::CheckConsistency(*h21,*h44) && TEfficiency::CheckConsistency(*h20,*h45))
            {
                std::cout << "about to do vs QIE plots" << std::endl;
                pEff21 = new TEfficiency(*h21,*h44);
                pEff21->SetLineWidth(3.);
                pEff21->SetLineColor(kRed);
                pEff21->Draw("same");
                pEff22 = new TEfficiency(*h20,*h45);
                pEff22->SetLineWidth(3.);
                pEff22->SetLineColor(kBlack);
                pEff22->Draw("same");
            }
            
    TLegend *legend8 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend8->SetTextFont(42);
    legend8->SetLineColor(0);
    legend8->SetTextSize(0.03);
    legend8->SetFillColor(0);
    legend8->AddEntry(pEff21, "Depth Flagged L1 Tower fg0 = 1", "l");
    legend8->AddEntry(pEff22, "Depth Flagged L1 Tower fg0 = 1, and tower i#eta#leq8", "l");
    
    
    legend8->Draw("same");
    
    char saveFile6[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile6,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_zero_4GeV.pdf");
    c1->SaveAs(saveFile6);

  
  // logic fg0 = 0 and ieta <= 8 for all
  
    h20->SetLineColorAlpha(kWhite, 1.);
    h20->SetTitle(Form("Delayed Tower Efficiency vs QIE delay (EMU)"));
    h20->GetXaxis()->SetTitle("QIE Delay [ns]");
    h20->GetXaxis()->SetTitleSize(0.045);
    h20->GetXaxis()->SetTitleOffset(1.1);
    h20->GetYaxis()->SetTitle("Tower Efficiency");
    h20->GetYaxis()->SetTitleSize(0.045);
    h20->GetYaxis()->SetTitleOffset(1.15);
    h20->GetYaxis()->SetRangeUser(0.,0.002);
    h20->GetXaxis()->SetRangeUser(-5.,12);
    h20->Draw();
                      
    c1->Update();
    c1->RedrawAxis();
                          
    TEfficiency* pEff6 = 0;
    TEfficiency* pEff7 = 0;
    
    if(TEfficiency::CheckConsistency(*h21,*h24) && TEfficiency::CheckConsistency(*h20,*h25))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff6 = new TEfficiency(*h21,*h24);
            pEff6->SetLineWidth(3.);
            pEff6->SetLineColor(kRed);
            pEff6->Draw("same");
            pEff7 = new TEfficiency(*h20,*h25);
            pEff7->SetLineWidth(3.);
            pEff7->SetLineColor(kBlack);
            pEff7->Draw("same");
        }
        
    TLegend *legend3 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend3->SetTextFont(42);
    legend3->SetLineColor(0);
    legend3->SetTextSize(0.03);
    legend3->SetFillColor(0);
    legend3->AddEntry(pEff6, "Depth Flagged L1 Tower fg0 = 1", "l");
    legend3->AddEntry(pEff7, "Depth Flagged L1 Tower fg0 = 1, and tower i#eta#leq8", "l");
                
                  
    legend3->Draw("same");
                  
    char saveFile[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_zero.pdf");
    c1->SaveAs(saveFile);
    
    
    // logic !fg1 && (fg2 || fg3) and ieta <= 8 for all E > 4GeV

    h45->SetLineColorAlpha(kWhite, 1.);
    h45->SetTitle(Form("Delayed Tower Efficiency vs QIE delay, E > 4GeV (EMU)"));
    h45->GetXaxis()->SetTitle("QIE Delay [ns]");
    h45->GetXaxis()->SetTitleSize(0.045);
    h45->GetXaxis()->SetTitleOffset(1.1);
    h45->GetYaxis()->SetTitle("Tower Efficiency");
    h45->GetYaxis()->SetTitleSize(0.045);
    h45->GetYaxis()->SetTitleOffset(1.15);
    h45->GetYaxis()->SetRangeUser(0.,0.2);
    h45->GetXaxis()->SetRangeUser(-5.,12);
    h45->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff26 = 0;
    TEfficiency* pEff27 = 0;

    if(TEfficiency::CheckConsistency(*h23,*h44) && TEfficiency::CheckConsistency(*h22,*h45))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff26 = new TEfficiency(*h23,*h44);
            pEff26->SetLineWidth(3.);
            pEff26->SetLineColor(kRed);
            pEff26->Draw("same");
            pEff27 = new TEfficiency(*h22,*h45);
            pEff27->SetLineWidth(3.);
            pEff27->SetLineColor(kBlack);
            pEff27->Draw("same");
        }

    TLegend *legend10 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend10->SetTextFont(42);
    legend10->SetLineColor(0);
    legend10->SetTextSize(0.03);
    legend10->SetFillColor(0);
    legend10->AddEntry(pEff26, "Time Flagged L1 Tower !fg1 && (fg2 || fg3) ", "l");
    legend10->AddEntry(pEff27, "Time Flagged L1 Tower !fg1 && (fg2 || fg3), and tower i#eta#leq8", "l");


    legend10->Draw("same");

    char saveFile8[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile8,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_other_E4GeV.pdf");
    c1->SaveAs(saveFile8);
    
    
    // logic !fg1 && (fg2 || fg3) and ieta <= 8 for all

    h22->SetLineColorAlpha(kWhite, 1.);
    h22->SetTitle(Form("Delayed Tower Efficiency vs QIE delay (EMU)"));
    h22->GetXaxis()->SetTitle("QIE Delay [ns]");
    h22->GetXaxis()->SetTitleSize(0.045);
    h22->GetXaxis()->SetTitleOffset(1.1);
    h22->GetYaxis()->SetTitle("Tower Efficiency");
    h22->GetYaxis()->SetTitleSize(0.045);
    h22->GetYaxis()->SetTitleOffset(1.15);
    h22->GetYaxis()->SetRangeUser(0.,0.014);
    h22->GetXaxis()->SetRangeUser(-5.,12);
    h22->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff8 = 0;
    TEfficiency* pEff9 = 0;

    if(TEfficiency::CheckConsistency(*h23,*h24) && TEfficiency::CheckConsistency(*h22,*h25))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff8 = new TEfficiency(*h23,*h24);
            pEff8->SetLineWidth(3.);
            pEff8->SetLineColor(kRed);
            pEff8->Draw("same");
            pEff9 = new TEfficiency(*h22,*h25);
            pEff9->SetLineWidth(3.);
            pEff9->SetLineColor(kBlack);
            pEff9->Draw("same");
        }

    TLegend *legend4 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend4->SetTextFont(42);
    legend4->SetLineColor(0);
    legend4->SetTextSize(0.03);
    legend4->SetFillColor(0);
    legend4->AddEntry(pEff6, "Time Flagged L1 Tower !fg1 && (fg2 || fg3) ", "l");
    legend4->AddEntry(pEff7, "Time Flagged L1 Tower !fg1 && (fg2 || fg3), and tower i#eta#leq8", "l");    
    

    legend4->Draw("same");

    char saveFile2[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile2,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_other.pdf");
    c1->SaveAs(saveFile2);
    
    
    // logic (!fg1 && (fg2 || fg3))/ (fg1 || fg2 || fg3)

    h22->SetLineColorAlpha(kWhite, 1.);
    h22->SetTitle(Form("Delayed Tower Efficiency vs QIE delay (EMU)"));
    h22->GetXaxis()->SetTitle("QIE Delay [ns]");
    h22->GetXaxis()->SetTitleSize(0.045);
    h22->GetXaxis()->SetTitleOffset(1.1);
    h22->GetYaxis()->SetTitle("Tower Efficiency");
    h22->GetYaxis()->SetTitleSize(0.045);
    h22->GetYaxis()->SetTitleOffset(1.15);
    h22->GetYaxis()->SetRangeUser(0.,1.3);
    h22->GetXaxis()->SetRangeUser(-5.,12);
    h22->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff10 = 0;

    if(TEfficiency::CheckConsistency(*h23,*h26))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff10 = new TEfficiency(*h23,*h26);
            pEff10->SetLineWidth(3.);
            pEff10->SetLineColor(kBlack);
            pEff10->Draw("same");
        }

    TLegend *legend5 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend5->SetTextFont(42);
    legend5->SetLineColor(0);
    legend5->SetTextSize(0.03);
    legend5->SetFillColor(0);
    legend5->AddEntry(pEff10, "Time Flagged L1 Tower    #frac{!fg1 && (fg2 || fg3)}{fg1 || fg2 || fg3} ", "l");


    legend5->Draw("same");

    char saveFile3[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile3,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_other_and_fg123.pdf");
    c1->SaveAs(saveFile3);

    // logic fg0 = 0 and (!fg1 && (fg2 || fg3)) for fg0 || (!fg1 && (fg2 || fg3)

    h23->SetLineColorAlpha(kWhite, 1.);
    h23->SetTitle(Form("Delayed Tower Efficiency vs QIE delay (EMU)"));
    h23->GetXaxis()->SetTitle("QIE Delay [ns]");
    h23->GetXaxis()->SetTitleSize(0.045);
    h23->GetXaxis()->SetTitleOffset(1.1);
    h23->GetYaxis()->SetTitle("Tower Efficiency");
    h23->GetYaxis()->SetTitleSize(0.045);
    h23->GetYaxis()->SetTitleOffset(1.15);
    h23->GetYaxis()->SetRangeUser(0.,1.4);
    h23->GetXaxis()->SetRangeUser(-5.,12);
    h23->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff11 = 0;
    TEfficiency* pEff12 = 0;

    if(TEfficiency::CheckConsistency(*h21,*h27) && TEfficiency::CheckConsistency(*h23,*h27))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff11 = new TEfficiency(*h21,*h27);
            pEff11->SetLineWidth(3.);
            pEff11->SetLineColor(kRed);
            pEff11->Draw("same");
            pEff12 = new TEfficiency(*h23,*h27);
            pEff12->SetLineWidth(3.);
            pEff12->SetLineColor(kBlack);
            pEff12->Draw("same");
        }

    TLegend *legend6 = new TLegend(0.15, 0.7, 0.6, 0.8);
    legend6->SetTextFont(42);
    legend6->SetLineColor(0);
    legend6->SetTextSize(0.02);
    legend6->SetFillColor(0);
    legend6->AddEntry(pEff11, "Depth Flagged L1 Tower    #frac{fg0 = 1}{fg0 || (!fg1 && (fg2 || fg3))} ", "l");
    legend6->AddEntry(pEff12, "Time Flagged L1 Tower     #frac{(!fg1 && (fg2 || fg3))}{fg0 || (!fg1 && (fg2 || fg3))}", "l");


    legend6->Draw("same");

    char saveFile4[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile4,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Tower_effs_other_zero_fg0123.pdf");
    c1->SaveAs(saveFile4);
    

   // for jet ET and hwQual all delay
    
    TH1F *h28 = (TH1F*)f1->Get("JetEt_all_emu_delay-4");
    TH1F *h29 = (TH1F*)f1->Get("JetEt_all_emu_delay-2");
    TH1F *h30 = (TH1F*)f1->Get("JetEt_all_emu_delay0");
    TH1F *h31 = (TH1F*)f1->Get("JetEt_all_emu_delay2");
    TH1F *h32 = (TH1F*)f1->Get("JetEt_all_emu_delay4");
    TH1F *h33 = (TH1F*)f1->Get("JetEt_all_emu_delay6");
    TH1F *h34 = (TH1F*)f1->Get("JetEt_all_emu_delay8");
    TH1F *h35 = (TH1F*)f1->Get("JetEt_all_emu_delay10");
    
    TH1F *h36 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay-4");
    TH1F *h37 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay-2");
    TH1F *h38 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay0");
    TH1F *h39 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay2");
    TH1F *h40 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay4");
    TH1F *h41 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay6");
    TH1F *h42 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay8");
    TH1F *h43 = (TH1F*)f1->Get("JetEt_hwQual_emu_delay10");
    
    h28->SetLineColorAlpha(kWhite, 1.);
    h28->SetTitle(Form("Delayed Jet Efficiency vs Jet E_{T} (EMU)"));
    h28->GetXaxis()->SetTitle("Jet E_{T} [GeV]");
    h28->GetXaxis()->SetTitleSize(0.045);
    h28->GetXaxis()->SetTitleOffset(1.1);
    h28->GetYaxis()->SetTitle("Jet Efficiency");
    h28->GetYaxis()->SetTitleSize(0.045);
    h28->GetYaxis()->SetTitleOffset(1.15);
    h28->GetYaxis()->SetRangeUser(0.,1.2);
    h28->GetXaxis()->SetRangeUser(0,400);
    h28->Draw();
    
    c1->Update();
    c1->RedrawAxis();
    
    TEfficiency* pEff13 = 0;
    TEfficiency* pEff14 = 0;
    TEfficiency* pEff15 = 0;
    TEfficiency* pEff16 = 0;
    TEfficiency* pEff17 = 0;
    TEfficiency* pEff18 = 0;
    TEfficiency* pEff19 = 0;
    TEfficiency* pEff20 = 0;
    
    if(TEfficiency::CheckConsistency(*h36,*h28) && TEfficiency::CheckConsistency(*h37,*h29) && TEfficiency::CheckConsistency(*h38,*h30) && TEfficiency::CheckConsistency(*h39,*h31) && TEfficiency::CheckConsistency(*h40,*h32) && TEfficiency::CheckConsistency(*h41,*h33) && TEfficiency::CheckConsistency(*h42,*h34) && TEfficiency::CheckConsistency(*h43,*h35))
            {
                std::cout << "about to do vs Et plots" << std::endl;
                pEff13 = new TEfficiency(*h36,*h28);
                pEff13->SetLineWidth(3.);
                pEff13->SetLineColor(kRed);
                pEff13->Draw("same");
                
                pEff14 = new TEfficiency(*h37,*h29);
                pEff14->SetLineWidth(3.);
                pEff14->SetLineColor(kOrange-3);
                pEff14->Draw("same");
                
                pEff15 = new TEfficiency(*h38,*h30);
                pEff15->SetLineWidth(3.);
                pEff15->SetLineColor(kYellow-3);
                pEff15->Draw("same");
                
                pEff16 = new TEfficiency(*h39,*h31);
                pEff16->SetLineWidth(3.);
                pEff16->SetLineColor(kGreen+2);
                pEff16->Draw("same");
                
                pEff17 = new TEfficiency(*h40,*h32);
                pEff17->SetLineWidth(3.);
                pEff17->SetLineColor(kBlue);
                pEff17->Draw("same");
                
                pEff18 = new TEfficiency(*h41,*h33);
                pEff18->SetLineWidth(3.);
                pEff18->SetLineColor(kAzure+6);
                pEff18->Draw("same");
                
                pEff19 = new TEfficiency(*h42,*h34);
                pEff19->SetLineWidth(3.);
                pEff19->SetLineColor(kGray+1);
                pEff19->Draw("same");
                
                pEff20 = new TEfficiency(*h43,*h35);
                pEff20->SetLineWidth(3.);
                pEff20->SetLineColor(kBlack);
                pEff20->Draw("same");
                
            }
    TLegend *legend7 = new TLegend(0.15, 0.7, 0.6, 0.9);
    legend7->SetTextFont(42);
    legend7->SetLineColor(0);
    legend7->SetTextSize(0.02);
    legend7->SetFillColor(0);
    legend7->AddEntry(pEff13, "Delayed L1 jet vs E_{T} delay -4 ", "l");
    legend7->AddEntry(pEff14, "Delayed L1 jet vs E_{T} delay -2", "l");
    legend7->AddEntry(pEff15, "Delayed L1 jet vs E_{T} delay 0", "l");
    legend7->AddEntry(pEff16, "Delayed L1 jet vs E_{T} delay 2", "l");
    legend7->AddEntry(pEff17, "Delayed L1 jet vs E_{T} delay 4", "l");
    legend7->AddEntry(pEff18, "Delayed L1 jet vs E_{T} delay 6", "l");
    legend7->AddEntry(pEff19, "Delayed L1 jet vs E_{T} delay 8", "l");
    legend7->AddEntry(pEff20, "Delayed L1 jet vs E_{T} delay 10", "l");
    
    
    
    legend7->Draw("same");
    
    char saveFile5[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile5,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Jet_Et_all_delay.pdf");
    c1->SaveAs(saveFile5);


   // calo tower

   TH1F *h46 = (TH1F*)f1->Get("TT_LLPhwQual_HB_emu");
   TH1F *h47 = (TH1F*)f1->Get("TT_LLP_HB_emu");

    h46->SetLineColorAlpha(kWhite, 1.);
    h46->SetTitle(Form("Calo Tower Efficiency vs QIE delay (EMU)"));
    h46->GetXaxis()->SetTitle("QIE Delay [ns]");
    h46->GetXaxis()->SetTitleSize(0.045);
    h46->GetXaxis()->SetTitleOffset(1.1);
    h46->GetYaxis()->SetTitle("Tower Efficiency");
    h46->GetYaxis()->SetTitleSize(0.045);
    h46->GetYaxis()->SetTitleOffset(1.15);
    h46->GetYaxis()->SetRangeUser(0.,1.4);
    h46->GetXaxis()->SetRangeUser(-5.,12);
    h46->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff25 = 0;

    if(TEfficiency::CheckConsistency(*h46,*h47) && TEfficiency::CheckConsistency(*h46,*h47))
        {
            std::cout << "about to do vs QIE plots" << std::endl;
            pEff25 = new TEfficiency(*h46,*h47);
            pEff25->SetLineWidth(3.);
            pEff25->SetLineColor(kBlack);
            pEff25->Draw("same");
        }

    TLegend *legend9 = new TLegend(0.15, 0.7, 0.6, 0.9);
    legend9->SetTextFont(42);
    legend9->SetLineColor(0);
    legend9->SetTextSize(0.02);
    legend9->SetFillColor(0);
    legend9->AddEntry(pEff25, "CaloTower_eff ", "l");


    legend9->Draw("same");

    char saveFile7[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    sprintf(saveFile7,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/Calo_Tower_effs.pdf");
    c1->SaveAs(saveFile7);



    // tower eff all delay (!fg1 && (fg2 || fg3))/ (fg1 || fg2 || fg3)

    TH1F *h50 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay-4");
    TH1F *h51 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay-2");
    TH1F *h52 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay0");
    TH1F *h53 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay2");
    TH1F *h54 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay4");
    TH1F *h55 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay6");
    TH1F *h56 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay8");
    TH1F *h57 = (TH1F*)f1->Get("JetFG_IEta_fg123_emu_delay10");



    TH1F *h60 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay-4");
    TH1F *h61 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay-2");
    TH1F *h62 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay0");
    TH1F *h63 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay2");
    TH1F *h64 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay4");
    TH1F *h65 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay6");
    TH1F *h66 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay8");
    TH1F *h67 = (TH1F*)f1->Get("JetFG_IEta_other_emu_delay10");


    h30->SetLineColorAlpha(kWhite, 1.);
    h30->SetTitle(Form("Tower Efficiency vs TP i#eta for all QIE delay (EMU)"));
    h30->GetXaxis()->SetTitle("TP i#eta");
    h30->GetXaxis()->SetTitleSize(0.045);
    h30->GetXaxis()->SetTitleOffset(1.1);
    h30->GetYaxis()->SetTitle("Tower Efficiency");
    h30->GetYaxis()->SetTitleSize(0.045);
    h30->GetYaxis()->SetTitleOffset(1.15);
    h30->GetYaxis()->SetRangeUser(0.,1.3);
    h30->GetXaxis()->SetRangeUser(-17 ,17);
    h30->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff30 = 0;
    TEfficiency* pEff31 = 0;
    TEfficiency* pEff32 = 0;
    TEfficiency* pEff33 = 0;
    TEfficiency* pEff34 = 0;
    TEfficiency* pEff35 = 0;
    TEfficiency* pEff36 = 0;
    TEfficiency* pEff37 = 0;


    if(TEfficiency::CheckConsistency(*h60,*h50) && TEfficiency::CheckConsistency(*h61,*h51) && TEfficiency::CheckConsistency(*h62,*h52) && TEfficiency::CheckConsistency(*h63,*h53) && TEfficiency::CheckConsistency(*h64,*h54) && TEfficiency::CheckConsistency(*h65,*h55) && TEfficiency::CheckConsistency(*h66,*h56) && TEfficiency::CheckConsistency(*h67,*h57))
    {
             std::cout << "about to do vs Et plots" << std::endl;
                pEff30 = new TEfficiency(*h60,*h50);
                pEff30->SetLineWidth(3.);
                pEff30->SetLineColor(kRed);
                pEff30->Draw("same");

                pEff31 = new TEfficiency(*h61,*h51);
                pEff31->SetLineWidth(3.);
                pEff31->SetLineColor(kOrange-3);
                pEff31->Draw("same");

                pEff32 = new TEfficiency(*h62,*h52);
                pEff32->SetLineWidth(3.);
                pEff32->SetLineColor(kYellow-3);
                pEff32->Draw("same");

                pEff33 = new TEfficiency(*h63,*h53);
                pEff33->SetLineWidth(3.);
                pEff33->SetLineColor(kGreen);
                pEff33->Draw("same");

                pEff34 = new TEfficiency(*h64,*h54);
                pEff34->SetLineWidth(3.);
                pEff34->SetLineColor(kBlue);
                pEff34->Draw("same");

                pEff35 = new TEfficiency(*h65,*h55);
                pEff35->SetLineWidth(3.);
                pEff35->SetLineColor(kAzure+6);
                pEff35->Draw("same");

                pEff36 = new TEfficiency(*h66,*h56);
                pEff36->SetLineWidth(3.);
                pEff36->SetLineColor(kBlack);
                pEff36->Draw("same");

                pEff37 = new TEfficiency(*h67,*h57);
                pEff37->SetLineWidth(3.);
                pEff37->SetLineColor(kGray);
                pEff37->Draw("same");





             }
    TLegend *legend11 = new TLegend(0.15, 0.75, 0.6, 0.9);
    legend11->SetTextFont(42);
    legend11->SetLineColor(0);
    legend11->SetTextSize(0.03);
    legend11->SetFillColor(0);
    legend11->AddEntry(pEff30, "Time flagged, delay -4", "l");
    legend11->AddEntry(pEff31, "Time flagged, delay -2", "l");
    legend11->AddEntry(pEff32, "Time flagged, delay 0", "l");
    legend11->AddEntry(pEff33, "Time flagged, delay 2", "l");
    legend11->AddEntry(pEff34, "Time flagged, delay 4", "l");
    legend11->AddEntry(pEff35, "Time flagged, delay 6", "l");
    legend11->AddEntry(pEff36, "Time flagged, delay 8", "l");
    legend11->AddEntry(pEff37, "Time flagged, delay 10", "l");

    legend11->Draw("same");

    char saveFile11[100];
    latex->DrawLatex(0.5, 0.9, cmsLabel);
    latex->DrawLatex(0.6, 0.8, "#scale[0.5]{#frac{!fg1 && (fg2 || fg3)}{fg1 || fg2 || fg3}}");
    sprintf(saveFile11,"/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_Tower_ieta_fg123_all_effs.pdf");
    c1->SaveAs(saveFile11);


   // eff with (!fg1 && (fg2 || fg3))/ (fg1 || fg2 || fg3)
    TH1F *h70 = (TH1F*)f1->Get("LLPqieDelay_FG_fg123_emu_1");
    TH1F *h71 = (TH1F*)f1->Get("LLPqieDelay_FG_fg123_emu_8");
    TH1F *h72 = (TH1F*)f1->Get("LLPqieDelay_FG_fg123_emu_15");

    TH1F *h73 = (TH1F*)f1->Get("LLPqieDelay_FG_other_emu_1");
    TH1F *h74 = (TH1F*)f1->Get("LLPqieDelay_FG_other_emu_8");
    TH1F *h75 = (TH1F*)f1->Get("LLPqieDelay_FG_other_emu_15");

    h70->SetLineColorAlpha(kWhite, 1.);
    h70->SetTitle("Tower Efficiency vs QIE delay for TP i #eta 1, 8, 15 (EMU)");
    h70->GetXaxis()->SetTitle("QIE delay");
    h70->GetXaxis()->SetTitleSize(0.045);
    h70->GetXaxis()->SetTitleOffset(1.1);
    h70->GetYaxis()->SetTitle("Tower Efficiency");
    h70->GetYaxis()->SetTitleSize(0.045);
    h70->GetYaxis()->SetTitleOffset(1.15);
    h70->GetYaxis()->SetRangeUser(0.,1.3);
    h70->GetXaxis()->SetRangeUser(-6 ,12);
    h70->Draw();

    c1->Update();
    c1->RedrawAxis();

    TEfficiency* pEff40 = 0;
    TEfficiency* pEff41 = 0;
    TEfficiency* pEff42 = 0;

    if(TEfficiency::CheckConsistency(*h73,*h70) && TEfficiency::CheckConsistency(*h74,*h71) && TEfficiency::CheckConsistency(*h75,*h72))
          {
            pEff40 = new TEfficiency(*h73,*h70);
            pEff40->SetLineWidth(3.);
            pEff40->SetLineColor(kBlue);
            pEff40->Draw("same");

            pEff41 = new TEfficiency(*h74,*h71);
            pEff41->SetLineWidth(3.);
            pEff41->SetLineColor(kRed);
            pEff41->Draw("same");

            pEff42 = new TEfficiency(*h75,*h72);
            pEff42->SetLineWidth(3.);
            pEff42->SetLineColor(kBlack);
            pEff42->Draw("same");
          }
        TLegend *legend12 = new TLegend(0.15, 0.7, 0.6, 0.8);
        legend12->SetTextFont(42);
        legend12->SetLineColor(0);
        legend12->SetTextSize(0.03);
        legend12->SetFillColor(0);
        legend12->AddEntry(pEff40, "Time flagged, i #eta 1", "l");
        legend12->AddEntry(pEff41, "Time flagged, i #eta 8", "l");
        legend12->AddEntry(pEff42, "Time flagged, i #eta 15 ", "l");
        legend12->Draw("same");

        char saveFile12[100];
        latex->DrawLatex(0.7, 0.8, "#scale[0.5]{#frac{!fg1 && (fg2 || fg3)}{fg1 || fg2 || fg3}}");
        sprintf(saveFile12, "/afs/cern.ch/user/s/shoienko/CMSSW_12_6_0_pre1/src/HcalTrigger/Validation/PlotEffs/Plot_emu/QIE_Tower_ieta_fg123_effs_diff_ieta.pdf");
        c1->SaveAs(saveFile12);
}
