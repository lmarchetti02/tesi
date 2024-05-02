#pragma once

#include "globals.hh"

class SampleEnergy
{
private:
  std::vector<G4double> x;
  std::vector<G4double> f;
  std::vector<G4double> F;

public:
  SampleEnergy() = default;
  ~SampleEnergy();

  void ReadFile(const char *fileName);
  G4double Sample();
};
