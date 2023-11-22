#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <map>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh" // allows the use of SI units
#include "G4GenericMessenger.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"

using std::array;
using std::map;
using std::pair;
using std::vector;

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
private:
    // dimensions
    static G4int nPixel;

    static G4double xWorld;
    static G4double yWorld;
    static G4double zWorld;

    static G4double xDet;
    static G4double yDet;
    static G4double zDet;

    // map with centers of pixels
    static map<G4int, array<G4double, 2>> pixelMap;

    // volumes (to be only defined once)
    G4Box *solidWorld, *solidDetector;
    G4LogicalVolume *logicWorld, *logicDetector;
    G4VPhysicalVolume *physWorld, *physDetector;

    // scoring volume
    G4LogicalVolume *fScoringVolume;

    // materials (to be only defined once)
    G4Material *worldMat, *detectorMat;
    void DefineMaterials();

    // create detector
    virtual void ConstructSDandField();

public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    // create all the volumes
    virtual G4VPhysicalVolume *Construct();

    // set number of pixels
    static void SetNPixel(G4int);
    static G4int GetNPixel() { return nPixel; }

    // access geometry dimensions
    static array<G4double, 3> GetWorldDimensions();
    static array<G4double, 3> GetPixelDimensions();

    // modify visualization
    static void setVisualization();

    // access pixel map
    static void AddToPixelMap(const pair<G4int, array<G4double, 2>> &p) { pixelMap.insert(p); }
    static map<G4int, array<G4double, 2>> GetPixelMap() { return pixelMap; }
    static void PrintPixelMap();
};