#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GenericMessenger.hh"

/**
 * Primary Generator Class
 * ---
 *
 * Creates the particle gun and defines the properties
 * of the particles to be shot (position and momentum).
 */
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
private:
    G4ParticleGun *fParticleGun;

    G4double maxTheta;
    G4double momentum;

    G4GenericMessenger *fMessenger;

public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator() override;

    void GeneratePrimaries(G4Event *) override;

    G4ThreeVector randomDirVector(G4double);

    void DefineCommands();
};