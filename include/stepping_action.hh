#pragma once

#include "G4LogicalVolume.hh"
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "event_action.hh"

/**
 * Stepping Action Class
 * ---
 *
 * It is used for analyzing the steps generated
 * during each event of the simulation.
 */
class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction *);
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step *) override;

private:
    EventAction *eventAction;
    G4LogicalVolume *scoringVolume, *scoringPlane;
};
