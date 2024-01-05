#pragma once

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "run_action.hh"

/**
 * Event Action Class
 * ---
 *
 * Defines all the operations to perform throughout each
 * event of the simulation. In particular, it is used to get the
 * data from the HitsCollection, operate on it, and save the results
 * in the NTuple.
 */
class MyEventAction : public G4UserEventAction
{
private:
    // HC index
    G4int index;

    // hits
    std::vector<G4double> energyVector;
    std::vector<G4double> energyVectorMerge;
    std::vector<G4double> escapeVector;

    // number of events
    static G4int nEvents;

    // run action
    MyRunAction *myRun;

public:
    MyEventAction(MyRunAction *);
    ~MyEventAction() override = default;

    void BeginOfEventAction(const G4Event *) override;
    void EndOfEventAction(const G4Event *) override;

    void readHitsCollection(const G4Event *);

    template <typename T>
    static T VectorSum(std::vector<T>);

    template <typename T>
    static bool IsVectorEmpty(std::vector<T>);

    static void SetNEvents(G4int N) { nEvents = N; }
    static G4int GetNEvents() { return nEvents; }

    void AddEnergyEscape(G4double energy) { escapeVector.push_back(energy); }
    void MergePixels();
};
