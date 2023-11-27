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
    fstream outFile;
    outFile.open("../root/results/info.txt", std::ios::out);
    if (outFile.is_open())
    {
        outFile << "INFORMATION ABOUT SIMULATION" << endl;
        outFile << "----------------------------" << endl;
        outFile << "Number of pixels: " << nPixel << endl;
        outFile << "Pixel x-dimension: " << 2 * xDet << " mm" << endl;
        outFile << "Pixel y-dimension: " << 2 * yDet << " mm" << endl;
        outFile << "Pixel z-dimension: " << 2 * zDet << " mm" << endl;
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
    solidDetector = new G4Box("solidDetector", xDet, yDet, zDet);

    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

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
array<G4double, 3> MyDetectorConstruction::GetWorldDimensions()
{
    array<G4double, 3> worldDim = {xWorld, yWorld, zWorld};

    return worldDim;
}

/**
 * Static function for accessing the dimensions of the pixels.
 *
 * @return `std::array(xPixel, yPixel, zPixel)`
 */
array<G4double, 3> MyDetectorConstruction::GetPixelDimensions()
{
    array<G4double, 3> detDim = {xDet, yDet, zDet};

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
    uiManager->ApplyCommand("/vis/scene/add/scale " + std::to_string(2 * zDet) + " mm z 1 0.75 0 manual " + std::to_string(xWorld) + " " + std::to_string(yWorld) + " " + std::to_string(zWorld - zDet) + " mm");
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
    xWorld += N * xDet;
    yWorld += N * yDet;

    if (xWorld < zDet)
        zWorld += zDet * 2;
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
 * @todo CHECK IF THE PIXEL MAP IS USED SOMEWHERE
 */
void MyDetectorConstruction::PrintPixelMap()
{
    for (auto &itr : pixelMap)
    {
        G4cout << itr.first << " : ";
        G4cout << "(" << itr.second[0] << ", " << itr.second[1] << ")" << G4endl;
    }
}
void MyDetectorConstruction::AddToPixelMap(const pair<G4int, array<G4double, 2>> &p)
{
    pixelMap.insert(p);
}
map<G4int, array<G4double, 2>> MyDetectorConstruction::GetPixelMap()
{
    return pixelMap;
}
