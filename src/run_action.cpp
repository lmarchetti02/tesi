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
    man->CreateNtupleIColumn("Pixel_N");
    man->CreateNtupleDColumn("Pixels_xy_dim");
    man->CreateNtupleDColumn("Pixels_z_dim");
    man->CreateNtupleIColumn("Subpixel_N");
    man->CreateNtupleDColumn("Subpixels_xy_dim");
    man->CreateNtupleDColumn("Subpixels_z_dim");
    man->CreateNtupleIColumn("Event_N");
    man->CreateNtupleIColumn("Beam_Width");
    man->FinishNtuple(0);

    man->CreateNtuple("Event", "Event");
    man->CreateNtupleIColumn("Event_ID");
    man->CreateNtupleIColumn("ID", pixelIDVector);
    man->CreateNtupleDColumn("Energy", pixelEnergyVector);
    man->CreateNtupleIColumn("ID_CS", pixelIDVectorCS);
    man->CreateNtupleDColumn("Energy_CS", pixelEnergyVectorCS);
    man->CreateNtupleIColumn("ID_Merge", pixelIDVectorMerge);
    man->CreateNtupleDColumn("Energy_Merge", pixelEnergyVectorMerge);
    man->CreateNtupleDColumn("Energy_Escape", energyEscape);
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
    pixelIDVectorMerge.clear();
    pixelEnergyVectorMerge.clear();
    energyEscape.clear();
}

/**
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (no charge sharing and no merge).
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
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (with charge sharing and no merge).
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

/**
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (with charge sharing and merge).
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void MyRunAction::AddEntryMerge(G4int ID, G4double Energy)
{
    if (Energy != 0)
    {
        pixelIDVectorMerge.push_back(ID);
        pixelEnergyVectorMerge.push_back(Energy);
    }
}

/**
 * Function for adding the energy escaped from the
 * detector to the `energyEscape` vector.
 *
 * @param[in] Energy The energy deposition in the pixel.
 */
void MyRunAction::AddEnergyEscape(G4double Energy)
{
    if (Energy != 0)
        energyEscape.push_back(Energy);
}
