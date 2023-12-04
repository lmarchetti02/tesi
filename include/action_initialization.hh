#pragma once

#include "G4VUserActionInitialization.hh"

/**
 * Action Initialization Class
 * ---
 *
 * Defines how to interact with the simulation, by initializing
 * the particle gun, the run action and the event action.
 */
class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization();
    ~MyActionInitialization() override = default;

    void Build() const override;
    void BuildForMaster() const override;
};