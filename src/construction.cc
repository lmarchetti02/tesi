#include "construction.hh"
#include "detector.hh"

using std::cout;
using std::endl;

// number of pixels
G4int MyDetectorConstruction::nPixel = 0;

// half dimensions of pixels
G4double MyDetectorConstruction::xDet = 50 * um;
G4double MyDetectorConstruction::yDet = 50 * um;
G4double MyDetectorConstruction::zDet = 0.75 * mm;

// world dimensions
G4double MyDetectorConstruction::xWorld = 0;
G4double MyDetectorConstruction::yWorld = 0;
G4double MyDetectorConstruction::zWorld = 0;

MyDetectorConstruction::MyDetectorConstruction()
{
    //  define materials
    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction() {}

void MyDetectorConstruction::DefineMaterials()
{
    // manager of G4 materials database
    G4NistManager *nist = G4NistManager::Instance();

    // world material
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    // detector material
    detectorMat = nist->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // DEFINE WORLD VOLUME (BOUNDARIES)
    // ---------------------------------------------------------------------------------------------
    // size of volume
    solidWorld = new G4Box("solidWorld", // name
                           xWorld,       // half lenght (meters)
                           yWorld,       // half width (meters)
                           zWorld);      // half lenght (meters)

    // volume material
    logicWorld = new G4LogicalVolume(solidWorld,    // solid volume
                                     worldMat,      // material
                                     "logicWorld"); // name

    // places/creates volume
    physWorld = new G4PVPlacement(nullptr,                   // rotation (none)
                                  G4ThreeVector(0., 0., 0.), // position (center)
                                  logicWorld,                // logical
                                  "physWorld",               // name
                                  nullptr,                   // mother volume
                                  false,                     // bool operations
                                  0,                         // copy number
                                  true);                     // overlapping
    // ---------------------------------------------------------------------------------------------

    // DEFINE DETECTOR VOLUME
    // ---------------------------------------------------------------------------------------------
    solidDetector = new G4Box("solidDetector", xDet, yDet, zDet);

    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    // scoring volume
    fScoringVolume = logicDetector;

    // array of detectors
    for (G4int i = 0; i < nPixel; i++)
    {
        for (G4int j = 0; j < nPixel; j++)
        {
            G4int ID = i * nPixel + j;

            physDetector = new G4PVPlacement(nullptr,
                                             G4ThreeVector(-xWorld + (2 * j + 1) * xDet, -yWorld + (2 * i + 1) * yDet, zWorld - zDet),
                                             logicDetector,
                                             "physDetector",
                                             logicWorld,
                                             false,
                                             ID, // unique id's
                                             true);

            // initialize map with hits per pixel
        }
    }
    // ---------------------------------------------------------------------------------------------

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    // create detector
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();

    MySensitiveDetector *sensDet = new MySensitiveDetector("/SensitiveDetector");
    sdManager->AddNewDetector(sensDet);
    logicDetector->SetSensitiveDetector(sensDet);
}

vector<G4double> MyDetectorConstruction::GetWorldDimensions()
{
    vector<G4double> worldDim = {xWorld, yWorld, zWorld};

    return worldDim;
}

vector<G4double> MyDetectorConstruction::GetPixelDimensions()
{
    vector<G4double> detDim = {xDet, yDet, zDet};

    return detDim;
}

void MyDetectorConstruction::setVisualization()
{
    auto *uiManager = G4UImanager::GetUIpointer();

    // number of pixels
    uiManager->ApplyCommand("/vis/set/textColour white");
    uiManager->ApplyCommand("/vis/set/textLayout right");
    uiManager->ApplyCommand("/vis/scene/add/text2D 0.9 -0.8 18 ! ! Number of pixels: " + std::to_string(nPixel) + " x " + std::to_string(nPixel));

    // scale
    uiManager->ApplyCommand("/vis/scene/add/scale " + std::to_string(2 * zDet) + " mm z 1 0.75 0 manual " + std::to_string(xWorld) + " " + std::to_string(yWorld) + " " + std::to_string(zWorld - zDet) + " mm");
    // uiManager->ApplyCommand("/vis/scene/add/scale " + std::to_string(2 * xDet) + " um x 1 0.75 0 manual " + std::to_string(-xWorld + xDet) + " " + std::to_string(-yWorld) + " " + std::to_string(zWorld) + " mm");
    // uiManager->ApplyCommand("/vis/scene/add/scale " + std::to_string(2 * yDet) + " um y 1 0.75 0 manual " + std::to_string(-xWorld) + " " + std::to_string(-yWorld + yDet) + " " + std::to_string(zWorld) + " mm");
}

void MyDetectorConstruction::SetNPixel(G4int N)
{
    nPixel += N;

    // update world dim
    xWorld += N * xDet;
    yWorld += N * yDet;

    if (xWorld < zDet)
        zWorld += zDet * 2;
    else
        zWorld += xWorld * 2;
}