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

    // Ntuple for info
    man->CreateNtuple("Info", "Info");
    man->CreateNtupleIColumn("Pixel N");
    man->CreateNtupleDColumn("Pixels xy-dim");
    man->CreateNtupleDColumn("Pixels z-dim");
    man->CreateNtupleIColumn("Subpixel N");
    man->CreateNtupleDColumn("Subpixels xy-dim");
    man->CreateNtupleDColumn("Subpixels z-dim");
    man->CreateNtupleIColumn("Event N");
    man->FinishNtuple(0);

    man->CreateNtuple("Event", "Event");
    man->CreateNtupleIColumn("Event ID");
    man->CreateNtupleIColumn("ID", pixelIDVector);
    man->CreateNtupleDColumn("Energy", pixelEnergyVector);
    man->CreateNtupleIColumn("ID CS", pixelIDVectorCS);
    man->CreateNtupleDColumn("Energy CS", pixelEnergyVectorCS);
    man->FinishNtuple(1);
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
    man->OpenFile("../results/output" + runNumber + ".root");

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

/**
 * Function for clearing the vectors used for saving
 * info into the ROOT file.
 */
void MyRunAction::ClearVectors()
{
    pixelIDVector.clear();
    pixelEnergyVector.clear();
    pixelIDVectorCS.clear();
    pixelEnergyVectorCS.clear();
}

/**
 * Function for adding ID and Energy the vectors used for saving
 * info into the ROOT file.
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void MyRunAction::AddEntry(G4int ID, G4double Energy)
{
    if (Energy != 0)
    {
        pixelIDVector.push_back(ID);
        pixelEnergyVector.push_back(Energy);
    }
}

/**
 * Function for adding ID and Energy the vectors used for saving
 * info into the ROOT file.
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void MyRunAction::AddEntryCS(G4int ID, G4double Energy)
{
    if (Energy != 0)
    {
        pixelIDVectorCS.push_back(ID);
        pixelEnergyVectorCS.push_back(Energy);
    }
}