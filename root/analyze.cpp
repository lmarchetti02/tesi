#include <iostream>
#include <vector>
#include <map>
#include <fstream>

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

void analyze(const char *fileName, const int nPixel)
{
    TFile *resultsFile = new TFile(fileName, "READ");
    TTree *resultsTree = (TTree *)resultsFile->Get("Hits");

    // get z values
    map<int, double> hitsMap = read_ntuple(resultsTree, nPixel);
    print_map(hitsMap);

    // generate (x,y) points
    map<int, vector<double>> pixelMap = reconstruct_grid(nPixel);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    TGraph2D *graph = new TGraph2D();

    for (int i = 0; i < nPixel * nPixel; i++)
    {
        graph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], hitsMap[i]);
    }

    std::fstream dataFile;
    dataFile.open("results/data.txt", std::ios::out);
    if (dataFile.is_open())
    {
        dataFile << "RISULTATI DELLA SIMULAZIONE" << endl;
        dataFile << "---------------------------" << endl;

        int i = 0;
        while (i < nPixel * nPixel)
        {
            dataFile << pixelMap[i][0] << "," << pixelMap[i][1] << "," << hitsMap[i] << endl;
            i++;
        }
    }
    dataFile.close();

    gStyle->SetPalette(1);
    graph->Draw("surf1");

    resultsFile->Close();
}
