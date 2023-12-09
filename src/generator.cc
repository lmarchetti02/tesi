#include "generator.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "detector_construction.hh"

/**
 * Constructor.
 *
 * Initializes the particle gun (particle type, initial position and momentum).
 */
MyPrimaryGenerator::MyPrimaryGenerator() : momentum(50 * keV), maxTheta(0.)
{
    fParticleGun = new G4ParticleGun(1); // 1 particles per event

    /* By putting the gun info in the constructor,
    it can be overwritten by the macro file during
    execution. */

    // fetch particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    // set position and momentum vectors
    G4ThreeVector pos(0., 0., -MyDetectorConstruction::GetWorldDimensions()[2]);

    // generate particle
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleMomentum(momentum);
    fParticleGun->SetParticleDefinition(particle);

    // messenger
    DefineCommands();
}

/**
 * The destructor.
 */
MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

/**
 * Geant4 function for generating the primary vertex.
 */
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // G4ThreeVector momDir = randomDirVector(maxTheta);
    // fParticleGun->SetParticleMomentumDirection(momDir);
    // fParticleGun->SetParticleMomentum(momentum);

    // generate primary vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

/**
 * Function for generating a random unit vector inside a cone.
 *
 * @param[in] maxTheta The angle that defines the cone.
 * @return A random unit vector inside the cone defined by `maxTheta`.
 */
G4ThreeVector MyPrimaryGenerator::randomDirVector(G4double maxTheta)
{
    maxTheta = maxTheta * (M_PI / 180);
    G4double theta = 2 * maxTheta * G4UniformRand() - maxTheta;
    G4double phi = 2 * M_PI * G4UniformRand();
    G4double u = std::cos(theta);

    G4double x = std::sqrt(1 - u * u) * std::cos(phi);
    G4double y = std::sqrt(1 - u * u) * std::sin(phi);
    G4double z = u;

    return G4ThreeVector(x, y, z);
}

/**
 * Function for defining the messenger commands.
 */
void MyPrimaryGenerator::DefineCommands()
{
    fMessenger = new G4GenericMessenger(this, "/beam/", "X ray beam customization");

    auto &angleCmd = fMessenger->DeclareProperty("angle", maxTheta, "Maximum beam angle");
    angleCmd.SetParameterName("angle", true);
    angleCmd.SetRange("angle>=0.");
    angleCmd.SetDefaultValue("5.0");

    auto &momentumCmd = fMessenger->DeclarePropertyWithUnit("momentum", "keV", momentum, "Maximum beam angle");
    momentumCmd.SetParameterName("momentum", true);
    momentumCmd.SetRange("momentum>=0.");
    momentumCmd.SetDefaultValue("100.");
}
