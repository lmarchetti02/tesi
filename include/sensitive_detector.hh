#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"

#include "hits.hh"

/**
 * Sensitive Detector Class
 * ---
 *
 * Defines the sensitive detector that is associated
 * to the array of pixels. It also manage the HitsCollection
 * where the hits are stored.
 */
class MySensitiveDetector : public G4VSensitiveDetector
{
private:
    MyHitsCollection *hitsCollection;
    G4int HCID;

    G4int ProcessHitsCounter;
    static G4ThreeVector firstHit;
    static G4int pixelFirstHit;

public:
    MySensitiveDetector(G4String);
    virtual ~MySensitiveDetector() {}

    virtual void Initialize(G4HCofThisEvent *);
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void EndOfEvent(G4HCofThisEvent *);

    //! @todo SERVE??
    static void InitializeFirstHit();
    static void SetFirstHit(const G4ThreeVector &pos) { firstHit = pos; }
    static G4ThreeVector GetFirstHit() { return firstHit; }

    static void SetPixelFirstHit(G4int ID) { pixelFirstHit = ID; }
    static G4int GetPixelFirstHit() { return pixelFirstHit; }
};
