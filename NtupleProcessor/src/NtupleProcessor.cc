
/*------------------------------------------------------------------------------
NtupleProcessor.cpp
 Created : 2022-09-02  okugawa
------------------------------------------------------------------------------*/

#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/NtupleProcessor.hh"

using std::cout;   using std::endl;

NtupleProcessor::NtupleProcessor()
{
  // TEST output
    cout << "    NtupleProcessor: Created." << endl;

    TString dummy_label = "rv02-02.sv02-02.mILD_l5_o1_v02.E250-SetA.I500010.P2f_z_h.eL.pR.n002.d_dstm_15162_000";

    NtupleFile = TFile::Open("rootfiles/" + dummy_label + ".tmp.root", "RECREATE");

  // Takes input options and processes appropriately
  //   Options that can be specified:
  //     - Operating points
  //     - Differential variables (default: JetPT)
  //     - Dataset ntuple file to run on (default: all)
  //     - Desired number of entry to run on per file and entries to skip.
  //     - Modifications to the analysis (systematic analyses, etc)
  // For each dataset that needs to be run over...
  //   Opens the appropriate file and tree
  //   Creates a TreeIterator, EventHandler, and the desired HistogramMakers
  //   Runs the TreeIterator.

}