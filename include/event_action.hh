#pragma once

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "run_action.hh"
#include "generator.hh"
#include "save_step_info.hh"
#include "save_compton.hh"

/**
 * Event Action Class
 * ---
 *
 * Defines all the operations to perform throughout each
 * event of the simulation. In particular, it is used to get the
 * data from the HitsCollection, operate on it, and save the results
 * in the NTuple.
 */
class EventAction : public G4UserEventAction
{
private:
  // HC index
  G4int indexPixelsHC;
  G4int indexPlanesHC;

  // hits
  std::vector<G4double> energyVector;
  std::vector<G4double> energyVectorMerge;
  std::vector<G4double> energyVectorMergeCS;
  std::vector<G4int> fluorescenceIDVector;
  std::vector<G4double> fluorescenceEnergyVector;

  // refactoring
  G4bool escapedPhoton;

  // number of events
  static G4int nEvents;

  RunAction *myRun;
  PrimaryGenerator *myGenerator;

  G4int eventID;
  SaveStepInfo *stepInfo;
  SaveCompton *saveCompton;

  const G4double minEnergy = 0.1 * keV;

  G4int photonID;
  G4double photonEnergy;
  G4int initialPhotonSet;
  G4int initialPhotonNoIter;

  void ReadHitsCollection(const G4Event *);
  void ReadHitsCollectionPlanes(const G4Event *);
  void SaveEnergies();

  void MergePixels(std::vector<G4double>, std::vector<G4double> &);

  template <typename T>
  bool IsVectorEmpty(std::vector<T>);

public:
  EventAction(RunAction *, PrimaryGenerator *);
  ~EventAction() override = default;

  void BeginOfEventAction(const G4Event *) override;
  void EndOfEventAction(const G4Event *) override;

  static void SetNEvents(G4int N) { nEvents = N; }
  static G4int GetNEvents() { return nEvents; }

  G4int GetEventID() { return eventID; }

  void AddStepInfo(G4int, G4int, G4int, G4int, G4double, G4double);
  void SetInitialPhoton(G4int, G4double);

  G4double GetEnergy() { return myGenerator->GetEnergy(); }

  void AddComptonInfo(G4int, G4int, G4int, G4int, G4double, G4double, G4String);
};
