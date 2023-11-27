#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <map>
#include <fstream>

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
#include "G4AnalysisManager.hh"

using std::array;
using std::fstream;
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

    // volumes
    G4Box *solidWorld, *solidDetector;
    G4LogicalVolume *logicWorld, *logicDetector;
    G4VPhysicalVolume *physWorld, *physDetector;

    // materials
    G4Material *worldMat, *detectorMat;
    void DefineMaterials();

    virtual void ConstructSDandField();

public:
    MyDetectorConstruction();
    virtual ~MyDetectorConstruction() {}

    virtual G4VPhysicalVolume *Construct();
    void ConstructWorld();
    void ConstructPixels();

    static void SetNPixel(G4int);
    static G4int GetNPixel();

    static array<G4double, 3> GetWorldDimensions();
    static array<G4double, 3> GetPixelDimensions();

    static void setVisualization();

    static void AddToPixelMap(const pair<G4int, array<G4double, 2>> &);
    static map<G4int, array<G4double, 2>> GetPixelMap();
    static void PrintPixelMap();
};