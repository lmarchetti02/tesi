// PROF. MONACO

#pragma once

#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

struct ComptonInfo
{
    G4int particle_id;
    G4int parent_id;
    G4int part_type;       // 0=photon 1=others
    G4int volume_id;       // volume id primary interaction
    G4int volume_id_iter;  // volume id secondary interaction (if < 1 it is escaped - to be confirmed)
    G4double energy;       // energy of the scattered photon
    G4double eneLoss;      // energy loss in the primary interaction
    G4double eneLoss_iter; // energy loss in the secondary interaction
    G4int iter_found;      // n.steps with energy loss in the primary interaction
    G4int iter_found_iter; // n.steps with energy loss in the secondary interaction
    G4double escaped_ene;
};

class SaveCompton
{
  public:
    SaveCompton() = default;

    ~SaveCompton() = default;

    void Clear();

    void AddStep(G4int, G4int, G4int, G4int, G4double, G4double, G4int, G4String = "");

    int get_n() { return list_info.size(); }

    G4int PrintList(int evid);

    G4double GetComptonEnergy();

    G4int GetComptonID();

    G4double GetComptonEloss();

    G4int GetComptonIDiter();

    G4double GetComptonElossiter();

  private:
    const G4double min_ene = 0.1 * keV;
    std::vector<ComptonInfo *> list_info;
};
