#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

Analyzer::Analyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ZZ4lAnalysis.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ZZ4lAnalysis.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("ZZ4lAnalysis.root:/ZZTree");
      dir->GetObject("candTree",tree);

   }
   Init(tree);
}

Analyzer::~Analyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}


void Analyzer::PlotHistogram(TString input_file_name)
{
   c = new TCanvas("c", "c", 900, 900);

   c->Divide(2, 2);

   c->cd(1);

   for (int i = 0; i < 4;i++)
   {
      histo_name = "LeptonPt_histo" + to_string(i + 1);
      LeptonPt_histo[i] = new TH1F(histo_name, "Transverse momentum", 50, 0., 150.);

      histo_name = "LeptonEta_histo" + to_string(i + 1);
      LeptonEta_histo[i] = new TH1F(histo_name, "Pseudorapidity", 50, -2.5, 2.5);

      histo_name = "LeptonPhi_histo" + to_string(i + 1);
      LeptonPhi_histo[i] = new TH1F(histo_name, "Azimuthal angle", 40, -4.0, 4.0);

      histo_name = "LeptonBDT_histo" + to_string(i + 1);
      LeptonBDT_histo[i] = new TH1F(histo_name, "BDT", 20, -1.0, 1.0);
   }

   Mass_histo = new TH1F("Mass_histo", "Reconstructed 4 lepton mass", 50, 90., 140.);

   input_file = new TFile(input_file_name);

   hCounters = (TH1F *)input_file->Get("ZZTree/Counters");
   gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);

   input_tree = (TTree *)input_file->Get("ZZTree/candTree");
   Init(input_tree);

   if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      //if ( jentry > 10000 ) break;

      for (int i = 0; i < 4; i++)
      {
        LeptonPt_histo[i]->Fill(LepPt->at(i),_event_weight);
        LeptonEta_histo[i]->Fill(LepEta->at(i),_event_weight);
        LeptonPhi_histo[i]->Fill(LepPhi->at(i),_event_weight);
        LeptonBDT_histo[i]->Fill(LepBDT->at(i),_event_weight);
      }

      //recounstruction of Higgs
      lep0.SetPtEtaPhiM(LepPt->at(0), LepEta->at(0), LepPhi->at(0), 0);
      lep1.SetPtEtaPhiM(LepPt->at(1), LepEta->at(1), LepPhi->at(1), 0);
      lep2.SetPtEtaPhiM(LepPt->at(2), LepEta->at(2), LepPhi->at(2), 0);
      lep3.SetPtEtaPhiM(LepPt->at(3), LepEta->at(3), LepPhi->at(3), 0);

      if ((LepLepId->at(0) + LepLepId->at(1)) == 0 && (LepLepId->at(2) + LepLepId->at(3)) == 0)
      {
        Z1 = lep0 + lep1;
        Z2 = lep2 + lep3;

        Higgs = Z1+Z2;
      }

      // event weight
              _event_weight = 137.0 * 1000 * xsec * overallEventWeight / gen_sum_weights;

      Mass_histo->Fill(Higgs.M(),_event_weight);
   }

   //Plotting
   for (int i = 0; i < 4; i++)
   {
     // Remove stat box
     LeptonPt_histo[i]->SetStats(0);
     LeptonEta_histo[i]->SetStats(0);
     LeptonPhi_histo[i]->SetStats(0);
     LeptonBDT_histo[i]->SetStats(0);

     // Set axis titles
     LeptonPt_histo[i]->GetXaxis()->SetTitle("p_{T} [GeV]");
     LeptonPt_histo[i]->GetXaxis()->SetLabelSize(0.04);
     LeptonPt_histo[i]->GetYaxis()->SetTitle("Events / 3 GeV");
     LeptonPt_histo[i]->GetYaxis()->SetTitleOffset(1.9);
     LeptonPt_histo[i]->GetYaxis()->SetLabelSize(0.04);

     LeptonPt_histo[i]->SetMaximum(1.6*LeptonPt_histo[3]->GetMaximum());

     LeptonEta_histo[i]->GetXaxis()->SetTitle("#eta");
     LeptonEta_histo[i]->GetXaxis()->SetLabelSize(0.04);
     LeptonEta_histo[i]->GetYaxis()->SetTitle("Events / 0.1");
     LeptonEta_histo[i]->GetYaxis()->SetTitleOffset(1.8);
     LeptonEta_histo[i]->GetYaxis()->SetLabelSize(0.04);

     LeptonEta_histo[i]->SetMaximum(1.8*LeptonEta_histo[3]->GetMaximum());

     LeptonPhi_histo[i]->GetXaxis()->SetTitle("#Phi");
     LeptonPhi_histo[i]->GetXaxis()->SetLabelSize(0.04);
     LeptonPhi_histo[i]->GetYaxis()->SetTitle("Events / 0.2");
     LeptonPhi_histo[i]->GetYaxis()->SetTitleOffset(1.8);
     LeptonPhi_histo[i]->GetYaxis()->SetLabelSize(0.04);

     LeptonPhi_histo[i]->SetMaximum(1.8*LeptonPhi_histo[3]->GetMaximum());

     LeptonBDT_histo[i]->GetXaxis()->SetTitle("BDT Score");
     LeptonBDT_histo[i]->GetXaxis()->SetLabelSize(0.04);
     LeptonBDT_histo[i]->GetYaxis()->SetTitle("Events / 0.1");
     LeptonBDT_histo[i]->GetYaxis()->SetTitleOffset(1.8);
     LeptonBDT_histo[i]->GetYaxis()->SetLabelSize(0.04);

     LeptonBDT_histo[i]->SetMaximum(200*LeptonBDT_histo[3]->GetMaximum());
   }

   // Coloring
   LeptonPt_histo[0]->SetLineColor(kRed - 2);
   LeptonEta_histo[0]->SetLineColor(kRed - 2);
   LeptonPhi_histo[0]->SetLineColor(kRed - 2);
   LeptonBDT_histo[0]->SetLineColor(kRed - 2);

   LeptonPt_histo[0]->SetFillColor(kRed - 2);
   LeptonEta_histo[0]->SetFillColor(kRed - 2);
   LeptonPhi_histo[0]->SetFillColor(kRed - 2);
   LeptonBDT_histo[0]->SetFillColor(kRed - 2);

   LeptonPt_histo[1]->SetLineColor(kBlue - 2);
   LeptonEta_histo[1]->SetLineColor(kBlue - 2);
   LeptonPhi_histo[1]->SetLineColor(kBlue - 2);
   LeptonBDT_histo[1]->SetLineColor(kBlue - 2);

   LeptonPt_histo[1]->SetLineStyle(kDashed);
   LeptonEta_histo[1]->SetLineStyle(kDashed);
   LeptonPhi_histo[1]->SetLineStyle(kDashed);
   LeptonBDT_histo[1]->SetLineStyle(kDashed);

   LeptonPt_histo[2]->SetLineColor(kGreen - 2);
   LeptonEta_histo[2]->SetLineColor(kGreen - 2);
   LeptonPhi_histo[2]->SetLineColor(kGreen - 2);
   LeptonBDT_histo[2]->SetLineColor(kGreen - 2);

   LeptonPt_histo[2]->SetLineStyle(kDashed);
   LeptonEta_histo[2]->SetLineStyle(kDashed);
   LeptonPhi_histo[2]->SetLineStyle(kDashed);
   LeptonBDT_histo[2]->SetLineStyle(kDashed);

   LeptonPt_histo[3]->SetLineColor(kGray);
   LeptonEta_histo[3]->SetLineColor(kGray);
   LeptonPhi_histo[3]->SetLineColor(kGray);
   LeptonBDT_histo[3]->SetLineColor(kGray);

   LeptonPt_histo[3]->SetLineStyle(kDashed);
   LeptonEta_histo[3]->SetLineStyle(kDashed);
   LeptonPhi_histo[3]->SetLineStyle(kDashed);
   LeptonBDT_histo[3]->SetLineStyle(kDashed);

   LeptonPt_histo[0]->Draw("HIST");
   LeptonPt_histo[1]->Draw("HIST SAME");
   LeptonPt_histo[2]->Draw("HIST SAME");
   LeptonPt_histo[3]->Draw("HIST SAME");

   legend = CreateLegend(LeptonPt_histo[0], LeptonPt_histo[1], LeptonPt_histo[2], LeptonPt_histo[3]);
   legend->Draw();

   c->cd(2);
   gPad->SetLeftMargin(0.15);

   LeptonEta_histo[0]->Draw("HIST");
   LeptonEta_histo[1]->Draw("HIST SAME");
   LeptonEta_histo[2]->Draw("HIST SAME");
   LeptonEta_histo[3]->Draw("HIST SAME");

   legend = CreateLegend(LeptonEta_histo[0], LeptonEta_histo[1], LeptonEta_histo[2], LeptonEta_histo[3]);
   legend->Draw();

   c->cd(3);
   gPad->SetLeftMargin(0.15);

   LeptonPhi_histo[0]->Draw("HIST");
   LeptonPhi_histo[1]->Draw("HIST SAME");
   LeptonPhi_histo[2]->Draw("HIST SAME");
   LeptonPhi_histo[3]->Draw("HIST SAME");

   legend = CreateLegend(LeptonPhi_histo[0], LeptonPhi_histo[1], LeptonPhi_histo[2], LeptonPhi_histo[3]);
   legend->Draw();

   c->cd(4);
   gPad->SetLeftMargin(0.15);
   gPad->SetLogy();

   LeptonBDT_histo[0]->Draw("HIST");
   LeptonBDT_histo[1]->Draw("HIST SAME");
   LeptonBDT_histo[2]->Draw("HIST SAME");
   LeptonBDT_histo[3]->Draw("HIST SAME");

   legend = CreateLegend(LeptonBDT_histo[0], LeptonBDT_histo[1], LeptonBDT_histo[2], LeptonBDT_histo[3]);
   legend->Draw();

   SavePlots(c, "Leptons");

}

void Analyzer::PlotMass()
{
   c = new TCanvas("c2", "c2", 900, 900);
   gPad->SetLeftMargin(0.15);

   Mass_histo->SetTitle("Reconstructed four lepton mass");
   Mass_histo->GetXaxis()->SetTitle("m_{41} [GeV]");
   Mass_histo->GetYaxis()->SetTitle("Events/1GeV");

   Mass_histo->SetMaximum(1.5 * Mass_histo->GetMaximum());

   Mass_histo->SetStats(0);
   Mass_histo->SetLineColor(kGreen - 1);
   Mass_histo->SetFillColor(kGreen - 1);

   Mass_histo->Draw("HIST");

   TLegend *leg;
   leg = new TLegend(0.5,0.8,0.9,0.9);

   leg->AddEntry(Mass_histo, "gluon-gluon fusion", "f");
   leg->SetTextSize(0.03);

   leg->Draw();

   SavePlots(c,"MassHistogram");


}

TLegend* Analyzer::CreateLegend(TH1F *lepton_1, TH1F *lepton_2, TH1F *lepton_3, TH1F *lepton_4)
{
  TLegend *leg;

  leg = new TLegend(0.5,0.7,0.9,0.9);

  leg->AddEntry(lepton_1, "Frist lepton", "f");
  leg->AddEntry(lepton_2, "Second lepton", "f");
  leg->AddEntry(lepton_3, "Third lepton", "f");
  leg->AddEntry(lepton_4, "Fourth lepton", "f");

  leg->SetTextSize(0.03);

  return leg;
}

void Analyzer::SavePlots(TCanvas *c, TString name)
{
  c->SaveAs(name + ".pdf");
  c->SaveAs(name + ".png");
  c->SaveAs(name + ".root");
}



void Analyzer::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Analyzer.C
//      root> Analyzer t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
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
   }
}
