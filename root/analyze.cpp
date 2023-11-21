#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <array>

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
#include "include/charge_sharing.hh"

using std::array;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

void analyze(const char *fileName, const int nPixel)
{
    TFile *resultsFile = new TFile(fileName, "READ");
    TTree *hitsTree = (TTree *)resultsFile->Get("Hits");
    TTree *energyTree = (TTree *)resultsFile->Get("Total Edep");

    // get z values
    map<int, double> hitsMap = data::read_hits(hitsTree, nPixel);
    cout << "\nAVG ENERGY DEPOSITION PER PIXEL" << endl;
    cout << "-------------------------------" << endl;
    functions::print_map(hitsMap);

    // get z values
    map<int, double> energyMap = data::read_energy_deposition(energyTree);
    // cout << "\nTOTAL ENERGY DEPOSITION PER EVENT" << endl;
    // cout << "-------------------------------" << endl;
    // functions::print_map(energyMap);

    // generate (x,y) points
    map<int, array<double, 2>> pixelMap = data::reconstruct_grid(nPixel);

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
