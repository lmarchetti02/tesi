#pragma once

#include <vector>

#include "G4UserRunAction.hh"
#include "G4Run.hh"

/**
 * Run Action class
 * ---
 *
 * Defines all the operations to perform throughout each
 * run of the simulation. In particular, it is used to save
 * the results of the simulation.
 */
class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction() override = default;

    void BeginOfRunAction(const G4Run *) override;
    void EndOfRunAction(const G4Run *) override;

    void ClearVectors();
    void AddEntry(G4int, G4double);
    void AddEntryCS(G4int, G4double);
    void AddEntryMerge(G4int, G4double);
    void AddEnergyEscape(G4double);

private:
    std::vector<G4int> pixelIDVector;
    std::vector<G4double> pixelEnergyVector;
    std::vector<G4int> pixelIDVectorCS;
    std::vector<G4double> pixelEnergyVectorCS;
    std::vector<G4int> pixelIDVectorMerge;
    std::vector<G4double> pixelEnergyVectorMerge;
    std::vector<G4double> energyEscape;
};