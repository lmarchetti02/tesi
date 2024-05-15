// PROF. MONACO

#pragma once

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

struct StepInfo
{
    G4int particle_id;
    G4int volume_id;
    G4double energy;
    G4double eneLoss;
    G4int iter_found;
};

class SaveStepInfo
{
  public:
    SaveStepInfo() = default;

    ~SaveStepInfo() = default;

    void Clear();

    void AddStep(G4int, G4int, G4int, G4int, G4double, G4double);

    void PrintList(int);

    std::vector<G4int> GetIDchannelsVector();

    std::vector<G4double> GetEneVector();

    std::vector<G4double> GetEneLossVector();

  private:
    const G4double min_ene = 0.1 * keV;
    std::vector<StepInfo *> list_info;
};
