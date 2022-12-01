#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

Float_t bins_cos_fine[] = {-1.0,-0.98,-0.96,-0.94,-0.92,-0.90,-0.88,-0.86,-0.84,-0.82,-0.80,-0.75,-0.70,-0.60,-0.50,-0.40,-0.30,-0.20,-0.10,
                            0.0,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.75,0.80,0.82,0.84,0.86,0.88,0.90,0.92,0.94,0.96,0.98,1.0};
Int_t   nbins_cos = sizeof(bins_cos_fine) / sizeof(Float_t) - 1;

Float_t bins_cos_fine_half[] = {0.0,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.75,0.80,0.82,0.84,0.86,0.88,0.90,0.92,0.94,0.96,0.98,1.0};
Int_t   nbins_cos_half = sizeof(bins_cos_fine_half) / sizeof(Float_t) - 1;

void BinNormal(TH1F *h)
{
  const Int_t nbins = h->GetNbinsX();
  for (int ibin=1; ibin<=nbins; ibin++){
    Float_t binc = h->GetBinContent(ibin);
    Float_t binw = h->GetBinWidth(ibin);
    Float_t bin_normal = binc / binw;
    h->SetBinContent(ibin,bin_normal);
  }

}

void Normalize(TH1F *h)
{
  // h->Scale( 1.0 / h->GetEntries() );
  const Int_t nbins = h->GetNbinsX();
  Int_t nbins4 = nbins / 4;
  Int_t int_high = (nbins / 2) + nbins4;
  Int_t int_low  = (nbins / 2 + 1) - nbins4;
  h->Scale( 1.0 / h->Integral(int_low,int_high) );
}

void Normalize2Gen(TH1F *h, TH1F *h_gen)
{
	double intCosReco = h->Integral(20,80);
	double intCosGen  = h_gen->Integral(20,80);
  h->Scale( intCosGen / intCosReco );
}

void StyleHist(TH1F *h, Color_t col)
{
  h->SetLineWidth(3);
  h->SetLineColor(col);
  h->SetFillStyle(3002);
  h->SetFillColor(col);
}

void StylePad(TPad *pad, Float_t t, Float_t b, Float_t r, Float_t l)
{
  pad->SetGrid(1,1);
  if(t) pad->SetTopMargin(t);
  if(b) pad->SetBottomMargin(b);
  if(r) pad->SetRightMargin(r);
  if(l) pad->SetLeftMargin(l);
  pad->Draw();
  pad->cd();

}

vector<Float_t> GetP( TH1F * h_accepted, TH1F * h_rejected )
{
  const Int_t nbins = h_accepted->GetNbinsX();
  vector<Float_t> result_error;
  vector<Float_t> result;

  for (int i = 1; i < nbins / 2 + 1; i++)
  {
    std::vector<float> result_j;
    for (int i1 = -1; i1 < 2; i1 += 2)
    {
      for (int i2 = -1; i2 < 2; i2 += 2)
      {
        for (int i3 = -1; i3 < 2; i3 += 2)
        {
          float accepted = h_accepted->GetBinContent(nbins + 1 - i) + i1 * sqrt(h_accepted->GetBinContent(nbins + 1 - i));
          float rejected = h_rejected->GetBinContent(nbins + 1 - i) + i2 * sqrt(h_rejected->GetBinContent(nbins + 1 - i));
          accepted += h_accepted->GetBinContent(i) + i3 * sqrt(h_accepted->GetBinContent(i));

          float a = 1;
          float b = -1;
          float c = rejected / (2 * (accepted + rejected));
          float p = (0.5 / a) * (-b + sqrt(b * b - 4 * a * c));
          float p2 = (0.5 / a) * (-b - sqrt(b * b - 4 * a * c));
          if (p > 0.99)
            p = 0;
          if (p2 > 0.99)
            p2 = 0;
          if (p > 0 || p2 > 0)
            result_j.push_back(max(p, p2));
        }
      }
    }

    float average = 0;
    float n = 0;
    for (unsigned j = 0; j < result_j.size(); j++)
    {
      if (result_j.at(j) > 0)
      {
        average += result_j.at(j);
        n++;
      }
    }
    average /= n;

    if ( (average != 0) && (average == average) )
    {
      result.push_back(average);
      float std_dev = 0;
      for (unsigned j = 0; j < result_j.size(); j++)
      {
        if (result_j.at(j) > 0)
        {
          std_dev += pow(result_j.at(j) - average, 2);
        }
      }
      std_dev = sqrt(std_dev / (n - 1));
      result_error.push_back(std_dev);
    }
    else
    {
      result_error.push_back(0);
      result.push_back(0);
    }
  }

  for (unsigned i = 0; i < result_error.size(); i++)
  {
    if (result_error.at(i) > 0)
      result.push_back(result_error.at(i));
    else
      result.push_back(0);
  }

  return result;

}

TH1F * CorrectHist( TH1F * h_reco, vector<Float_t> p_vec)
{
  const Int_t nbins = h_reco->GetNbinsX();

  TH1F *corrected = new TH1F("corrected", "corrected", nbins_cos,bins_cos_fine);
  corrected->Sumw2();
  for (int i = 1; i < nbins / 2 + 1; i++)
  {
    float p = p_vec.at(i - 1);
    float q = 1 - p;
    float weight = (p * p + q * q) / (q * q * q * q - p * p * p * p);

    // calcualte average
    float av_i = 0;
    float av_41i = 0;
    float n = 0;
    for (int i1 = -1; i1 < 2; i1 += 1)
    {
      for (int i2 = -1; i2 < 2; i2 += 1)
      {
        float nm_reco_error = h_reco->GetBinContent(i) + i1 * h_reco->GetBinError(i);
        float np_reco_error = h_reco->GetBinContent(nbins + 1 - i) + i2 * h_reco->GetBinError(nbins + 1 - i);
        av_i += (np_reco_error * q * q - nm_reco_error * p * p) * weight;
        av_41i += -(np_reco_error * p * p - nm_reco_error * q * q) * weight;
        n++;
      }
    }
    av_i /= n;
    av_41i /= n;
    corrected->SetBinContent(i, av_i);
    corrected->SetBinContent(nbins + 1 - i, av_41i);

    // calculate error
    float error_i = 0;
    float error_41i = 0;
    n = 0;
    for (int i1 = -1; i1 < 2; i1 += 1)
    {
      for (int i2 = -1; i2 < 2; i2 += 1)
      {
        float nm_reco_error = h_reco->GetBinContent(i) + i1 * h_reco->GetBinError(i);
        float np_reco_error = h_reco->GetBinContent(nbins + 1 - i) + i2 * h_reco->GetBinError(nbins + 1 - i);
        error_i += pow((np_reco_error * q * q - nm_reco_error * p * p) * weight - av_i, 2);
        error_41i += pow(-(np_reco_error * p * p - nm_reco_error * q * q) * weight - av_41i, 2);
        n++;
      }
    }
    error_i = sqrt(error_i / (n - 1.));
    error_41i = sqrt(error_41i / (n - 1.));
    corrected->SetBinError(i, error_i);
    corrected->SetBinError(nbins + 1 - i, error_41i);
  }

  return corrected;

}

void pq_method()
{
  gStyle->SetOptStat(0);

  // TFile *file = new TFile("../rootfiles/merged/rv02-02.sv02-02.mILD_l5_o1_v02.E250-SetA.I500010.P2f_z_h.eL.pR.uu.LPFOp15_pNaN.tpc0.hists.all.root","READ");
  TFile *file = new TFile("../rootfiles/merged/rv02-02.sv02-02.mILD_l5_o1_v02.E250-SetA.I500010.P2f_z_h.eL.pR.ss.LPFOp15_pNaN.tpc0.hists.all.root","READ");
  // TFile *file = new TFile("../rootfiles/merged/rv02-02.sv02-02.mILD_l5_o1_v02.E250-SetA.I500010.P2f_z_h.eL.pR.ss.LPFOp20_p60.tpc0.hists.all.root","READ");

  if (!file->IsOpen()) return;

  TH1F *h_gen_q_qcos  = (TH1F*) file->Get("h_gen_q_qcos");
  TH1F *h_reco_K_scos  = (TH1F*) file->Get("h_reco_K_scos");
  TH1F *h_reco_K_qcos = (TH1F*) file->Get("h_reco_K_qcos");
  TH1F *h_acc_KK_cos  = (TH1F*) file->Get("pq/h_acc_KK_cos");
  TH1F *h_rej_KK_cos  = (TH1F*) file->Get("pq/h_rej_KK_cos");

  StyleHist(h_gen_q_qcos,kGreen+1);
  h_gen_q_qcos->SetFillStyle(0);
  h_gen_q_qcos->SetLineStyle(2);

  StyleHist(h_reco_K_scos,kBlack);
  h_reco_K_scos->SetFillStyle(0);
  StyleHist(h_reco_K_qcos,kRed+2);
  StyleHist(h_acc_KK_cos,kRed+2);
  StyleHist(h_rej_KK_cos,kBlue+2);

  const Int_t nbins = h_reco_K_scos->GetNbinsX();

  TH1F *p_KK = new TH1F("p_KK", "p_KK", nbins_cos_half,bins_cos_fine_half);
  p_KK->Sumw2();

  vector<Float_t> p_vec = GetP(h_acc_KK_cos, h_rej_KK_cos);

  for (unsigned i = 0; i < p_vec.size() / 2; i++)
  {
    p_KK->SetBinContent(nbins / 2 - i, p_vec.at(i));
    p_KK->SetBinError(nbins / 2 - i, p_vec.at(i + nbins / 2));
  }

  TH1F *h_reco_K_pq_cos = CorrectHist(h_reco_K_qcos, p_vec);
  StyleHist(h_reco_K_pq_cos,kBlue);

  TCanvas *c0 = new TCanvas("c0","c0",800,800);
  TPad *pad0 = new TPad("pad0", "pad0",0,0,1,1);
  StylePad(pad0,0,0.12,0,0.15);

  BinNormal(h_gen_q_qcos);
  BinNormal(h_reco_K_scos);
  BinNormal(h_reco_K_pq_cos);
  BinNormal(h_reco_K_qcos);

  Normalize(h_gen_q_qcos);
  Normalize(h_reco_K_scos);
  Normalize(h_reco_K_pq_cos);
  Normalize(h_reco_K_qcos);

  h_reco_K_pq_cos->GetYaxis()->SetRangeUser(0,0.17);
  h_reco_K_pq_cos->SetTitle(";K^{+}K^{-} cos#theta;a.u.");
  h_reco_K_pq_cos->Draw("h");
  h_reco_K_qcos->Draw("hsame");
  h_reco_K_scos->Draw("hsame");
  h_gen_q_qcos->Draw("hsame");

  TLegend *leg = new TLegend(0.2,0.76,0.7,0.85);
  leg->SetLineColor(0);
  leg->AddEntry(h_gen_q_qcos,"Generated s-quark angle","l");
  leg->AddEntry(h_reco_K_scos,"Reconstructed K^{+}K^{-} matched with s-quark angle","l");
  leg->AddEntry(h_reco_K_qcos,"Reconstructed K^{+}K^{-}","l");
  leg->AddEntry(h_reco_K_pq_cos,"Reconstructed K^{+}K^{-} (corrected)","l");
  leg->Draw();

  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  TPad *pad1 = new TPad("pad1", "pad1",0,0,1,1);
  StylePad(pad1,0,0.12,0,0.15);
  
  StyleHist(p_KK,kGreen+2);
  p_KK->SetTitle(";cos#theta_{K^{#pm}};p value");
  p_KK->GetYaxis()->SetRangeUser(0,1);
  p_KK->Draw("h");

  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  TGaxis::SetMaxDigits(3);
  gPad->SetGrid(1,1);
  h_acc_KK_cos->SetTitle(";K^{+}K^{-} cos#theta;Entries");

  BinNormal(h_acc_KK_cos);
  BinNormal(h_rej_KK_cos);

  h_acc_KK_cos->Draw("h");
  h_rej_KK_cos->Draw("hsame");

  TLegend *leg2 = new TLegend(0.15,0.75,0.45,0.85);
  leg2->SetLineColor(0);
  leg2->AddEntry(h_acc_KK_cos,"N Accepted","l");
  leg2->AddEntry(h_rej_KK_cos,"N Rejected","l");
  leg2->Draw();

}
