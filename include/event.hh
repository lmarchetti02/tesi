#pragma once

#include <map>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
private:
    // deposited energy
    G4double fEdep;
    G4int index;

public:
    MyEventAction(MyRunAction *);
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);
};
