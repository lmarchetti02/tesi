#pragma once

#include "G4LogicalVolume.hh"
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "event_action.hh"

/**
 * Stepping Action Class
 * ---
 *
 *
 */
class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(MyEventAction *);
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step *) override;

private:
    MyEventAction *eventAction;
    G4LogicalVolume *scoringVolume, *scoringPlane;
};