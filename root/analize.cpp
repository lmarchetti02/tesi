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

#include "functions.hh"

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

void analize(const char *fileName)
{

    TFile *resultsFile = new TFile(fileName, "READ");
    TTree *resultsTree = (TTree *)resultsFile->Get("Hits");

    map<int, double> hitsMap = read_ntuple(resultsTree);
    print_map(hitsMap);

    TGraph *graph = new TGraph();
    TCanvas *c = new TCanvas("c", "c", 800, 600);

    for (auto itr = hitsMap.begin(); itr != hitsMap.end(); itr++)
    {
        graph->SetPoint(graph->GetN(), itr->first, itr->second);
    }

    graph->SetTitle("Graph");

    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(1);

    graph->GetXaxis()->SetTitle("pixel number");
    graph->GetYaxis()->SetTitle("energy deposition");

    graph->Draw("ALP");

    resultsFile->Close();
}
