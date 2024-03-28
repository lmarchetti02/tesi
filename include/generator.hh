#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GenericMessenger.hh"

#include "sample_energy.hh"

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

  G4int beamWidth;
  G4double monoEnergy;
  G4int energyDistType;

  G4double energy;

  G4GenericMessenger *fMessenger;

public:
  SampleEnergy *sampleEnergy;

  PrimaryGenerator();
  ~PrimaryGenerator() override;

  void GeneratePrimaries(G4Event *) override;

  void DefineCommands();

  G4ThreeVector randomPositionVector(G4double, G4double);

  G4int GetBeamWidth() { return beamWidth; }
  G4int GetBeamType() { return energyDistType; }
  G4double GetEnergy() { return energy; }
};
