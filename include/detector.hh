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

public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();

    virtual void Initialize(G4HCofThisEvent *);
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void EndOfEvent(G4HCofThisEvent *);
};
