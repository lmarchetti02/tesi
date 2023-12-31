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

    static G4ThreeVector firstHit;
    static G4int pixelFirstHit;

public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector() override = default;

    void Initialize(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    void EndOfEvent(G4HCofThisEvent *) override;
};
