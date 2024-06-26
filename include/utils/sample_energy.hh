#pragma once

#include "G4Types.hh"

#include <vector>

class SampleEnergy
{
  private:
    std::vector<std::vector<G4double>> x;
    std::vector<std::vector<G4double>> f;
    std::vector<std::vector<G4double>> F;

  public:
    SampleEnergy();
    ~SampleEnergy() = default;

    G4double Sample(G4int type);
};
