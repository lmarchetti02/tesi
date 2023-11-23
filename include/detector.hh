#pragma once

#include <fstream>

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "hits.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
private:
    MyHitsCollection *hitsCollection;
    G4int HCID = -1;

    G4int ProcessHitsCounter;
    static G4ThreeVector firstHit;
    static G4int pixelFirstHit;

public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();

    virtual void Initialize(G4HCofThisEvent *);
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void EndOfEvent(G4HCofThisEvent *);

    static void InitializeFirstHit() { firstHit = G4ThreeVector(); }
    static void SetFirstHit(const G4ThreeVector &pos) { firstHit = pos; }
    static G4ThreeVector GetFirstHit() { return firstHit; }

    static void SetPixelFirstHit(G4int ID) { pixelFirstHit = ID; }
    static G4int GetPixelFirstHit() { return pixelFirstHit; }
};
