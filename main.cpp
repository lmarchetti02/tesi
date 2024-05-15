#include <filesystem>

// Geant4 libraries
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

// my scripts
#include "action_initialization.hh"
#include "detector_construction.hh"
#include "energy_spectrum.hh"
#include "physics.hh"

int main(int argc, char **argv)
{
    std::filesystem::create_directory("../results/");
    std::filesystem::create_directory("../spectrums/");

    // SETUP
    // --------------------------------------------------------------------------------
    // if no argument, interactive mode
    G4UIExecutive *ui = nullptr;
    if (argc == 1)
        ui = new G4UIExecutive(argc, argv);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // save .dat files for energy spectrum
    spectrum::save_spectrum_files();

// for running simulation
#ifndef G4MULTITHREADED
    auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
#else
    auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
#endif

    runManager->SetUserInitialization(new DetectorConstruction); // detector
    runManager->SetUserInitialization(new PhysicsList);          // physics
    runManager->SetUserInitialization(new ActionInitialization); // particles

    // for visualization
    auto *visManager = new G4VisExecutive;
    visManager->Initialize();
    // --------------------------------------------------------------------------------

    // START PROGRAM
    // --------------------------------------------------------------------------------
    auto *uiManager = G4UImanager::GetUIpointer();

    /* If no interactive mode, just execute in
    the terminal. */
    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command + fileName);
    }
    /* If interactive mode, load macro file and
    start visualization. */
    else {
        uiManager->ApplyCommand("/control/execute vis.mac");
        DetectorConstruction::SetVisualization();
        ui->SessionStart();

        delete ui;
    }
    //   --------------------------------------------------------------------------------

    delete visManager;
    delete runManager;

    return 0;
}
