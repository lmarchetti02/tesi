#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <array>
#include <string>
#include <tuple>

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

/**
 * Struct containing all the information
 * about the simulation.
 */
struct SimulationInfo
{
    int nPixel;
    double xPixel;
    double yPixel;
    double zPixel;
    long int nEvents;
};

SimulationInfo get_simulation_info(TTree *);

void analyze_3d(const char *fileName)
{
    TFile *resultsFile = new TFile(fileName, "READ");

    // get trees
    TTree *hitsTree, *csTree, *infoTree;
    hitsTree = static_cast<TTree *>(resultsFile->Get("Hits"));
    csTree = static_cast<TTree *>(resultsFile->Get("CS"));
    infoTree = static_cast<TTree *>(resultsFile->Get("Info"));

    // get info about the simulation
    SimulationInfo si = get_simulation_info(infoTree);

    // get hits from tree
    std::map<int, double> hitsMap = data::read_hits_tree_full(hitsTree, si.nPixel);
    functions::print_hits_map(hitsMap);

    std::map<int, double> csMap = data::read_hits_tree_full(csTree, si.nPixel);
    functions::print_hits_map(csMap);

    // close file
    resultsFile->Close();
    delete resultsFile;
    resultsFile = nullptr;

    // get (x,y) of pixels center
    std::map<int, std::array<double, 2>> pixelMap = data::reconstruct_grid(si.nPixel, si.xPixel);

    TCanvas *histCanvas = new TCanvas("hitsCanvas", "Energy Deposition (No Charge Sharing)", 800, 600);
    TCanvas *csCanvas = new TCanvas("csCanvas", "Energy Deposition (Charge Sharing)", 800, 600);
    TGraph2D *hitsGraph = new TGraph2D();
    TGraph2D *csGraph = new TGraph2D();

    for (int i = 0; i < si.nPixel * si.nPixel; i++)
    {
        hitsGraph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], hitsMap[i]);
    }
    for (int i = 0; i < si.nPixel * si.nPixel; i++)
    {
        csGraph->SetPoint(i, pixelMap[i][0], pixelMap[i][1], csMap[i]);
    }

    gStyle->SetPalette(1);
    histCanvas->cd();
    hitsGraph->Draw("surf1");

    csCanvas->cd();
    csGraph->Draw("surf1");

    data::save_results(pixelMap, hitsMap);
}

/**
 * Function for reading the info about the simulation from the
 * ROOT file.
 *
 * @param[in] Tree The pointer to the `TTree` where the info is stored.
 *
 * @return The `SimulationInfo` struct.
 */
SimulationInfo get_simulation_info(TTree *Tree)
{
    int nPixel;
    double xPixel;
    double yPixel;
    double zPixel;
    int nEvents;

    Tree->SetBranchAddress("Pixel N", &nPixel);
    Tree->SetBranchAddress("Pixels x-dim", &xPixel);
    Tree->SetBranchAddress("Pixels y-dim", &yPixel);
    Tree->SetBranchAddress("Pixels z-dim", &zPixel);
    Tree->SetBranchAddress("Event N", &nEvents);

    Tree->GetEntry(0);

    return {nPixel, xPixel, yPixel, zPixel, nEvents};
}
