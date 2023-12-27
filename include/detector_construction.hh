#pragma once

#include <array>
#include <map>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"

// using std::std::array;
// using std::std::map;
// using std::pair;

/**
 * Detector Construction Class
 * ---
 *
 * Defines the geometry and the materials of the detector used
 * in the simulation.
 */
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
private:
    // dimensions
    static G4int nPixel;

    static G4double xWorld;
    static G4double yWorld;
    static G4double zWorld;

    static G4double xPixel;
    static G4double yPixel;
    static G4double zPixel;

    // map with centers of pixels
    static std::map<G4int, std::array<G4double, 2>> pixelMap;

    // volumes
    G4Box *solidWorld, *solidDetector;
    G4LogicalVolume *logicWorld, *logicDetector;
    G4VPhysicalVolume *physWorld, *physDetector;

    // materials
    G4Material *worldMat, *detectorMat;
    void DefineMaterials();

    void ConstructSDandField() override;

public:
    MyDetectorConstruction();
    ~MyDetectorConstruction() override = default;

    G4VPhysicalVolume *Construct() override;
    void ConstructWorld();
    void ConstructPixels();

    static void SetNPixel(G4int);
    static G4int GetNPixel();

    static std::array<G4double, 3> GetWorldDimensions();
    static std::array<G4double, 3> GetPixelDimensions();
    static void SetPixelDimensions(G4double);

    static void setVisualization();

    static void AddToPixelMap(std::pair<G4int, std::array<G4double, 2>>);
    static std::map<G4int, std::array<G4double, 2>> GetPixelMap();
    static void PrintPixelMap();
};