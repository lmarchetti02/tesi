#include <iostream>
#include <memory>

// Geant4 libraries
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4RunManagerFactory.hh"

// my scripts
#include "construction.hh"
#include "physics.hh"
#include "action_initialization.hh"
#include "sensitive_detector.hh"

int main(int argc, char **argv)
{
	// SETUP
	// --------------------------------------------------------------------------------
	// if no argument, interactive mode
	G4UIExecutive *ui = nullptr;
	if (argc == 1)
		ui = new G4UIExecutive(argc, argv);

// for running simulation
#ifndef G4MULTITHREADED
	auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
#else
	auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
#endif

	runManager->SetUserInitialization(new MyDetectorConstruction); // detector
	runManager->SetUserInitialization(new MyPhysicsList);		   // physics
	runManager->SetUserInitialization(new MyActionInitialization); // particles

	// for visualization
	auto *visManager = new G4VisExecutive;
	visManager->Initialize();
	// --------------------------------------------------------------------------------

	// START PROGRAM
	// --------------------------------------------------------------------------------
	// set number of pixels
	G4int N;
	while (N <= 0)
	{
		G4cout << "Number of pixels:" << G4endl;
		G4cin >> N;
	}
	MyDetectorConstruction::SetNPixel(N);

	auto *uiManager = G4UImanager::GetUIpointer();

	/* If no interactive mode, just execute in
	the terminal. */
	if (!ui)
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		uiManager->ApplyCommand(command + fileName);
	}
	/* If interactive mode, load macro file and
	start visualization. */
	else
	{
		uiManager->ApplyCommand("/control/execute vis.mac");
		MyDetectorConstruction::setVisualization();
		ui->SessionStart();

		delete ui;
	}
	//   --------------------------------------------------------------------------------

	delete visManager;
	delete runManager;

	return 0;
}
