#ifndef GUARD_HistManager_h
#define GUARD_HistManager_h

#include <iostream>
#include <map>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TList.h>
#include <TFile.h> 

const int NSLABS = 15;

class HistManager
{
  public:
    HistManager();
    ~HistManager(){}

  // Methods
    virtual void InitializeHists();
    virtual void Hist2List();
    virtual void WriteLists( TFile * output );

  // Declear histograms
  // h1 hist

    enum h1_K_reco{
      d0_K_reco_primary,
      d0_sigma_K_reco_primary,
      d0_sigma_d0_K_reco_primary,

      z0_K_reco_primary,
      z0_sigma_K_reco_primary,
      z0_sigma_z0_K_reco_primary,

      d0_K_reco_secondary,
      d0_sigma_K_reco_secondary,
      d0_sigma_d0_K_reco_secondary,

      z0_K_reco_secondary,
      z0_sigma_K_reco_secondary,
      z0_sigma_z0_K_reco_secondary,

      d0_K_reco_pseudo,
      d0_sigma_K_reco_pseudo,
      d0_sigma_d0_K_reco_pseudo,

      z0_K_reco_pseudo,
      z0_sigma_K_reco_pseudo,
      z0_sigma_z0_K_reco_pseudo,

      ctag,

      d0_K_reco_garbage,
      d0_sigma_K_reco_garbage,
      d0_sigma_d0_K_reco_garbage,

      z0_K_reco_garbage,
      z0_sigma_K_reco_garbage,
      z0_sigma_z0_K_reco_garbage,

      d0_K_reco_primary_ctag_cut,
      d0_sigma_K_reco_primary_ctag_cut,
      d0_sigma_d0_K_reco_primary_ctag_cut,

      z0_K_reco_primary_ctag_cut,
      z0_sigma_K_reco_primary_ctag_cut,
      z0_sigma_z0_K_reco_primary_ctag_cut,

      d0_K_reco_secondary_ctag_cut,
      d0_sigma_K_reco_secondary_ctag_cut,
      d0_sigma_d0_K_reco_secondary_ctag_cut,

      z0_K_reco_secondary_ctag_cut,
      z0_sigma_K_reco_secondary_ctag_cut,
      z0_sigma_z0_K_reco_secondary_ctag_cut,

      pmag_K_reco,
      cos_theta_K_reco,

      dummy_K_reco,
      Last_h1_K_reco = dummy_K_reco
    };
    TH1F * h1_K_reco[Last_h1_K_reco];

    enum h_PS{
    d0_P_single,
    d0_S_single,
    d0_P_mult,
    d0_S_mult,
    z0_P_single,
    z0_S_single,
    z0_P_mult,
    z0_S_mult,
    dummy_h_PS,
    Last_h_PS = dummy_h_PS
    };
    TH1* h_PS[Last_h_PS];

    enum h_tagging {
      p_ctag,
      s_ctag,
      p_btag,
      s_btag,
      t_ctag,
      t_btag,
      jets_info,

      dummy_tagging,
      Last_h_tagging = dummy_tagging
    };
    TH1F * h_tagging[Last_h_tagging];


    enum h_general {
      // nvtx_ctag,
      n_K_ecal,

      dummy_general,
      Last_h_general = dummy_general
    };
    TH1F * h_general[Last_h_general];

  private:
    TList* hList_K_reco          = new TList();
    TList* hList_general         = new TList();
    TList* hList_tagging         = new TList();
    TList* hList_PS              = new TList();
};

#endif
