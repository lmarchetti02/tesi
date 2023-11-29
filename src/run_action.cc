#include "run_action.hh"
#include "construction.hh"

#include "G4AnalysisManager.hh"

/**
 * The default constructor.
 *
 * It creates all the NTuples necessary for saving
 * the results of the simulation.
 */
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
    man->FinishNtuple(1);

    // Ntuple for total energy deposition
    man->CreateNtuple("Total Edep", "Total Edep");
    man->CreateNtupleDColumn("eDep");
    man->FinishNtuple(2);
}

/**
 * Geant4 function called at the beginning of the run action.
 *
 * It creates the ROOT files where the NTuples are saved.
 *
 * @param run A pointer to the current `G4Run` object.
 */
void MyRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4String runNumber = to_string(run->GetRunID());
    man->OpenFile("../root/results/output" + runNumber + ".root");
}

/**
 * Geant4 function called at the end of the run action.
 *
 *  Saves the data in the ROOT files and closes them.
 *
 * @param run Not used.
 */
void MyRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance(); // singleton

    man->Write();
    man->CloseFile();
}
