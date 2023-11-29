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
    virtual ~MyActionInitialization() {}

    virtual void Build() const;
    virtual void BuildForMaster() const;
};