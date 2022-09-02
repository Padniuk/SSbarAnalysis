/*------------------------------------------------------------------------------
   main.cpp
------------------------------------------------------------------------------*/

// HEADERS
#include <iostream>                 // stdlib
#include <string>
#include "TFile.h"                  // ROOT class headers
#include "TString.h"
#include "TTree.h"
#include "../SSbarLibrary/timestamp.hh"
#include "include/NtupleProcessor.hh"

using namespace std;

int main(int argc, char* argv[])
{
  // Record the time main starts processing.
  TString ts_mainBegin  = timeStamp();
  TString fts_mainBegin = fileTimeStamp();

  // BEGIN OUTPUT
  cout << "\n\n"
          "============================================\n"
          "==========SSbarHistogramExtractor===========\n"
          "  Processing Begun: " << ts_mainBegin << "\n"
          "\n";

  NtupleProcessor ntplproc;

  // CLOSING OUTPUT.
    TString ts_mainEnd = timeStamp();

    cout << "\n"
            "   Completion time: " << ts_mainEnd <<      "\n"
            "=====SSbarHistogramExtractor - FINISHED=====\n"
            "============================================\n" << endl;

    return 0;
}