#define Analyzer_cxx
#include "Analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Analyzer::PlotHistogram()
{
   c = new TCanvas("c", "c", 900, 700);
   c->Divide(2);

   Higgs = new TLorentzVector();


   c->cd(1);

   Particle1_pt_histo = new TH1F("Particle1_pt_histo", "Histogram of decay particle p_{T}", 50, 0., 150.);
   Particle2_pt_histo = new TH1F("Particle2_pt_histo", "Histogram of decay particle p_{T}", 50, 0., 150.);

   Higgs_pt_histo= new TH1F("Higgs_pt_histo", "Histogram of Higgs p_{T}", 50, 0., 150.);

   //uklanjamo podatke o broju dogadaja i srednjoj vrijednosti
   Particle1_pt_histo->SetStats(0);
   Particle2_pt_histo->SetStats(0);

   Higgs_pt_histo->SetStats(0);

   // Set axis titles
   Particle1_pt_histo->GetXaxis()->SetTitle("p_{T} [GeV]");
   Particle1_pt_histo->GetXaxis()->SetLabelSize(0.04);
   Particle1_pt_histo->GetYaxis()->SetTitle("Events / 3 GeV");
   Particle1_pt_histo->GetYaxis()->SetTitleOffset(1.5);
   Particle1_pt_histo->GetYaxis()->SetLabelSize(0.04);

   Particle2_pt_histo->GetXaxis()->SetTitle("p_{T} [GeV]");
   Particle2_pt_histo->GetXaxis()->SetLabelSize(0.04);
   Particle2_pt_histo->GetYaxis()->SetTitle("Events / 3 GeV");
   Particle2_pt_histo->GetYaxis()->SetTitleOffset(1.5);
   Particle2_pt_histo->GetYaxis()->SetLabelSize(0.04);

   Higgs_pt_histo->GetXaxis()->SetTitle("p_{T} [GeV]");
   Higgs_pt_histo->GetXaxis()->SetLabelSize(0.04);
   Higgs_pt_histo->GetYaxis()->SetTitle("Events / 3 GeV");
   Higgs_pt_histo->GetYaxis()->SetTitleOffset(1.5);
   Higgs_pt_histo->GetYaxis()->SetLabelSize(0.04);

   //Color
   Particle1_pt_histo->SetLineColor(kBlue - 2);
   Particle1_pt_histo->SetFillColor(kRed - 2);

   Particle2_pt_histo->SetLineColor(kRed + 1);

   Higgs_pt_histo->SetLineColor(kGreen + 1);
   Higgs_pt_histo->SetFillColor(kGreen + 1);


   if (fChain == 0)
      return;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      Particle1_pt_histo->Fill(DecayParticle1_pt);
      Particle2_pt_histo->Fill(DecayParticle2_pt);

      //Reconstruct Higgs from decay products
      Higgs->SetPxPyPzE(DecayParticle1_px + DecayParticle2_px, DecayParticle1_py + DecayParticle2_py, DecayParticle1_pz + DecayParticle2_pz, DecayParticle1_E + DecayParticle2_E);

      Higgs_pt_histo->Fill(Higgs->Pt());
  }

  Particle1_pt_histo->Draw();
  Particle2_pt_histo->Draw("Hist same");

  TLegend *legend = new TLegend(0.5,0.8,0.9,0.9);
  legend->AddEntry(Particle1_pt_histo,"Frist decay particle","f");
  legend->AddEntry(Particle2_pt_histo,"Second decay particle","f");
  legend->SetTextSize(0.03);
  legend->Draw();
  c->cd(2);

  Higgs_pt_histo->Draw();

  TLegend *legend_Higgs = new TLegend(0.1,0.8,0.5,0.9);
  legend_Higgs->AddEntry(Higgs_pt_histo,"Reconstructed Higgs","f");
  legend_Higgs->SetTextSize(0.03);
  legend_Higgs->Draw();

  SavePlots(c, "Higgs");

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
//    root> t.Loop();       // Loop on all entries
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
