#include "sensitive_detector.hh"

G4ThreeVector MySensitiveDetector::firstHit = G4ThreeVector();
G4int MySensitiveDetector::pixelFirstHit = G4int();

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    // SensitiveDetectorName and collectionName are data members of G4VSensitiveDetector
    collectionName.insert("MyHitsCollection");
}

/**
 * Geant4 function for initializing the hits collection relative to the sensitive detector.
 *
 * @param[in,out] HCE Pointer to the hits collection of the event.
 */
void MySensitiveDetector::Initialize(G4HCofThisEvent *HCE)
{
    ProcessHitsCounter = 0;

    InitializeFirstHit();

    hitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);

    if (HCID < 0)
        HCID = GetCollectionID(0);

    HCE->AddHitsCollection(HCID, hitsCollection);
}

/**
 * Geant4 function for processing the hits. It gets called for every step inside the SD.
 *
 *
 * @param[in] aStep One of the steps to be processed.
 * @param ROhist Not used.
 *
 * @return `true`/`false` (Old Geant4 feature not used anymore).
 */
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
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

    return false;
}

/**
 * Geant4 function called at the end of the event. It adds the hits to the hits collection.
 *
 * @param[in,out] HCE Pointer to the hits collection of the event.
 */
void MySensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE)
{
    static G4int HCID = -1;
    if (HCID < 0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, hitsCollection);
}

/**
 * @todo SERVE?
 */
void MySensitiveDetector::InitializeFirstHit()
{
    firstHit = G4ThreeVector();
}