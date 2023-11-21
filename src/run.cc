#include "run.hh"
#include "construction.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);

    // Ntuple for hits
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("ID");
    man->CreateNtupleDColumn("eVector");
    man->FinishNtuple(0);

    // Ntuple for pixels
    man->CreateNtuple("First Hit", "First Hit");
    man->CreateNtupleDColumn("x");
    man->CreateNtupleDColumn("y");
    // man->CreateNtupleDColumn("z");
    man->FinishNtuple(1);

    // Ntuple for total energy deposition
    man->CreateNtuple("Total Edep", "Total Edep");
    man->CreateNtupleDColumn("eDep");
    // man->CreateNtupleDColumn("z");
    man->FinishNtuple(2);

    // Ntuple for pixels
    // man->CreateNtuple("Pixels", "Pixels");
    // man->CreateNtupleIColumn("nPixel");
    // man->FinishNtuple(1);
}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // create ROOT file
    G4String runNumber = to_string(run->GetRunID());
    man->OpenFile("../root/results/output" + runNumber + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance(); // singleton

    // close file used to store run
    man->Write();
    man->CloseFile();
}
