#include "planes_sd.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

/**
 * The constructor.
 *
 * @param[in] name The name of the sensitive detector.
 */
PlanesSD::PlanesSD(G4String name) : G4VSensitiveDetector(name), HCID(-1)
{
    // SensitiveDetectorName and collectionName are data members of G4VSensitiveDetector
    collectionName.insert("PlanesHC");
}

/**
 * Geant4 function for initializing the hits collection relative to the sensitive detector.
 *
 * @param[in,out] HCE Pointer to the hits collection of the event.
 */
void PlanesSD::Initialize(G4HCofThisEvent *HCE)
{
    hitsCollection = new PlanesHitsCollection(SensitiveDetectorName, collectionName[0]);

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
G4bool PlanesSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // get the id of the detector that interacted w/ photon
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // energy deposition of the step
    G4double eDep = aStep->GetTotalEnergyDeposit();

    // save step in HC
    PlanesHits *hit = new PlanesHits();
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
void PlanesSD::EndOfEvent(G4HCofThisEvent *HCE)
{
    static G4int HCID = -1;
    if (HCID < 0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, hitsCollection);
}
