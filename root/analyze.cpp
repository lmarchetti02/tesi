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

    // GET TTRESS FROM GEANT4 SIMULATION
    // **********************************************************
    TFile *resultsFile = new TFile(fileName, "READ");

    // get trees from file
    TTree *hitsTree = (TTree *)resultsFile->Get("Hits");
    TTree *energyTree = (TTree *)resultsFile->Get("Total Edep");
    TTree *fhTree = (TTree *)resultsFile->Get("First Hit");
    // **********************************************************

    // GET AVERAGE ENERGY DEPOSITION PER PIXEL
    // ************************************************************
    map<int, double> hitsMap = data::read_hits(hitsTree, nPixel);
    cout << "\nAVG ENERGY DEPOSITION PER PIXEL" << endl;
    cout << "-------------------------------" << endl;
    functions::print_map(hitsMap);
    // ************************************************************

    // GET MAP WITH {#evt : (fh_x, fh_y, E_dep)}
    // *******************************************************************************
    map<int, array<double, 3>> csMap = charge_sharing::get_cs_map(fhTree, energyTree);
    cout << "\nCHARGE SHARING MAP" << endl;
    cout << "------------------" << endl;
    functions::print_map(csMap);

    // close file
    resultsFile->Close();
    // *******************************************************************************

    // PLOT SIMULATION DATA
    // *****************************************************************************************
    // get (x,y) of pixels center
    map<int, array<double, 2>> pixelMap = data::reconstruct_grid(nPixel);

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    TGraph2D *graph = new TGraph2D();

    for (int i = 0; i < nPixel * nPixel; i++)
    {
        graph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], hitsMap[i]);
    }

    gStyle->SetPalette(1);
    graph->Draw("surf1");
    // *****************************************************************************************

    // OUTPUT DATA TO TEXT FILE
    // *****************************************************************************************
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
    // *****************************************************************************************
}
