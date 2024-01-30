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
  G4ParticleGun *particleGun;

  G4double energy;
  G4int beamWidth;

  G4GenericMessenger *fMessenger;

public:
  MyPrimaryGenerator();
  ~MyPrimaryGenerator() override;

  void GeneratePrimaries(G4Event *) override;
  G4ThreeVector randomPositionVector(G4double, G4double);

  void DefineCommands();

  G4int GetBeamWidth() { return beamWidth; }
  G4double GetEnergy() { return energy; }
};
