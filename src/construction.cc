#include "construction.hh"
#include "detector.hh"

using std::cout;
using std::endl;

// number of pixels
G4int MyDetectorConstruction::nPixel = 0;

// half dimensions of pixels
G4double MyDetectorConstruction::xDet = 25 * um;
G4double MyDetectorConstruction::yDet = 25 * um;
G4double MyDetectorConstruction::zDet = 0.5 * mm;

// world dimensions
G4double MyDetectorConstruction::xWorld = 0;
G4double MyDetectorConstruction::yWorld = 0;
G4double MyDetectorConstruction::zWorld = 0;

// pixel map
auto MyDetectorConstruction::pixelMap = map<G4int, array<G4double, 2>>();

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
            G4double x = -xWorld + (2 * j + 1) * xDet;
            G4double y = -yWorld + (2 * i + 1) * yDet;

            physDetector = new G4PVPlacement(nullptr,
                                             G4ThreeVector(x, y, zWorld - zDet),
                                             logicDetector,
                                             "physDetector",
                                             logicWorld,
                                             false,
                                             ID, // unique id's
                                             true);

            // add to pixel map
            array<G4double, 2> xy = {x, y};
            pair<G4int, array<G4double, 2>> p(ID, xy);
            AddToPixelMap(p);
        }
    }
    PrintPixelMap();
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

array<G4double, 3> MyDetectorConstruction::GetWorldDimensions()
{
    array<G4double, 3> worldDim = {xWorld, yWorld, zWorld};

    return worldDim;
}

array<G4double, 3> MyDetectorConstruction::GetPixelDimensions()
{
    array<G4double, 3> detDim = {xDet, yDet, zDet};

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

void MyDetectorConstruction::PrintPixelMap()
{
    for (auto &itr : pixelMap)
    {
        G4cout << itr.first << " : ";
        G4cout << "(" << itr.second[0] << ", " << itr.second[1] << ")" << G4endl;
    }
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