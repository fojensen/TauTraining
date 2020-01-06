#include <TCut.h>
#include <iostream>
#include <TLegend.h>
#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

double addOverflow(TH1D *h)
{
   const int n = h->GetNbinsX();
   const double y = h->GetBinContent(n+1);
   if (y) std::cout << h->GetName() << " has overflow" << std::endl;
   h->SetBinContent(n+1, 0);
   h->SetBinError(n+1, 0.);
   h->AddBinContent(n, y);
   return y;
}

double addUnderflow(TH1D *h)
{
   const double y = h->GetBinContent(0);
   if (y) std::cout << h->GetName() << " has underflow" << std::endl;
   h->SetBinContent(0, 0);
   h->SetBinError(0, 0.);
   h->AddBinContent(1, y);
   return y;
}

void runPoint(TH1D * htemp, const TString var, const bool dolog=false)
{
   std::cout << "plotting: " << var << std::endl;

   const int n = 4;
   TString filetag[n];
   TCut cuts[n];

   TCut baseline = "1>0";
   baseline = baseline && TCut("hcalEnergy==0."); 

   filetag[0] = "QCD_Flat_Run2";     cuts[0] = "drmin_tau_tau>=0.4";
   filetag[1] = "VBFHToTauTau_Run2"; cuts[1] = "drmin_tau_tau<0.4";
   filetag[2] = "QCD_Flat";          cuts[2] = "drmin_tau_tau>=0.4";
   filetag[3] = "VBFHToTauTau";      cuts[3] = "drmin_tau_tau<0.4";

   TString legendtag[4];
   legendtag[0] = "QCD Flat; Run2";
   legendtag[1] = "VBF H; Run2";
   legendtag[2] = "QCD Flat; Phase2";
   legendtag[3] = "VBF H; Phase2";
   
/*   filetag[0] = "QCD_Flat";          cuts[0] = "drmin_tau_tau>=0.4";
   filetag[1] = "WToLNu_2J";         cuts[1] = "drmin_tau_tau<0.4";
   filetag[2] = "DYToLL-M-50_2J";    cuts[2] = "drmin_tau_tau<0.4";
   filetag[3] = "TTTo2L2Nu";         cuts[3] = "drmin_tau_tau<0.4";
   filetag[4] = "GluGluHToTauTau";   cuts[4] = "drmin_tau_tau<0.4";
   filetag[5] = "VBFHToTauTau";      cuts[5] = "drmin_tau_tau<0.4";
   filetag[6] = "VBFHToTauTau_Run2"; cuts[6] = "drmin_tau_tau<0.4";*/

   TH1D * h[n];
   for (int i = 0; i < n; ++i) {
      const TString hname = TString(htemp->GetName())+"_"+TString::Itoa(i, 10);
      h[i] = (TH1D*)htemp->Clone(hname);
      //TChain chain("skimmedTree");
      TChain chain("tauAnalyzer/tree");
      char buffer[100];
      //sprintf(buffer, "./outputData/skim_%s.root", filetag[i].Data()); 
      sprintf(buffer, "./outputData/%s.root", filetag[i].Data());
      chain.Add(buffer);
      if (chain.GetEntries()) {
         const double nentries = chain.Project(hname, var, cuts[i] && baseline);
         std::cout << filetag[i] << " has " << nentries << " entries." << std::endl;
         addOverflow(h[i]);
         addUnderflow(h[i]);
         h[i]->Scale(1./nentries);
         h[i]->SetLineWidth(2);
         h[i]->SetLineColor(i+1);
      }
   }
//   h[4]->SetLineColor(9);
 //  h[6]->SetLineWidth(1);  

   TCanvas * canvas = new TCanvas("canvas_"+var, var, 400, 400);
   double max = 0.;
   for (int i = 0; i < n; ++i) {
      if (h[i]->GetMaximum()>max) max = h[i]->GetMaximum();
      h[i]->Draw("HIST, E, SAME");      
   }
   h[0]->Draw("HIST, E, SAME");   

   h[0]->SetMinimum(0.);
   h[0]->SetMaximum(1.1*max);
   h[0]->SetStats(0);
   if (dolog) {
      h[0]->SetMaximum(2.);
      h[0]->SetMinimum(1.e-4);
      canvas->SetLogy();
   }

   TLegend * l = new TLegend(0.25, 0.8, 0.875, 0.875);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   for (int i = 0; i < n; ++i) {
      l->AddEntry(h[i], legendtag[i], "L");
   }
   l->Draw();

   char hname[100];
   sprintf(hname, "./plots/dists_%s.pdf", htemp->GetName());
   canvas->SaveAs(hname);
   std::cout << "" << std::endl;
}

void plotTrainingVariables()
{
//   TH1D h_pt("h_pt", ";pt;#tau_{h} candidates / 10 GeV", 20, 20., 220.);
  // runPoint(&h_pt, "pt", true);

   TH1D h_eta("h_eta", ";TMath::Abs(eta);#tau_{h} candidates / 0.1", 30, 0., 3.);
   runPoint(&h_eta, "TMath::Abs(eta)", false);

  /* TH1D h_chargedIsoPtSum("h_chargedIsoPtSum", ";chargedIsoPtSum;#tau_{h} candidates / 1 GeV", 20, 0., 250.);
   runPoint(&h_chargedIsoPtSum, "chargedIsoPtSum", true);

   TH1D h_neutralIsoPtSum("h_neutralIsoPtSum", ";neutralIsoPtSum;#tau_{h} candidates / 1 GeV", 20, 0., 250.);
   runPoint(&h_neutralIsoPtSum, "neutralIsoPtSum", true);
   
   TH1D h_puCorrPtSum("h_puCorrPtSum", ";puCorrPtSum;#tau_{h} candidates / 10 GeV", 20, 0., 500.);
   runPoint(&h_puCorrPtSum, "puCorrPtSum", true);

   TH1D h_photonPtSumOutsideSignalCone("h_photonPtSumOutsideSignalCone", ";photonPtSumOutsideSignalCone;#tau_{h} candidates /", 20, 0., 100.);
   runPoint(&h_photonPtSumOutsideSignalCone, "photonPtSumOutsideSignalCone", true);

   TH1D h_decayMode("h_decayMode", ";decayMode;# of #tau_{h} / bin", 12, -0.5, 11.5);
   runPoint(&h_decayMode, "decayMode", false);

   TH1D h_nPhotons_0p5("h_nPhotons_0p5", ";size signalGammaCands+isolationGammaCands;", 20, -0.5, 99.5);
   runPoint(&h_nPhotons_0p5, "signalGammaCands_size_0p5+isolationGammaCands_size_0p5", true);

   TH1D h_isoCands_deta("h_isoCands_deta", ";isolationGammaCands #Delta #eta;", 24, 0., 0.6);
   runPoint(&h_isoCands_deta, "isoCands_deta", true);

   TH1D h_isoCands_dphi("h_isoCands_dphi", ";isolationGammaCands #Delta #phi;", 24, 0., 0.6);
   runPoint(&h_isoCands_dphi, "isoCands_dphi", true);

   TH1D h_isoCands_dr("h_isoCands_dr", ";isolationGammaCands #Delta R;", 24, 0., 0.6);
   runPoint(&h_isoCands_dr, "isoCands_dr", true);

   TH1D h_sigCands_deta("h_sigCands_deta", ";signalGammaCands #Delta #eta;", 24, 0., 0.6);
   runPoint(&h_sigCands_deta, "sigCands_deta", true);

   TH1D h_sigCands_dphi("h_sigCands_dphi", ";signalGammaCands #Delta #phi;", 24, 0., 0.6);
   runPoint(&h_sigCands_dphi, "sigCands_dphi", true);

   TH1D h_sigCands_dr("h_sigCands_dr", ";signalGammaCands #Delta R", 24, 0., 0.6);
   runPoint(&h_sigCands_dr, "sigCands_dr", true);

   TH1D h_eRatio("h_eRatio", ";ecalEnergy/(ecalEnergy+hcalEnergy);", 20, -0.2, 1.);
   runPoint(&h_eRatio, "eRatio", true);

   TH1D h_dxy_sign("h_dxy_sign", ";TMath::Sign(+1., dxy);#tau_{h} candidates / bin", 3, -1.5, 1.5);
   runPoint(&h_dxy_sign, "TMath::Sign(+1., dxy)", true);
   
   TH1D h_dxy("h_dxy", ";TMath::Abs(dxy);#tau_{h} candidates / 0.05", 20, 0., 1.);
   runPoint(&h_dxy, "TMath::Abs(dxy)", true);

   TH1D h_dxy_Sig("h_dxy_Sig", ";TMath::Abs(dxy_Sig);#tau_{h} candidates / 5", 20, 0., 100.);
   runPoint(&h_dxy_Sig, "TMath::Abs(dxy_Sig)", true);

   TH1D h_ip3d_sign("h_ip3d_sign", ";TMath::Sign(+1., ip3d);#tau_{h} candidates / bin", 3, -1.5, 1.5);
   runPoint(&h_ip3d_sign, "TMath::Sign(+1., ip3d)", true);

   TH1D h_ip3d("h_ip3d", ";TMath::Abs(ip3d);#tau_{h} candidates / 0.005", 20, 0., 1.);
   runPoint(&h_ip3d, "TMath::Abs(ip3d)", true);

   TH1D h_ip3d_Sig("h_ip3d_Sig", ";TMath::Abs(ip3d_Sig);#tau_{h} candidates / 0.25", 20, 0., 10.);
   runPoint(&h_ip3d_Sig, "TMath::Abs(ip3d_Sig)", true);

   TH1D h_hasSecondaryVertex("h_hasSecondaryVertex", ";hasSecondaryVertex;#tau_{h} candidates / bin", 2, -0.5, 1.5);
   runPoint(&h_hasSecondaryVertex, "hasSecondaryVertex", false);

   TH1D h_flightLength("h_flightLength", ";flightLength;#tau_{h} candidate", 20, 0., 10.);
   runPoint(&h_flightLength, "flightLength", true);

   TH1D h_flightLengthSig("h_flightLengthSig", ";flightLengthSig;#tau_{h} candidates / bin", 20, -1., 100.);
   runPoint(&h_flightLengthSig, "flightLengthSig", true);
*/
//   TH1D h_recTauGJangleDiff("h_recTauGJangleDiff", ";recTauGJangleDiff;", 20, -2., 4.);
  // runPoint(&h_recTauGJangleDiff, "recTauGJangleDiff", true); 
}

