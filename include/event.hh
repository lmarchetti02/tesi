#pragma once

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "run_action.hh"

class MyEventAction : public G4UserEventAction
{
private:
    // HC index
    G4int index;

    // hits map
    std::vector<G4double> energyVector;

public:
    MyEventAction(MyRunAction *);
    virtual ~MyEventAction() {}

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    template <typename T>
    static T VectorSum(const std::vector<T> &);
};
