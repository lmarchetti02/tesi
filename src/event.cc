#include "event.hh"
#include "hits.hh"
#include "construction.hh"
#include "detector.hh"

MyEventAction::MyEventAction(MyRunAction *)
{
    // index of HC
    index = -1;
}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *)
{
    G4int nPixel = MyDetectorConstruction::GetNPixel();
    energyVector = vector<G4double>(nPixel * nPixel, 0.);
}

void MyEventAction::EndOfEventAction(const G4Event *event)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

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

            energyVector[stepData.first] += stepData.second;
        }
    }

    for (int i = 0; i < energyVector.size(); i++)
    {
        if (energyVector[i] != 0)
        {
            man->FillNtupleIColumn(0, i);
            man->FillNtupleDColumn(1, energyVector[i]);
            man->AddNtupleRow(0);
        }
    }

    G4cout << "First hit: " << MySensitiveDetector::GetFirstHit() << G4endl;
}
