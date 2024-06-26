#pragma once

#include "G4TouchableHistory.hh"
#include "G4VSensitiveDetector.hh"
#include "hits.hh"

/**
 * Sensitive Detector Class
 * ---
 *
 * Defines the sensitive detector that is associated
 * to the array of pixels. It also manage the HitsCollection
 * where the hits are stored.
 */
class PixelsSD : public G4VSensitiveDetector
{
  private:
    MyHitsCollection *hitsCollection;
    G4int HCID;

  public:
    PixelsSD(G4String);
    ~PixelsSD() override = default;

    void Initialize(G4HCofThisEvent *) override;
    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    void EndOfEvent(G4HCofThisEvent *) override;
};
