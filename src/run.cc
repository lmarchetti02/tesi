#include "run.hh"
#include "construction.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->SetNtupleMerging(true);

    // rows
    man->CreateNtuple("Hits", "Hits");
    // columns
    man->CreateNtupleIColumn("ID");
    man->CreateNtupleDColumn("eVector");
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // get run number
    G4String runNumber = to_string(run->GetRunID());

    // create file to store run
    man->OpenFile("../results/output" + runNumber + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance(); // singleton

    // close file used to store run
    man->Write();
    man->CloseFile();
}
