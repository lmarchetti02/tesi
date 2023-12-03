#include "event_action.hh"

#include "hits.hh"
#include "detector_construction.hh"
#include "sensitive_detector.hh"
#include "charge_sharing.hh"

#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

G4int MyEventAction::nEvents = 0;

/**
 * The constructor.
 *
 * @param[in] run Not used
 */
MyEventAction::MyEventAction(MyRunAction *run) : index(-1) {}

/**
 * Geant4 function called at the beginning of the event.
 * It creates the vector containing the energy deposition of each pixel (per event).
 *
 * @param Event Not used.
 */
void MyEventAction::BeginOfEventAction(const G4Event *Event)
{
    G4int nPixel = MyDetectorConstruction::GetNPixel();
    energyVector = vector<G4double>(nPixel * nPixel, 0.);

    // save info about the simulation
    if (Event->GetEventID() == 0)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(2, 0, MyDetectorConstruction::GetNPixel());
        man->FillNtupleDColumn(2, 1, MyDetectorConstruction::GetPixelDimensions()[1]);
        man->FillNtupleDColumn(2, 2, MyDetectorConstruction::GetPixelDimensions()[2]);
        man->FillNtupleDColumn(2, 3, MyDetectorConstruction::GetPixelDimensions()[3]);
        man->FillNtupleIColumn(2, 4, MyEventAction::GetNEvents());
        man->AddNtupleRow(2);
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

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    for (int i = 0; i < energyVector.size(); i++)
    {
        if (energyVector[i] != 0)
        {
            man->FillNtupleIColumn(0, 0, i);
            man->FillNtupleDColumn(0, 1, energyVector[i]);
            man->FillNtupleIColumn(0, 2, Event->GetEventID());
            man->AddNtupleRow(0);
        }
    }

    // vector isn't empty (photon has interacted)
    if (!IsVectorEmpty(energyVector))
    {
        // add charge sharing
        G4int nPixel = MyDetectorConstruction::GetNPixel();
        energyVector = charge_sharing::add_charge_sharing(energyVector, nPixel * 10);

        // save energy depositions
        for (int i = 0; i < energyVector.size(); i++)
        {
            if (energyVector[i] != 0)
            {
                man->FillNtupleIColumn(1, 0, i);
                man->FillNtupleDColumn(1, 1, energyVector[i]);
                man->FillNtupleIColumn(1, 2, Event->GetEventID());
                man->AddNtupleRow(1);
            }
        }

        return;
    }

    // vector is empty (photon hasn't interact)
    man->FillNtupleIColumn(1, 0, -1);
    man->FillNtupleDColumn(1, 1, 0);
    man->FillNtupleIColumn(1, 2, Event->GetEventID());
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

            // pair (ID, eDep) of the step
            std::pair<G4int, G4double> stepData(hit->GetID(), hit->GetEnergy());

            // add step info to `energyVector`
            energyVector[stepData.first] += stepData.second;
        }
    }
}

/**
 * Static function for setting the static variable `nEvents`,
 * which represents the number of events processed in the run.
 *
 * @param[in] N The number of events.
 */
void MyEventAction::SetNEvents(G4int N)
{
    nEvents = N;
}

/**
 * Static function for getting the static variable `nEvents`,
 * which represents the number of events processed in the run.
 *
 * @return The number of events.
 */
G4int MyEventAction::GetNEvents()
{
    return nEvents;
}

/**
 * Static template for getting the sum of all the elements inside an `std::vector`.
 *
 * @tparam T the type of the elements inside the vector.
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
