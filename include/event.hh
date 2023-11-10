#pragma once

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "run.hh"
#include "construction.hh"

using std::map;
using std::vector;

class MyEventAction : public G4UserEventAction
{
private:
    // HC index
    G4int index;

    // hits map
    vector<G4double> energyVector;

public:
    MyEventAction(MyRunAction *);
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);
};
