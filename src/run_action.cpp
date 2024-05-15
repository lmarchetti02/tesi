#include "run_action.hh"

#include "G4AnalysisManager.hh"
#include "event_action.hh"

/**
 * The default constructor.
 *
 * It creates all the NTuples necessary for saving
 * the results of the simulation.
 */
RunAction::RunAction()
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
    man->CreateNtupleIColumn("Beam_Type");
    man->FinishNtuple(0);

    man->CreateNtuple("Event", "Event");
    man->CreateNtupleIColumn("Event_ID");
    man->CreateNtupleDColumn("Photon_energy");
    man->CreateNtupleIColumn("Photon_ID");
    man->CreateNtupleIColumn("Photon_NoInt");
    man->CreateNtupleDColumn("Compton_energy");
    man->CreateNtupleIColumn("Compton_ID_primary");
    man->CreateNtupleDColumn("Compton_eneloss_primary");
    man->CreateNtupleIColumn("Compton_ID_interaction"); // <0 escaped
    man->CreateNtupleDColumn("Compton_eneloss_interaction");
    man->CreateNtupleIColumn("ID", pixelIDVector);
    man->CreateNtupleDColumn("Energy", pixelEnergyVector);
    man->CreateNtupleIColumn("ID_CS", pixelIDVectorCS);
    man->CreateNtupleDColumn("Energy_CS", pixelEnergyVectorCS);
    man->CreateNtupleIColumn("ID_Merge", pixelIDVectorMergeCS);
    man->CreateNtupleDColumn("Energy_Merge", pixelEnergyVectorMergeCS);
    man->CreateNtupleIColumn("ID_Merge_NOCS", pixelIDVectorMerge);
    man->CreateNtupleDColumn("Energy_Merge_NOCS", pixelEnergyVectorMerge);
    man->CreateNtupleDColumn("Energy_Fluoro", energyFluorescence);
    man->CreateNtupleIColumn("ID_Fluoro", pixelIDFluorescence); // -1 escape else id interaction fluorescence
    man->CreateNtupleDColumn("EneLoss_Fluoro", energyLossFluorescence);
    man->FinishNtuple(1);
}

/**
 * Geant4 function called at the beginning of the run action.
 *
 * It creates the ROOT files where the NTuples are saved.
 *
 * @param run A pointer to the current `G4Run` object.
 */
void RunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4String runNumber = to_string(run->GetRunID());
    man->OpenFile("../results/output" + runNumber + ".root"); // VM

    // get total number of events
    EventAction::SetNEvents(run->GetNumberOfEventToBeProcessed());
}

/**
 * Geant4 function called at the end of the run action.
 *
 *  Saves the data in the ROOT files and closes them.
 *
 * @param run Not used.
 */
void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance(); // singleton

    man->Write();
    man->CloseFile();

    // reset number of events
    EventAction::SetNEvents(0);
}

/**
 * Function for clearing the vectors used for saving
 * info into the ROOT file.
 */
void RunAction::ClearVectors()
{
    pixelIDVector.clear();
    pixelEnergyVector.clear();
    pixelIDVectorCS.clear();
    pixelEnergyVectorCS.clear();
    pixelIDVectorMerge.clear();
    pixelEnergyVectorMerge.clear();
    pixelIDVectorMergeCS.clear();
    pixelEnergyVectorMergeCS.clear();
    energyFluorescence.clear();
    pixelIDFluorescence.clear();
    energyLossFluorescence.clear();
}

/**
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (no charge sharing and no merge).
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void RunAction::AddEntry(G4int ID, G4double Energy)
{
    if (Energy != 0) {
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
void RunAction::AddEntryCS(G4int ID, G4double Energy)
{
    if (Energy != 0) {
        pixelIDVectorCS.push_back(ID);
        pixelEnergyVectorCS.push_back(Energy);
    }
}

/**
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (without charge sharing and merge).
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void RunAction::AddEntryMerge(G4int ID, G4double Energy)
{
    if (Energy != 0) {
        pixelIDVectorMerge.push_back(ID);
        pixelEnergyVectorMerge.push_back(Energy);
    }
}

/**
 * Function for adding ID and Energy to the vectors used for saving
 * info into the ROOT file (with charge sharing and merge).
 *
 * @param[in] ID The ID of the pixel.
 * @param[in] Energy The energy deposition in the pixel.
 */
void RunAction::AddEntryMergeCS(G4int ID, G4double Energy)
{
    if (Energy != 0) {
        pixelIDVectorMergeCS.push_back(ID);
        pixelEnergyVectorMergeCS.push_back(Energy);
    }
}

/**
 * Function for adding the energy of the fluorescence photons
 * to the vector used for saving info into the ROOT file.
 */
void RunAction::AddFluorescence(G4int ID, G4double Energy, G4double energyLoss)
{
    pixelIDFluorescence.push_back(ID);
    energyFluorescence.push_back(Energy);
    energyLossFluorescence.push_back(energyLoss);
}

/**
 * Function for clearing the subpixels
 * energies (before charge sharing).
 */
void RunAction::ClearOriginal()
{
    pixelIDVector.clear();
    pixelEnergyVector.clear();
}

/**
 * Function for clearing the subpixels
 * energies (after charge sharing).
 */
void RunAction::ClearCS()
{
    pixelIDVectorCS.clear();
    pixelEnergyVectorCS.clear();
}
