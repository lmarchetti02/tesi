#include "event.hh"
#include "hits.hh"
#include "construction.hh"
#include "sensitive_detector.hh"
#include "charge_sharing.hh"

MyEventAction::MyEventAction(MyRunAction *)
{
    // index of HC
    index = -1;
}

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
}

/**
 * Geant4 function called at the end of the event.
 * It saves the energy deposition in a ROOT tree and adds the charge sharing.
 *
 * @param[in] Event The pointer to the event.
 */
void MyEventAction::EndOfEventAction(const G4Event *Event)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // find index of HC
    if (index < 0)
        index = G4SDManager::GetSDMpointer()->GetCollectionID("SensitiveDetector/MyHitsCollection");

    // retrieve all HCs
    G4HCofThisEvent *HCE = Event->GetHCofThisEvent();

    // retrieve HCs by index
    MyHitsCollection *hitsColl;
    if (HCE)
        hitsColl = (MyHitsCollection *)(HCE->GetHC(index));

    if (hitsColl)
    {
        int nEntries = hitsColl->entries();

        for (int i = 0; i < nEntries; i++)
        {
            MyHit *hit = (*hitsColl)[i];

            // pair (ID, eDep) of the step
            std::pair<G4int, G4double> stepData(hit->GetID(), hit->GetEnergy());

            energyVector[stepData.first] += stepData.second;
        }
    }

    // add charge sharing
    G4int nPixel = MyDetectorConstruction::GetNPixel();
    energyVector = charge_sharing::add_charge_sharing(energyVector, nPixel * 100);

    // save energy depositions
    for (int i = 0; i < energyVector.size(); i++)
    {
        if (energyVector[i] != 0)
        {
            man->FillNtupleIColumn(0, 0, i);
            man->FillNtupleDColumn(0, 1, energyVector[i]);
            man->AddNtupleRow(0);
        }
    }

    man->FillNtupleDColumn(1, 0, MySensitiveDetector::GetFirstHit()[0]);
    man->FillNtupleDColumn(1, 1, MySensitiveDetector::GetFirstHit()[1]);
    // man->FillNtupleDColumn(1, 2, MySensitiveDetector::GetFirstHit()[2]);
    man->AddNtupleRow(1);

    man->FillNtupleDColumn(2, 0, VectorSum(energyVector));
    man->AddNtupleRow(2);
}

/**
 * Template for getting the sum of all the elements inside an `std::vector`.
 *
 * @tparam T the type of the elements inside the vector.
 */
template <typename T>
T MyEventAction::VectorSum(const vector<T> &vector)
{
    T sum = 0;

    for (T d : vector)
    {
        sum += d;
    }

    return sum;
}
