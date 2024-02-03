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
class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
private:
  G4ParticleGun *particleGun;

  static G4double energy;
  G4int beamWidth;

  G4GenericMessenger *fMessenger;

public:
  PrimaryGenerator();
  ~PrimaryGenerator() override;

  void GeneratePrimaries(G4Event *) override;
  G4ThreeVector randomPositionVector(G4double, G4double);

  void DefineCommands();

  G4int GetBeamWidth() { return beamWidth; }
  static G4double GetEnergy() { return energy; }
};
