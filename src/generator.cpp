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
MyPrimaryGenerator::MyPrimaryGenerator() : energy(50 * keV), beamWidth(BEAM_WIDTH)
{
    particleGun = new G4GeneralParticleSource();

    // fetch particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    // set particle type
    particleGun->SetParticleDefinition(particle);

    // messenger
    DefineCommands();
}

/**
 * The destructor.
 */
MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete particleGun;
}

/**
 * Geant4 function for generating the primary vertex.
 *
 * @param[in] anEvent The pointer to the event in which the particle has to be created.
 */
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    if (beamWidth < 0)
    {
        G4cout << "MyPrimaryGenerator::GeneratePrimaries(G4event): Invalid 'beamWidth'" << G4endl;
        return;
    }

    G4ThreeVector centre{0, 0, -Z_WORLD};

    // set position
    if (beamWidth > 1)
    {
        particleGun->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(centre);
    }
    else
    {
        // 0 for central pixel, 1 for whole array (see constants.hh)
        G4double max = (beamWidth == 0) ? XY_PIXEL : XY_WORLD;

        particleGun->GetCurrentSource()->GetPosDist()->SetPosDisType("Plane");
        particleGun->GetCurrentSource()->GetPosDist()->SetPosDisShape("Rectangle");
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(centre);
        particleGun->GetCurrentSource()->GetPosDist()->SetHalfX(max);
        particleGun->GetCurrentSource()->GetPosDist()->SetHalfY(max);
    }

    // set energy
    particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Brem");
    particleGun->GetCurrentSource()->GetEneDist()->SetEmin(0.02); // in MeV
    particleGun->GetCurrentSource()->GetEneDist()->SetEmax(0.1);  // in MeV
    particleGun->GetCurrentSource()->GetEneDist()->SetTemp(2e9);

    // generate primary vertex
    particleGun->GeneratePrimaryVertex(anEvent);

    // get particle energy
    G4PrimaryVertex *primaryVertex = anEvent->GetPrimaryVertex(0);
    G4PrimaryParticle *primaryParticle = primaryVertex->GetPrimary(0);
    energy = primaryParticle->GetTotalEnergy() / keV;
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

    return G4ThreeVector(x, y, -Z_WORLD);
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
