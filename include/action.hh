#pragma once

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "event.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization();
    virtual ~MyActionInitialization() {}

    virtual void Build() const;
    virtual void BuildForMaster() const;
};