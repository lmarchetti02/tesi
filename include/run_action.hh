#pragma once

#include "G4UserRunAction.hh"
#include "G4Run.hh"

/**
 * Run Action class
 * ---
 *
 * Defines all the operations to perform throughout each
 * run of the simulation. In particular, it is used to save
 * the results of the simulation.
 */
class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction() {}

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);
};