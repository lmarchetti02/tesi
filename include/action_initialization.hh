#pragma once

#include "G4VUserActionInitialization.hh"

/**
 * Action Initialization Class
 * ---
 *
 * Defines how to interact with the simulation, by initializing
 * the particle gun, the run action and the event action.
 */
class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ~ActionInitialization() override = default;

    void Build() const override;
    void BuildForMaster() const override;
};
