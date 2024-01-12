#include "event_action.hh"

#include <vector>

#include "hits.hh"
#include "detector_construction.hh"
#include "sensitive_detector.hh"
#include "charge_sharing.hh"
#include "generator.hh"
#include "constants.hh"

#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

G4int MyEventAction::nEvents = 0;

/**
 * The constructor.
 *
 * @param[in] run The pointer to the run action.
 */
MyEventAction::MyEventAction(MyRunAction *run) : index(-1), myRun(run) {}

/**
 * Geant4 function called at the beginning of the event.
 * It creates the vector containing the energy deposition of each pixel (per event).
 *
 * @param Event The pointer to the current event.
 */
void MyEventAction::BeginOfEventAction(const G4Event *Event)
{
    energyVector = std::vector<G4double>(N_SUBPIXEL * N_SUBPIXEL, 0.);
    energyVectorMerge = std::vector<G4double>(N_PIXEL * N_PIXEL, 0.);

    myRun->ClearVectors();
    escapeVector = {};

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
        man->FillNtupleIColumn(0, 6, MyEventAction::GetNEvents());
        man->FillNtupleIColumn(0, 7, BEAM_WIDTH);
        man->AddNtupleRow(0);
    }
}

/**
 * Geant4 function called at the end of the event.
 * It saves the energy deposition in a ROOT tree and adds the charge sharing.
 *
 * @param[in] Event The pointer to the event.
 */
void MyEventAction::EndOfEventAction(const G4Event *Event)
{
    readHitsCollection(Event);

    if (!(Event->GetEventID() % 10000) && Event->GetEventID() != 0)
        G4cout << Event->GetEventID() << " events processed." << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleIColumn(1, 0, Event->GetEventID());

    // no charge sharing and no merge
    for (int i = 0; i < energyVector.size(); i++)
        myRun->AddEntry(i, energyVector[i]);

    // vector isn't empty (photon has interacted)
    if (!IsVectorEmpty(energyVector))
    {
        // add charge sharing
        energyVector = charge_sharing::add_charge_sharing(energyVector, N_PARTS_ENERGY);

        // save energy depositions
        for (int i = 0; i < energyVector.size(); i++)
            myRun->AddEntryCS(i, energyVector[i]);
    }

    MergePixels();

    if (!IsVectorEmpty(energyVectorMerge))
    {
        for (int i = 0; i < energyVectorMerge.size(); i++)
        {
            if (energyVectorMerge[i] > 0)
                myRun->AddEntryMerge(i, energyVectorMerge[i]);
        }
    }

    if (escapeVector.size())
    {
        for (int i = 0; i < escapeVector.size(); i++)
            myRun->AddEnergyEscape(escapeVector[i]);
    }

    man->AddNtupleRow(1);
}

/**
 * Function for reading the HitsCollection and saving its data
 * inside the `energyVector`.
 *
 * @param[in] Event The pointer to the current event.
 */
void MyEventAction::readHitsCollection(const G4Event *Event)
{
    // find index of HC
    if (index < 0)
        index = G4SDManager::GetSDMpointer()->GetCollectionID("SensitiveDetector/MyHitsCollection");

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
            MyHit *hit = (*hitsColl)[i];

            // add step info to `energyVector`
            energyVector[hit->GetID()] += hit->GetEnergy();
        }
    }
}

/**
 * Static template for getting the sum of all the elements inside an `std::vector`.
 *
 * @tparam T the type of the elements inside the vector.
 * @param[in] vector The vector to be checked.
 *
 * @return The aforementioned sum.
 */
template <typename T>
T MyEventAction::VectorSum(std::vector<T> vector)
{
    T sum = 0;

    for (T d : vector)
    {
        sum += d;
    }

    return sum;
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
bool MyEventAction::IsVectorEmpty(std::vector<T> vector)
{
    for (T element : vector)
    {
        if (element != 0)
            return false;
    }

    return true;
}

/**
 * Function for merging the subpixels into bigger pixels.
 */
void MyEventAction::MergePixels()
{
    for (int i = 0; i < N_SUBPIXEL; i++)
    {
        for (int j = 0; j < N_SUBPIXEL; j++)
        {
            G4int ID = i * N_SUBPIXEL + j;
            if (energyVector[ID] > 0)
            {
                const G4int i_m = i / PIXEL_RATIO;
                const G4int j_m = j / PIXEL_RATIO;
                const G4int ID_m = i_m * N_PIXEL + j_m;
                energyVectorMerge[ID_m] += energyVector[ID];
            }
        }
    }
}