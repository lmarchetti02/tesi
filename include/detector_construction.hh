#pragma once

#include <array>
#include <map>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Box.hh"

/**
 * Detector Construction Class
 * ---
 *
 * Defines the geometry and the materials of the detector used
 * in the simulation.
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
private:
    // dimensions
    G4int nPixel;

    G4double xWorld;
    G4double yWorld;
    G4double zWorld;

    G4double zPlanes;

    G4double xPixel;
    G4double yPixel;
    G4double zPixel;

    // map with centers of pixels
    static std::map<G4int, std::array<G4double, 2>> pixelMap;

    // volumes
    G4Box *solidWorld, *solidDetector, *solidPlane;
    G4LogicalVolume *logicWorld, *logicDetector, *logicPlane;
    G4VPhysicalVolume *physWorld, *physDetector, *physPlane1, *physPlane2;

    G4LogicalVolume *scoringVolume;
    G4LogicalVolume *scoringPlane;

    void ConstructWorld();
    void ConstructPixels();
    void ConstructPlanes();

    // materials
    G4Material *worldMat, *detectorMat;
    void DefineMaterials();

    void ConstructSDandField() override;

public:
    DetectorConstruction();
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume *Construct() override;

    static void SetVisualization();

    static void AddToPixelMap(std::pair<G4int, std::array<G4double, 2>>);
    static std::map<G4int, std::array<G4double, 2>> GetPixelMap() { return pixelMap; }

    G4LogicalVolume *GetScoringVolume() const { return scoringVolume; }
    G4LogicalVolume *GetScoringPlane() const { return scoringPlane; }
};