#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GenericMessenger.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
private:
    G4ParticleGun *fParticleGun;

    G4double maxTheta;
    G4double momentum;

    G4GenericMessenger *fMessenger;

public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

    G4ThreeVector randomDirVector(G4double);

    void DefineCommands();
};