#pragma once

#include "G4VModularPhysicsList.hh"

/**
 * Physics List Class
 * ---
 *
 * Defines all the physics necessary for the simulation. In particular,
 * it defines al the particles and physical processes needed.
 */
class PhysicsList : public G4VModularPhysicsList
{
  public:
    explicit PhysicsList();
    ~PhysicsList() override;

    void ConstructParticle() override;
    void ConstructProcess() override;

  private:
    G4VPhysicsConstructor *fEmPhysicsList;
    G4VPhysicsConstructor *fDecPhysicsList;
    G4VPhysicsConstructor *fRadDecayPhysicsList;
};
