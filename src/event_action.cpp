#include "event_action.hh"

#include <vector>

#include "hits.hh"
#include "detector_construction.hh"
#include "sensitive_detector.hh"
#include "planes_hits.hh"
#include "charge_sharing.hh"
#include "generator.hh"
#include "constants.hh"

#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

G4int EventAction::nEvents = 0;

/**
 * The constructor.
 *
 * @param[in] run The pointer to the run action.
 */
EventAction::EventAction(RunAction *run, PrimaryGenerator *generator) : index(-1),
                                                                                iPlanes(-1),
                                                                                myRun(run),
                                                                                myGenerator(generator),
                                                                                stepInfo(new SaveStepInfo),
                                                                                saveCompton(new SaveCompton) {}

/**
 * Geant4 function called at the beginning of the event.
 * It creates the vector containing the energy deposition of each pixel (per event).
 *
 * @param Event The pointer to the current event.
 */
void EventAction::BeginOfEventAction(const G4Event *Event)
{

    energyVector = std::vector<G4double>(N_SUBPIXEL * N_SUBPIXEL, 0.);
    energyVectorMerge = std::vector<G4double>(N_PIXEL * N_PIXEL, 0.);
    energyVectorMergeCS = std::vector<G4double>(N_PIXEL * N_PIXEL, 0.);

    myRun->ClearVectors();
    fluorescenceEnergyVector.clear();
    fluorescenceIDVector.clear();
    stepInfo->Clear();
    saveCompton->Clear();

    eventID = Event->GetEventID();

    // save info about the simulation
    if (Event->GetEventID() == 0)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        // pixels
        man->FillNtupleIColumn(0, 0, N_PIXEL);
        man->FillNtupleDColumn(0, 1, XY_PIXEL);
        man->FillNtupleDColumn(0, 2, Z_PIXEL);
        // subpixels
        man->FillNtupleIColumn(0, 3, N_SUBPIXEL);
        man->FillNtupleDColumn(0, 4, XY_SUBPIXEL);
        man->FillNtupleDColumn(0, 5, Z_PIXEL);
        // number of events
        man->FillNtupleIColumn(0, 6, EventAction::GetNEvents());
        man->FillNtupleIColumn(0, 7, myGenerator->GetBeamWidth());
        man->AddNtupleRow(0);
    }

    initialPhotonSet = 0;
}

/**
 * Geant4 function called at the end of the event.
 * It saves the energy deposition in a ROOT tree and adds the charge sharing.
 *
 * @param[in] Event The pointer to the event.
 */
void EventAction::EndOfEventAction(const G4Event *Event)
{
    ReadHitsCollection(Event);
    ReadHitsCollectionPlanes(Event);

    if (!(Event->GetEventID() % 10000) && Event->GetEventID() != 0)
        G4cout << Event->GetEventID() << " events processed." << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleIColumn(1, 0, Event->GetEventID());
    man->FillNtupleDColumn(1, 1, myGenerator->GetEnergy()); // VM added
    man->FillNtupleIColumn(1, 2, photonID);
    man->FillNtupleIColumn(1, 3, initialPhotonNoIter);
    man->FillNtupleDColumn(1, 4, (G4double)saveCompton->GetComptonEnergy());

    G4int comptID = saveCompton->GetComptonID();
    if (saveCompton->GetComptonEnergy() < 0)
        comptID = -1;
    man->FillNtupleIColumn(1, 5, comptID);
    man->FillNtupleDColumn(1, 6, (G4double)saveCompton->GetComptonEloss());
    man->FillNtupleIColumn(1, 7, (G4int)saveCompton->GetComptonIDiter()); // <0 escaped
    man->FillNtupleDColumn(1, 8, (G4double)saveCompton->GetComptonElossiter());

    SaveEnergies();

    std::vector<G4int> fluorescenceIDchannel = stepInfo->GetIDchannelsVector();
    for (int i = 0; i < fluorescenceIDchannel.size(); i++)
    {
        G4int ID = fluorescenceIDchannel[i];
        if (ID >= 0)
        {
            G4int i_s = ID / N_SUBPIXEL;
            G4int j_s = ID - i_s * N_SUBPIXEL;
            G4int i_m = i_s / PIXEL_RATIO;
            G4int j_m = j_s / PIXEL_RATIO;
            G4int ID_m = i_m * N_PIXEL + j_m;
            fluorescenceIDchannel[i] = ID_m;
        }
    }
    std::vector<G4double> fluorescenceEnergies = stepInfo->GetEneVector();
    std::vector<G4double> fluorescenceEnergyLosses = stepInfo->GetEneLossVector();
    for (int i = 0; i < fluorescenceIDchannel.size(); i++)
    {
        if (fluorescenceEnergyLosses[i] > minEnergy || fluorescenceIDchannel[i] < 0)
            myRun->AddFluorescence(fluorescenceIDchannel[i], fluorescenceEnergies[i], fluorescenceEnergyLosses[i]);
    }

    // delete subpixels info
    if (!INCLUDE_ORIGINAL)
        myRun->ClearOriginal();
    if (!INCLUDE_CS)
        myRun->ClearCS();

    man->AddNtupleRow(1);
}

/**
 * Function for reading the HitsCollection and saving its data
 * inside the `energyVector`.
 *
 * @param[in] Event The pointer to the current event.
 */
void EventAction::ReadHitsCollection(const G4Event *Event)
{
    // find index of HC
    if (index < 0)
        index = G4SDManager::GetSDMpointer()->GetCollectionID("Pixels/PixelsHC");

    // get HC of the current event
    G4HCofThisEvent *HCE = Event->GetHCofThisEvent();

    MyHitsCollection *hitsColl;
    if (HCE)
        hitsColl = (MyHitsCollection *)(HCE->GetHC(index));

    // get values inside HC
    if (hitsColl)
    {
        int nEntries = hitsColl->entries();

        for (int i = 0; i < nEntries; i++)
        {
            PixelsHit *hit = (*hitsColl)[i];

            // add step info to `energyVector`
            energyVector[hit->GetID()] += hit->GetEnergy();
        }
    }
}

/**
 * Function for reading the HitsCollection and saving its data
 * inside the `energyVector`.
 *
 * @param[in] Event The pointer to the current event.
 */
void EventAction::ReadHitsCollectionPlanes(const G4Event *Event)
{
    // find index of HC
    if (iPlanes < 0)
        iPlanes = G4SDManager::GetSDMpointer()->GetCollectionID("Planes/PlanesHC");

    // get HC of the current event
    G4HCofThisEvent *HCE = Event->GetHCofThisEvent();

    PlanesHitsCollection *hitsColl;
    if (HCE)
        hitsColl = (PlanesHitsCollection *)(HCE->GetHC(iPlanes));

    // get values inside HC
    if (hitsColl)
    {
        int nEntries = hitsColl->entries();

        for (int i = 0; i < nEntries; i++)
        {
            PlanesHits *hit = (*hitsColl)[i];

            // add step info to `energyVector`
            G4cout << "\033[38:5:160m"
                   << "ID = " << hit->GetID() << " Energy= " << hit->GetEnergy() << "\033[0m" << G4endl;
        }
    }
}

/**
 * Static template for checking if a vector has only zero values.
 *
 * @tparam T The type of the elements in the vector.
 * @param[in] vector The vector to be checked.
 *
 * @return `true` if the vector only contains 0, `false` otherwise.
 */
template <typename T>
bool EventAction::IsVectorEmpty(std::vector<T> vector)
{
    for (T element : vector)
    {
        if (element != 0)
            return false;
    }

    return true;
}

/**
 * Function for saving the pixels energies to the
 * vectors, which are then stored into the ROOT file.
 */
void EventAction::SaveEnergies()
{
    // no charge sharing and not merged
    for (int i = 0; i < energyVector.size(); i++)
        myRun->AddEntry(i, energyVector[i]);

    // energy vector isn't empty (photon has interacted)
    if (!IsVectorEmpty(energyVector))
    {
        // no charge sharing but merged
        MergePixels(energyVector, energyVectorMerge);
        for (int i = 0; i < energyVectorMerge.size(); i++)
            myRun->AddEntryMerge(i, energyVectorMerge[i]);

        // add charge sharing
        energyVector = charge_sharing::add_charge_sharing(energyVector, N_PARTS_ENERGY);

        // with charge sharing but not merged
        for (int i = 0; i < energyVector.size(); i++)
            myRun->AddEntryCS(i, energyVector[i]);

        // with charge sharing and merged
        MergePixels(energyVector, energyVectorMergeCS);
        for (int i = 0; i < energyVectorMergeCS.size(); i++)
            myRun->AddEntryMergeCS(i, energyVectorMergeCS[i]);
    }
}

/**
 * Function for merging the subpixels into bigger pixels.
 *
 * @param[in] inVector The vector to be merged.
 * @param[inout] outVector The reference the the merged vector.
 */
void EventAction::MergePixels(std::vector<G4double> inVector, std::vector<G4double> &outVector)
{
    for (int i = 0; i < N_SUBPIXEL; i++)
    {
        for (int j = 0; j < N_SUBPIXEL; j++)
        {
            G4int ID = i * N_SUBPIXEL + j;
            if (inVector[ID] > 0)
            {
                const G4int i_m = i / PIXEL_RATIO;
                const G4int j_m = j / PIXEL_RATIO;
                const G4int ID_m = i_m * N_PIXEL + j_m;

                outVector[ID_m] += inVector[ID];
            }
        }
    }
}

// VM added
void EventAction::AddStepInfo(G4int partID, G4int parentID, G4int partType, G4int volID, G4double ene, G4double eneloss)
{
    if (partType == -2 && ene < myGenerator->GetEnergy())
    {
        // G4cout << " escaped partID=" << partID << " parentID=" << parentID << " ene=" << ene << " photonEnergy=" << myGenerator->GetEnergy() << G4endl;
    }
    else if (partType == -1)
    {
        stepInfo->AddStep(partID, parentID, partType, partType, ene, 0);
    }
    else if (partType >= 0)
        stepInfo->AddStep(partID, parentID, partType, volID, ene, eneloss);
}

void EventAction::AddComptonInfo(G4int partID, G4int parentID, G4int partType, G4int volID, G4double ene,
                                   G4double eneloss, G4String processname)
{
    saveCompton->AddStep(partID, parentID, partType, volID, ene, eneloss, eventID, processname);
}

void EventAction::SetInitialPhoton(G4int copyNo, G4double ene)
{
    if (!initialPhotonSet)
    {
        G4int i_s = copyNo / N_SUBPIXEL;
        G4int j_s = copyNo - i_s * N_SUBPIXEL;
        G4int i_m = i_s / PIXEL_RATIO;
        G4int j_m = j_s / PIXEL_RATIO;
        G4int ID_m = i_m * N_PIXEL + j_m;
        photonID = ID_m;
        photonEnergy = myGenerator->GetEnergy();
        initialPhotonSet = 1;
        initialPhotonNoIter = 0;
    }
    else
    {
        if (copyNo == 2 && ene == photonEnergy)
        {
            // G4cout << "initialPhoton has not interacted" << G4endl;
            initialPhotonNoIter = 1;
        }
    }
}
