#include "event.hh"
#include "hits.hh"
#include "construction.hh"

MyEventAction::MyEventAction(MyRunAction *)
{
    index = -1;
}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *) {}

void MyEventAction::EndOfEventAction(const G4Event *event)
{
    // find index of HC
    if (index < 0)
        index = G4SDManager::GetSDMpointer()->GetCollectionID("SensitiveDetector/MyHitsCollection");

    // retrieve all HCs
    G4HCofThisEvent *HCE = event->GetHCofThisEvent();

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
            AddToMap(stepData);

            G4cout << "Detector ID: " << stepData.first << G4endl;
            G4cout << "Energy deposition: " << stepData.second << G4endl;
        }
    }
}

void MyEventAction::AddToMap(std::pair<G4int, G4double> pair)
{
}
