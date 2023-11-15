#include <iostream>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph2D.h"
#include "TStyle.h"

#include "functions.hh"

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

void analyze(const char *fileName)
{
    TFile *resultsFile = new TFile(fileName, "READ");
    TTree *resultsTree = (TTree *)resultsFile->Get("Hits");

    // get z values
    map<int, double> hitsMap = read_ntuple(resultsTree);

    // generate (x,y) points
    int nPixel = TMath::Sqrt(hitsMap.size());
    cout << "Number of pixels: " << nPixel << endl;
    map<int, vector<double>> pixelMap = reconstruct_grid(nPixel);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    TGraph2D *graph = new TGraph2D();

    for (int i = 0; i < nPixel * nPixel; i++)
    {
        graph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], hitsMap[i]);
    }

    gStyle->SetPalette(1);
    graph->Draw("surf1");

    resultsFile->Close();
}
