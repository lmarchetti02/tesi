#pragma once

#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include <array>
#include <filesystem>

/**
 * Whether to save the energy in the subpixels (before
 * adding the charge sharing) in the .root file.
 */
constexpr G4bool INCLUDE_ORIGINAL = false;
/**
 * Whether to save the energy in the subpixels (after
 * adding the charge sharing) in the .root file.
 */
constexpr G4bool INCLUDE_CS = false;

constexpr G4int N_PIXEL = 7;

// pixel ratio must be odd
constexpr G4int PIXEL_RATIO = 7;
constexpr G4int N_SUBPIXEL = N_PIXEL * PIXEL_RATIO;

constexpr G4double XY_PIXEL = 175 * um;
constexpr G4double XY_SUBPIXEL = XY_PIXEL / PIXEL_RATIO;
constexpr G4double Z_PIXEL = 0.5 * mm;

constexpr G4double Z_PLANES = 5 * um;
constexpr G4double XY_WORLD = XY_SUBPIXEL * N_SUBPIXEL;
constexpr G4double Z_WORLD = Z_PIXEL * 5;

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
constexpr G4int N_PARTS_ENERGY = 500;
/**
 * The standard deviation of the gaussian used for distributing the energy bits around.
 * See charge_sharing.hh for more info.
 */
constexpr G4double SMEAR_WIDTH = 25 * um;

/**
 * Make the center of the charge-sharing
 * gaussian randomly chosen within the pixel.
 */
constexpr G4bool RND_GAUSS_CENTER = true;

/**
 * Spectrum of the illuminating photons:
 *  - 0 for monochromatic photons
 *  - 1 for gaussian distribution
 *  - 2 for uniform distribution
 *  - 3 for linear distribution with a MAX_ENE
 *  - 4 from user file (spectrum.dat)
 */
constexpr G4int SPECTRUM_SHAPE = 4;

// The maximum energy
constexpr G4double MAX_ENE = 150. * keV;

// paths where the .dt files are stored
const std::array<std::filesystem::path, 4> DAT_PATHS = {"../spectrums/gauss.dat", "../spectrums/uniform.dat",
                                                        "../spectrums/linear.dat", "../spectrums/spectrum.dat"};
