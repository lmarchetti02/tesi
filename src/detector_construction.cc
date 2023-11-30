#include "detector_construction.hh"
#include "sensitive_detector.hh"

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

#include <fstream>

using std::cout;
using std::endl;

// number of pixels
G4int MyDetectorConstruction::nPixel = 0;

// half dimensions of pixels
G4double MyDetectorConstruction::xPixel = 25 * um;
G4double MyDetectorConstruction::yPixel = 25 * um;
G4double MyDetectorConstruction::zPixel = 0.5 * mm;

// world dimensions
G4double MyDetectorConstruction::xWorld = 0;
G4double MyDetectorConstruction::yWorld = 0;
G4double MyDetectorConstruction::zWorld = 0;

// pixel map
auto MyDetectorConstruction::pixelMap = std::map<G4int, std::array<G4double, 2>>();

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineMaterials();
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
    detectorMat = nist->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
}

/**
 * Geant4 function for constructing the detector volume.
 *
 * @return The physical volume of the world volume containing the detector.
 */
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // construct volumes
    ConstructWorld();
    ConstructPixels();

    // save info about the detector in text file
    std::fstream outFile;
    outFile.open("../root/results/info.txt", std::ios::out);
    if (outFile.is_open())
    {
        outFile << "INFORMATION ABOUT SIMULATION" << endl;
        outFile << "----------------------------" << endl;
        outFile << "Number of pixels: " << nPixel << endl;
        outFile << "Pixel x-dimension (mm): " << 2 * xPixel << endl;
        outFile << "Pixel y-dimension (mm): " << 2 * yPixel << endl;
        outFile << "Pixel z-dimension (mm): " << 2 * zPixel << endl;
    }
    outFile.close();

    return physWorld;
}

/**
 * Function for constructing the solid, logical and physical volumes of the world volume.
 */
void MyDetectorConstruction::ConstructWorld()
{
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
}

/**
 * Function for constructing the solid, logical and physical volumes of the array of pixels.
 */
void MyDetectorConstruction::ConstructPixels()
{
    solidDetector = new G4Box("solidDetector", xPixel, yPixel, zPixel);

    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    // array of detectors
    for (G4int i = 0; i < nPixel; i++)
    {
        for (G4int j = 0; j < nPixel; j++)
        {
            G4int ID = i * nPixel + j;
            G4double x = -xWorld + (2 * j + 1) * xPixel;
            G4double y = -yWorld + (2 * i + 1) * yPixel;

            physDetector = new G4PVPlacement(nullptr,
                                             G4ThreeVector(x, y, zWorld - zPixel),
                                             logicDetector,
                                             "physDetector",
                                             logicWorld,
                                             false,
                                             ID, // unique id's
                                             true);

            // add to pixel map
            std::array<G4double, 2> xy = {x, y};
            std::pair<G4int, std::array<G4double, 2>> p(ID, xy);
            AddToPixelMap(p);
        }
    }
    // ---------------------------------------------------------------------------------------------
}

/**
 * Geant4 function for making the array of pixel a sensitive detector.
 */
void MyDetectorConstruction::ConstructSDandField()
{
    // create detector
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();

    MySensitiveDetector *sensDet = new MySensitiveDetector("/SensitiveDetector");
    sdManager->AddNewDetector(sensDet);
    logicDetector->SetSensitiveDetector(sensDet);
}

/**
 * Static function for accessing the dimensions of the world volume.
 *
 * @return `std::array(xWorld, yWorld, zWorld)`
 */
std::array<G4double, 3> MyDetectorConstruction::GetWorldDimensions()
{
    std::array<G4double, 3> worldDim = {xWorld, yWorld, zWorld};

    return worldDim;
}

/**
 * Static function for accessing the dimensions of the pixels.
 *
 * @return `std::array(xPixel, yPixel, zPixel)`
 */
std::array<G4double, 3> MyDetectorConstruction::GetPixelDimensions()
{
    std::array<G4double, 3> detDim = {xPixel, yPixel, zPixel};

    return detDim;
}

/**
 * Function for modifying the visualization macro at runtime.
 *
 */
void MyDetectorConstruction::setVisualization()
{
    auto *uiManager = G4UImanager::GetUIpointer();

    // number of pixels
    uiManager->ApplyCommand("/vis/set/textColour white");
    uiManager->ApplyCommand("/vis/set/textLayout right");
    uiManager->ApplyCommand("/vis/scene/add/text2D 0.9 -0.8 18 ! ! Number of pixels: " + std::to_string(nPixel) + " x " + std::to_string(nPixel));

    // scale
    uiManager->ApplyCommand("/vis/scene/add/scale " + std::to_string(2 * zPixel) + " mm z 1 0.75 0 manual " + std::to_string(xWorld) + " " + std::to_string(yWorld) + " " + std::to_string(zWorld - zPixel) + " mm");
}

/**
 * Function for setting the number of pixels in the array at runtime.
 *
 * It also modifies the world dimensions according to the number of pixels.
 *
 * @param[in] N The number of pixel along x and y, so the square root of the total number of pixels.
 */
void MyDetectorConstruction::SetNPixel(G4int N)
{
    nPixel += N;

    // update world dim
    xWorld += N * xPixel;
    yWorld += N * yPixel;

    if (xWorld < zPixel)
        zWorld += zPixel * 2;
    else
        zWorld += xWorld * 2;
}

/**
 * Static function for accessing the number of pixels in the array.
 *
 * @return The number of pixel along x and y, so the square root of the total number of pixels.
 */
G4int MyDetectorConstruction::GetNPixel()
{
    return nPixel;
}

/**
 * Static function for printing `pixelMap`.
 */
void MyDetectorConstruction::PrintPixelMap()
{
    for (auto &itr : pixelMap)
    {
        G4cout << itr.first << " : ";
        G4cout << "(" << itr.second[0] << ", " << itr.second[1] << ")" << G4endl;
    }
}

/**
 * Static function for adding a new element to `pixelMap`.
 *
 * @param[in] newElement Reference to the `std::pair` containing the ID of the pixel and the coordinates of its center.
 */
void MyDetectorConstruction::AddToPixelMap(const std::pair<G4int, std::array<G4double, 2>> &newElement)
{
    pixelMap.insert(newElement);
}

/**
 * Static function for accessing `pixelMap`.
 *
 * @return The static variable `pixelMap`, which contains {ID : (xCenter, yCenter)} of each pixel.
 */
std::map<G4int, std::array<G4double, 2>> MyDetectorConstruction::GetPixelMap()
{
    return pixelMap;
}
