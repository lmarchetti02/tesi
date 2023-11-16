#include "physics.hh"

// MyPhysicsList::MyPhysicsList()
// {
//     RegisterPhysics(new G4EmStandardPhysics_option4());
// }

// MyPhysicsList::~MyPhysicsList() {}

#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4StepLimiter.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

MyPhysicsList::MyPhysicsList()
{
    SetVerboseLevel(1);

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250 * eV, 1 * GeV);
    SetDefaultCutValue(0.001 * mm);
    DumpCutValuesTable();

    // EM physics
    fEmPhysicsList = new G4EmLivermorePhysics();

    // Add Decay
    fDecPhysicsList = new G4DecayPhysics();
    fRadDecayPhysicsList = new G4RadioactiveDecayPhysics();
}

MyPhysicsList::~MyPhysicsList()
{
    delete fDecPhysicsList;
    delete fRadDecayPhysicsList;
    delete fEmPhysicsList;
}

void MyPhysicsList::ConstructParticle()
{
    fDecPhysicsList->ConstructParticle();
}

void MyPhysicsList::ConstructProcess()
{
    AddTransportation();
    fEmPhysicsList->ConstructProcess();

    // decay physics list
    fDecPhysicsList->ConstructProcess();
    fRadDecayPhysicsList->ConstructProcess();

    // Deexcitation
    // Both Fluorescence and Auger e- emission activated
    G4VAtomDeexcitation *de = new G4UAtomicDeexcitation();
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);
    de->SetFluo(true);
    de->SetAuger(true);
    de->SetPIXE(true);
    de->SetAugerCascade(true);

    // To model full Auger cascade include in the macro file
    // the following UI commands:
    // process/em/augerCascade true
    // process/em/deexcitationIgnoreCut true
}