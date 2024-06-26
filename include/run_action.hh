#pragma once

#include "G4Run.hh"
#include "G4UserRunAction.hh"

#include <vector>

/**
 * Run Action class
 * ---
 *
 * Defines all the operations to perform throughout each
 * run of the simulation. In particular, it is used to save
 * the results of the simulation.
 */
class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run *) override;
    void EndOfRunAction(const G4Run *) override;

    void ClearVectors();
    void AddEntry(G4int, G4double);
    void AddEntryCS(G4int, G4double);
    void AddEntryMerge(G4int, G4double);
    void AddEntryMergeCS(G4int, G4double);
    void AddFluorescence(G4int, G4double, G4double);

    void ClearOriginal();
    void ClearCS();

  private:
    std::vector<G4int> pixelIDVector{};
    std::vector<G4double> pixelEnergyVector{};
    std::vector<G4int> pixelIDVectorCS{};
    std::vector<G4double> pixelEnergyVectorCS{};
    std::vector<G4int> pixelIDVectorMerge{};
    std::vector<G4double> pixelEnergyVectorMerge{};
    std::vector<G4int> pixelIDVectorMergeCS{};
    std::vector<G4double> pixelEnergyVectorMergeCS{};
    std::vector<G4int> pixelIDFluorescence{};
    std::vector<G4double> energyFluorescence{};
    std::vector<G4double> energyLossFluorescence{};
};
