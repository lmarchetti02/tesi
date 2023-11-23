#include "detector.hh"

G4ThreeVector MySensitiveDetector::firstHit = G4ThreeVector();
G4int MySensitiveDetector::pixelFirstHit = G4int();

// SensitiveDetectorName and collectionName are data members of G4VSensitiveDetector

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    collectionName.insert("MyHitsCollection");
}

MySensitiveDetector::~MySensitiveDetector() {}

void MySensitiveDetector::Initialize(G4HCofThisEvent *HCE)
{
    ProcessHitsCounter = 0;
    InitializeFirstHit();

    hitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);

    if (HCID < 0)
        HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, hitsCollection);
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    // get the id of the detector that interacted w/ photon
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // update firstHit vector and pixel
    if (ProcessHitsCounter == 0)
    {
        G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
        G4ThreeVector posPhoton = preStepPoint->GetPosition();

        G4ThreeVector firstHit = G4ThreeVector(posPhoton[0], posPhoton[1], 0);
        SetFirstHit(firstHit);
        SetPixelFirstHit(copyNo);

        ProcessHitsCounter++;
    }

    // energy deposition of the step
    G4double eDep = aStep->GetTotalEnergyDeposit();

    // save step in HC
    MyHit *hit = new MyHit();
    hit->SetEnergy(eDep);
    hit->SetID(copyNo);

    hitsCollection->insert(hit);

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE)
{
    static G4int HCID = -1;
    if (HCID < 0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, hitsCollection);
}