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
MyPrimaryGenerator::MyPrimaryGenerator() : monoEnergy(100. * keV), beamWidth(BEAM_WIDTH), energyDistType(0)
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
    SetPositionDistribution();
    SetEnergyDistribution();

    // generate primary vertex
    particleGun->GeneratePrimaryVertex(anEvent);

    // get particle energy
    G4PrimaryVertex *primaryVertex = anEvent->GetPrimaryVertex(0);
    G4PrimaryParticle *primaryParticle = primaryVertex->GetPrimary(0);
    energy = primaryParticle->GetTotalEnergy() / keV;

    G4cout << "Photon energy = " << energy << " keV" << G4endl;
}

/**
 * Function for setting the position distribution
 * of the source of photons.
 */
void MyPrimaryGenerator::SetPositionDistribution()
{
    if (beamWidth < 0)
    {
        G4cout << "MyPrimaryGenerator::SetPositionDistribution(G4event): Invalid 'beamWidth'" << G4endl;
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
}

/**
 * Function for setting the energy distribution of
 * the source of photons.
 */
void MyPrimaryGenerator::SetEnergyDistribution()
{
    if (energyDistType < 0)
    {
        G4cout << "MyPrimaryGenerator::SetEnergyDistribution(): Invalid 'energyDistType'" << G4endl;
        return;
    }

    // bremsstrahlung distribution
    if (!energyDistType)
    {
        particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Brem");
        particleGun->GetCurrentSource()->GetEneDist()->SetEmin(0.02); // in MeV
        particleGun->GetCurrentSource()->GetEneDist()->SetEmax(0.1);  // in MeV
        particleGun->GetCurrentSource()->GetEneDist()->SetTemp(2e9);
    }
    // arbitrary distribution
    else if (energyDistType == 1)
    {
        particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Arb");
        particleGun->GetCurrentSource()->GetEneDist()->ArbEnergyHistoFile("../spectrum/spectrum.dat");
        particleGun->GetCurrentSource()->GetEneDist()->ArbInterpolate("Lin");
    }
    // monoenergetic beam
    else
    {
        particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(monoEnergy);
    }
}

/**
 * Function for defining the messenger commands.
 */
void MyPrimaryGenerator::DefineCommands()
{
    fMessenger = new G4GenericMessenger(this, "/beam/", "X ray beam customization");

    auto &energyCmd = fMessenger->DeclareProperty("type", energyDistType, "Type of energy distribution");
    energyCmd.SetParameterName("type", true);
    energyCmd.SetRange("type>=0");
    energyCmd.SetDefaultValue("0");

    auto &beamWidthCmd = fMessenger->DeclareProperty("beam_width", beamWidth, "Type of beam");
    beamWidthCmd.SetParameterName("beam_width", true);
    beamWidthCmd.SetRange("beam_width>=0");
    beamWidthCmd.SetDefaultValue("0");
}
