#pragma once

#include "G4SystemOfUnits.hh"
#include "globals.hh"

constexpr G4int N_PIXELS = 15;
constexpr G4int PIXEL_RATIO = 5;
constexpr G4int N_SUBPIXEL = N_PIXELS * PIXEL_RATIO;

constexpr G4double XY_PIXEL = 22.5 * um;
constexpr G4double Z_PIXEL = 0.5 * mm;
constexpr G4double XY_SUBPIXEL = XY_PIXEL / PIXEL_RATIO;

constexpr const char *TGT_MATERIAL = "G4_CADMIUM_TELLURIDE";

constexpr G4double TOLL_WORLD = 10 * um;
constexpr G4double Z_WORLD = Z_PIXEL * 2;

/**
 * Defines how the pixels should be irradiated:
 *  - 0 for uniform irradiation of the central pixel
 *  - 1 for uniform irradiation of the whole array
 *  - 2 for pointwise irradiation of the center pf the central pixel
 */
constexpr G4int BEAM_WIDTH = 1;

/**
 * For charge sharing (see charge_sharing.hh):
 *  - `N_PARTS_ENERGY` is the number of parts in which each pixel energy
 *    is divided into
 *  - `SMEAR_WIDTH` id the standard deviation of the gaussian used for
 *    distributing the parts around
 */
constexpr G4int N_PARTS_ENERGY = 50;
constexpr G4double SMEAR_WIDTH = 10 * um;