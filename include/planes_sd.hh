#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"

#include "planes_hits.hh"

/**
 * Sensitive Detector Class
 * ---
 *
 * Defines the sensitive detector that is associated
 * to the array of pixels. It also manage the HitsCollection
 * where the hits are stored.
 */
class PlanesSD : public G4VSensitiveDetector
{
private:
    PlanesHitsCollection *hitsCollection;
    G4int HCID;

public:
    PlanesSD(G4String);
    ~PlanesSD() override = default;

    void Initialize(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    void EndOfEvent(G4HCofThisEvent *) override;
};
