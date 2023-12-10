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
    void AddEntry(G4int ID, G4double Energy);
    void AddEntryCS(G4int ID, G4double Energy);
    G4int GetIDVectorSize() const { return id_pixel_nocs.size(); }
    G4int GetIDVectorSizeCS() const { return id_pixel_cs.size(); }

private:
    std::vector<int> id_pixel_nocs;
    std::vector<double> ene_pixel_nocs;
    std::vector<int> id_pixel_cs;
    std::vector<double> ene_pixel_cs;
};