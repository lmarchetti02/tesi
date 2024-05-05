#include "generator.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "G4Event.hh"

#include "detector_construction.hh"
#include "constants.hh"
#include "energy_spectrum.hh"

/**
 * Constructor.
 *
 * Initializes the particle gun (particle type, initial position and momentum).
 */
PrimaryGenerator::PrimaryGenerator() : beamWidth(BEAM_WIDTH), monoEnergy(100 * keV), energyDistType(SPECTRUM_SHAPE)
{
    particleGun = new G4ParticleGun(1); // 1 particles per event

    // fetch particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    // set position and momentum vectors
    G4ThreeVector pos(0., 0., -Z_WORLD);

    // generate particle
    particleGun->SetParticlePosition(pos);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    particleGun->SetParticleEnergy(monoEnergy);
    particleGun->SetParticleDefinition(particle);

    // messenger
    DefineCommands();

    spectrum::save_to_file(energyDistType, MAX_ENE);
    sampleEnergy = new SampleEnergy();
    sampleEnergy->ReadFile("spectrum.dat");
}

/**
 * The destructor.
 */
PrimaryGenerator::~PrimaryGenerator()
{
    delete particleGun;
}

/**
 * Geant4 function for generating the primary vertex.
 *
 * @param[in] anEvent The pointer to the event in which the particle has to be created.
 */
void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    if (beamWidth < 0)
    {
        G4cout << "PrimaryGenerator::GeneratePrimaries(G4event): Invalid 'beamWidth'" << G4endl;
        return;
    }

    // set position
    if (beamWidth > 1)
        particleGun->SetParticlePosition(G4ThreeVector(0, 0, -Z_WORLD));
    else
    {
        // 0 for central pixel, 1 for whole array (see constants.hh)
        G4double max = (beamWidth == 0) ? XY_PIXEL : XY_WORLD;
        G4ThreeVector positionVector = randomPositionVector(max, max);
        particleGun->SetParticlePosition(positionVector);
    }

    // set energy
    energy = (!energyDistType) ? monoEnergy : sampleEnergy->Sample();
    particleGun->SetParticleEnergy(energy);

    // generate primary vertex
    particleGun->GeneratePrimaryVertex(anEvent);
}

/**
 * Function for generating a random unit vector inside a cone.
 *
 * @param[in] maxTheta The angle that defines the cone.
 * @return A random unit vector inside the cone defined by `maxTheta`.
 */
G4ThreeVector PrimaryGenerator::randomPositionVector(G4double xMax, G4double yMax)
{
    G4double x = -xMax + G4UniformRand() * 2 * xMax;
    G4double y = -yMax + G4UniformRand() * 2 * yMax;

    return G4ThreeVector(x, y, -Z_WORLD);
}

/**
 * Function for defining the messenger commands.
 */
void PrimaryGenerator::DefineCommands()
{
    fMessenger = new G4GenericMessenger(this, "/beam/", "X ray beam customization");

    auto &momentumCmd = fMessenger->DeclarePropertyWithUnit("energy", "keV", monoEnergy, "Photon energy");
    momentumCmd.SetParameterName("energy", true);
    momentumCmd.SetRange("energy>=0.");
    momentumCmd.SetDefaultValue("50.");

    auto &beamWidthCmd = fMessenger->DeclareProperty("beam_width", beamWidth, "The type of illumination.");
    beamWidthCmd.SetParameterName("beam_width", true);
    beamWidthCmd.SetRange("beam_width>=0");
    beamWidthCmd.SetDefaultValue("0");

    auto &beamTypeCmd = fMessenger->DeclareProperty("beam_type", energyDistType, "The type of energy spectrum.");
    beamTypeCmd.SetParameterName("beam_type", true);
    beamTypeCmd.SetRange("beam_type>=0");
    beamTypeCmd.SetDefaultValue("0");
}
