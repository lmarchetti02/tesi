#pragma once

// #include "G4VModularPhysicsList.hh"
// #include "G4EmStandardPhysics_option4.hh"

// class MyPhysicsList : public G4VModularPhysicsList
// {
// public:
//     MyPhysicsList();
//     ~MyPhysicsList();
// };

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"

// Modular physics used in this application
class MyPhysicsList : public G4VModularPhysicsList
{
public:
    explicit MyPhysicsList();
    ~MyPhysicsList();

    void ConstructParticle() override;
    void ConstructProcess() override;

private:
    G4VPhysicsConstructor *fEmPhysicsList;
    G4VPhysicsConstructor *fDecPhysicsList;
    G4VPhysicsConstructor *fRadDecayPhysicsList;
};