#pragma once

#include "G4SystemOfUnits.hh"
#include "globals.hh"

/**
 * Whether to save the energy in the subpixels (before
 * adding the charge sharing) in the .root file.
 */
constexpr G4bool INCLUDE_ORIGINAL = true;
/**
 * Whether to save the energy in the subpixels (after
 * adding the charge sharing) in the .root file.
 */
constexpr G4bool INCLUDE_CS = true;

constexpr G4int N_PIXEL = 15;
constexpr G4int PIXEL_RATIO = 5;
constexpr G4int N_SUBPIXEL = N_PIXEL * PIXEL_RATIO;

constexpr G4double XY_PIXEL = 100 * um;
constexpr G4double XY_SUBPIXEL = XY_PIXEL / PIXEL_RATIO;
constexpr G4double Z_PIXEL = 0.5 * mm;

constexpr G4double Z_PLANES = 5 * um;
constexpr G4double XY_WORLD = XY_SUBPIXEL * N_SUBPIXEL;
constexpr G4double Z_WORLD = Z_PIXEL * 4;

// The material of which the pixels are made.
constexpr const char TGT_MATERIAL[] = "G4_CADMIUM_TELLURIDE";

/**
 * Defines how the pixels should be irradiated:
 *  - 0 for uniform irradiation of the central pixel
 *  - 1 for uniform irradiation of the whole array
 *  - 2 for pointwise irradiation of the center pf the central pixel
 */
constexpr G4int BEAM_WIDTH = 1;

/**
 * The number of parts in which each pixel energy is divided into.
 * See charge_sharing.hh for more info.
 */
constexpr G4int N_PARTS_ENERGY = 50;
/**
 * The standard deviation of the gaussian used for distributing the energy bits around.
 * See charge_sharing.hh for more info.
 */
constexpr G4double SMEAR_WIDTH = 10 * um;