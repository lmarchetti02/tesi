#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <array>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph2D.h"
#include "TStyle.h"

#include "include/data.hh"
#include "include/functions.hh"

using std::cin;
using std::cout;
using std::endl;

void analyze(const char *fileName, const int nPixel, const char *type = "")
{
    TFile *resultsFile = new TFile(fileName, "READ");

    // get trees from file
    TTree *hitsTree;
    if (strcmp(type, "-ncs") == 0)
        hitsTree = static_cast<TTree *>(resultsFile->Get("Hits"));

    hitsTree = static_cast<TTree *>(resultsFile->Get("CS"));

    // get hits from tree
    std::map<int, double> hitsMap = data::read_hits_tree(hitsTree, nPixel);
    functions::print_hits_map(hitsMap);

    // close file
    resultsFile->Close();
    delete resultsFile;
    resultsFile = nullptr;

    // get (x,y) of pixels center
    std::map<int, std::array<double, 2>> pixelMap = data::reconstruct_grid(nPixel);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    TGraph2D *graph = new TGraph2D();

    for (int i = 0; i < nPixel * nPixel; i++)
    {
        graph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], hitsMap[i]);
    }

    gStyle->SetPalette(1);
    graph->Draw("surf1");

    data::save_results(pixelMap, hitsMap);

    cin.get();
    cout << "ciao\n";
}
