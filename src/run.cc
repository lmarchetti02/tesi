#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // rows
    man->CreateNtuple("Output", "Output");
    // columns
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleIColumn("fDetID");
    man->CreateNtupleDColumn("fEdep");
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
    //
    G4AnalysisManager *man = G4AnalysisManager::Instance(); // singleton

    // close file used to store run
    man->Write();
    man->CloseFile();
}
