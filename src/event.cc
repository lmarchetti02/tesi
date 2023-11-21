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

G4double MyEventAction::VectorSum(const vector<G4double> &vector)
{
    G4double sum = 0;

    for (G4double d : vector)
    {
        sum += d;
    }

    return sum;
}
