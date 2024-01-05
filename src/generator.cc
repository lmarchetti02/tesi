#include "generator.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "detector_construction.hh"
#include "constants.hh"

/**
 * Constructor.
 *
 * Initializes the particle gun (particle type, initial position and momentum).
 */
MyPrimaryGenerator::MyPrimaryGenerator() : energy(50 * keV), maxTheta(0.), beamWidth(BEAM_WIDTH)
{
    fParticleGun = new G4ParticleGun(1); // 1 particles per event

    // fetch particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    // set position and momentum vectors
    G4ThreeVector pos(0., 0., -MyDetectorConstruction::GetWorldDimensions()[2]);

    // generate particle
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(energy);
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
 *
 * @param[in] anEvent The pointer to the event in which the particle has to be created.
 */
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // all the photons in the center of the array
    if (beamWidth > 1)
    {
        fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -MyDetectorConstruction::GetWorldDimensions()[2]));
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }

    G4double max;
    // all the photons in the central pixel
    if (beamWidth == 0)
        max = MyDetectorConstruction::GetWorldDimensions()[0];
    // photons distributed uniformly across the array
    else if (beamWidth == 1)
        max = MyDetectorConstruction::GetPixelDimensions()[0] * PIXEL_RATIO;

    G4ThreeVector positionVector = randomPositionVector(max, max);
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(positionVector);

    // generate primary vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

/**
 * Function for generating a random unit vector inside a cone.
 *
 * @param[in] maxTheta The angle that defines the cone.
 * @return A random unit vector inside the cone defined by `maxTheta`.
 */
G4ThreeVector MyPrimaryGenerator::randomPositionVector(G4double xMax, G4double yMax)
{
    G4double x = -xMax + G4UniformRand() * 2 * xMax;
    G4double y = -yMax + G4UniformRand() * 2 * yMax;

    return G4ThreeVector(x, y, -MyDetectorConstruction::GetWorldDimensions()[2]);
}

/**
 * Function for defining the messenger commands.
 */
void MyPrimaryGenerator::DefineCommands()
{
    fMessenger = new G4GenericMessenger(this, "/beam/", "X ray beam customization");

    auto &momentumCmd = fMessenger->DeclarePropertyWithUnit("energy", "keV", energy, "Photon energy");
    momentumCmd.SetParameterName("energy", true);
    momentumCmd.SetRange("energy>=0.");
    momentumCmd.SetDefaultValue("50.");

    auto &beamWidthCmd = fMessenger->DeclareProperty("beam_width", beamWidth, "Type of beam");
    beamWidthCmd.SetParameterName("beam_width", true);
    beamWidthCmd.SetRange("beam_width>=0");
    beamWidthCmd.SetDefaultValue("0");
}
