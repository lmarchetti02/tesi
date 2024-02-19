#include "sensitive_detector.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

#include "constants.hh"

G4int PixelsSD::firstID = -1;

/**
 * The constructor.
 *
 * @param[in] name The name of the sensitive detector.
 */
PixelsSD::PixelsSD(G4String name) : G4VSensitiveDetector(name), HCID(-1), hitsCounter(0)
{
    // SensitiveDetectorName and collectionName are data members of G4VSensitiveDetector
    collectionName.insert("PixelsHC");
}

/**
 * Geant4 function for initializing the hits collection relative to the sensitive detector.
 *
 * @param[in,out] HCE Pointer to the hits collection of the event.
 */
void PixelsSD::Initialize(G4HCofThisEvent *HCE)
{
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
G4bool PixelsSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    const G4Track *track = aStep->GetTrack();
    const G4String &particleName = track->GetParticleDefinition()->GetParticleName();

    const G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();
    const G4int copyNo = touchable->GetCopyNumber();
    const G4double eDep = aStep->GetTotalEnergyDeposit();
    const G4VProcess *currentProcess = preStepPoint->GetProcessDefinedStep();

    if (currentProcess)
    {
        const G4String &processName = currentProcess->GetProcessName();
        if (processName == "compt" && particleName == "gamma")
        {
            G4cout << "\033[38:5:160m"
                   << "Compton scattering"
                   << "\033[0m" << G4endl;

            G4cout << "ID = " << ConvertPixelID(copyNo) << " Energy = " << eDep << G4endl;
        }
    }

    // get pixel ID of primary photon
    if (hitsCounter < 1)
    {
        firstID = ConvertPixelID(copyNo);
        hitsCounter++;
    }

    // save step in HC
    PixelsHit *hit = new PixelsHit();
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
void PixelsSD::EndOfEvent(G4HCofThisEvent *HCE)
{
    static G4int HCID = -1;
    if (HCID < 0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, hitsCollection);
}

G4int PixelsSD::ConvertPixelID(const G4int ID)
{
    const G4int i_s = ID / N_SUBPIXEL;
    const G4int j_s = ID - i_s * N_SUBPIXEL;
    const G4int i_m = i_s / PIXEL_RATIO;
    const G4int j_m = j_s / PIXEL_RATIO;
    const G4int ID_m = i_m * N_PIXEL + j_m;

    return ID_m;
}
