#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
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
  G4GeneralParticleSource *particleGun;

  G4double energy;
  G4int beamWidth;

  G4double monoEnergy;
  G4int energyDistType;

  G4GenericMessenger *fMessenger;

public:
  PrimaryGenerator();
  ~PrimaryGenerator() override;

  void GeneratePrimaries(G4Event *) override;

  void SetPositionDistribution();
  void SetEnergyDistribution();

  void DefineCommands();

  G4int GetBeamWidth() { return beamWidth; }
  G4double GetEnergy() { return energy; }
};
