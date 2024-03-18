#pragma once

#include "Randomize.hh"
#include "globals.hh"

class SampleEnergy
{
private:
  std::vector<G4double> x;
  std::vector<G4double> f;
  std::vector<G4double> F;

public:
  SampleEnergy(const char *);
  ~SampleEnergy();

  G4double Sample();
};
