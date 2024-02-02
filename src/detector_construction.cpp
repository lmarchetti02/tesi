#include "detector_construction.hh"

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"

#include "sensitive_detector.hh"
#include "constants.hh"

/**
 * IMPORTANT:
 * The detector geometry is the following:
 *
 * |--------|--|-------------------------|--|--------->
 * 0       plane1         pixel        plane2        z
 * |<----------------zWorld---------------->|
 */

// pixel map
auto MyDetectorConstruction::pixelMap = std::map<G4int, std::array<G4double, 2>>();

MyDetectorConstruction::MyDetectorConstruction() : nPixel(N_SUBPIXEL),
                                                   xWorld(XY_WORLD),
                                                   yWorld(XY_WORLD),
                                                   zWorld(Z_WORLD),
                                                   zPlanes(Z_PLANES),
                                                   xPixel(XY_SUBPIXEL),
                                                   yPixel(XY_SUBPIXEL),
                                                   zPixel(Z_PIXEL),
                                                   scoringVolume(nullptr),
                                                   scoringPlane(nullptr)
{
    DefineMaterials();
}

/**
 * Function for constructing the solid, logical and physical volumes of the world volume.
 */
void MyDetectorConstruction::ConstructWorld()
{
    // size of volume
    solidWorld = new G4Box("solidWorld", // name
                           xWorld,       // half length (meters)
                           yWorld,       // half width (meters)
                           zWorld);      // half length (meters)

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
}

/**
 * Function for constructing the solid, logical and physical volumes of the array of pixels.
 */
void MyDetectorConstruction::ConstructPixels()
{
    solidDetector = new G4Box("solidDetector", xPixel, yPixel, zPixel);

    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    const G4double zPosition = zWorld - 2 * zPlanes - zPixel;

    // array of detectors
    for (G4int i = 0; i < nPixel; i++)
    {
        for (G4int j = 0; j < nPixel; j++)
        {
            const G4int ID = i * nPixel + j;
            const G4double x = -xWorld + (2 * j + 1) * xPixel;
            const G4double y = -yWorld + (2 * i + 1) * yPixel;

            physDetector = new G4PVPlacement(nullptr,
                                             G4ThreeVector(x, y, zPosition),
                                             logicDetector,
                                             "physDetector",
                                             logicWorld,
                                             false,
                                             ID, // unique id's
                                             true);

            // add to pixel map
            std::array<G4double, 2> xy = {x, y};
            AddToPixelMap(std::make_pair(ID, xy));
        }
    }

    scoringVolume = logicDetector;
}

/**
 * Function for constructing the volume used to track the photons
 * escaping from the detector.
 */
void MyDetectorConstruction::ConstructPlanes()
{
    solidPlane = new G4Box("solidPlane", xWorld, yWorld, zPlanes);

    logicPlane = new G4LogicalVolume(solidPlane, worldMat, "logicPlane");

    const G4double zPosition1 = zWorld - 2 * zPlanes - 2 * zPixel - zPlanes;
    const G4double zPosition2 = zWorld - zPlanes;

    physPlane1 = new G4PVPlacement(nullptr,
                                   G4ThreeVector(0., 0., zPosition1),
                                   logicPlane,
                                   "physPlane",
                                   logicWorld,
                                   false,
                                   1,
                                   true);

    physPlane2 = new G4PVPlacement(nullptr,
                                   G4ThreeVector(0., 0., zPosition2),
                                   logicPlane,
                                   "physPlane",
                                   logicWorld,
                                   false,
                                   2,
                                   true);

    scoringPlane = logicPlane;
}

/**
 * Function for defining the material of the world and detector volumes.
 */
void MyDetectorConstruction::DefineMaterials()
{
    // manager of G4 materials database
    G4NistManager *nist = G4NistManager::Instance();

    // world material
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");

    // detector material
    detectorMat = nist->FindOrBuildMaterial(TGT_MATERIAL);
}

/**
 * Geant4 function for constructing the detector volume.
 *
 * @return The physical volume of the world volume containing the detector.
 */
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // generate volume
    ConstructWorld();
    ConstructPixels();
    ConstructPlanes();

    return physWorld;
}

/**
 * Geant4 function for making the array of pixel a sensitive detector.
 */
void MyDetectorConstruction::ConstructSDandField()
{
    // create detector
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();

    MySensitiveDetector *sensDet = new MySensitiveDetector("/Pixels");
    sdManager->AddNewDetector(sensDet);
    logicDetector->SetSensitiveDetector(sensDet);
}

/**
 * Function for modifying the visualization macro at runtime.
 *
 */
void MyDetectorConstruction::SetVisualization()
{
    auto *uiManager = G4UImanager::GetUIpointer();

    // number of pixels
    uiManager->ApplyCommand("/vis/set/textColour white");
    uiManager->ApplyCommand("/vis/set/textLayout right");
    uiManager->ApplyCommand("/vis/scene/add/text2D 0.9 -0.8 18 ! ! Number of pixels: " + std::to_string(N_SUBPIXEL) + " x " + std::to_string(N_SUBPIXEL));

    // scale
    uiManager->ApplyCommand("/vis/scene/add/scale " +
                            std::to_string(2 * Z_PIXEL) + " mm z 1 0.75 0 manual " +
                            std::to_string(XY_WORLD) + " " +
                            std::to_string(XY_WORLD) + " " +
                            std::to_string(Z_WORLD - 2 * Z_PLANES - Z_PIXEL) + " mm");
}

/**
 * Static function for adding a new element to `pixelMap`.
 *
 * @param[in] newElement Reference to the `std::pair` containing the ID of the pixel and the coordinates of its center.
 */
void MyDetectorConstruction::AddToPixelMap(std::pair<G4int, std::array<G4double, 2>> newElement)
{
    pixelMap.insert(newElement);
}
