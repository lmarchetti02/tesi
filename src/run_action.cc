#include "run_action.hh"

#include "G4AnalysisManager.hh"

#include "detector_construction.hh"
#include "event_action.hh"

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

    // Ntuple for hits (no CS)
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("ID");
    man->CreateNtupleDColumn("Energy");
    man->CreateNtupleIColumn("Event");
    man->FinishNtuple(0);

    // Ntuple for hits (with CS)
    man->CreateNtuple("CS", "CS");
    man->CreateNtupleIColumn("ID");
    man->CreateNtupleDColumn("Energy");
    man->CreateNtupleIColumn("Event");
    man->FinishNtuple(1);

    // Ntuple for info
    man->CreateNtuple("Info", "Info");
    man->CreateNtupleIColumn("Pixel N");
    man->CreateNtupleDColumn("Pixels x-dim");
    man->CreateNtupleDColumn("Pixels y-dim");
    man->CreateNtupleDColumn("Pixels z-dim");
    man->CreateNtupleIColumn("Event N");
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

    // get total number of events
    MyEventAction::SetNEvents(run->GetNumberOfEventToBeProcessed());
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

    // reset number of events
    MyEventAction::SetNEvents(0);
}
