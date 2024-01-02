#pragma once

#include "G4SystemOfUnits.hh"
#include "globals.hh"

#define N_SUBPIXEL 45
#define PIXEL_RATIO 3

#define XY_PIXEL 22.5 * um
#define XY_SUBPIXEL 7.5 * um
#define Z_PIXEL 0.5 * mm
#define Z_WORLD 1 * mm

/**
 * Defines how the pixels should be irradiated:
 *  - 0 for uniform irradiation of the central pixel
 *  - 1 for uniform irradiation of the whole array
 *  - 2 for pointwise irradiation of the center pf the central pixel
 */
#define BEAM_WIDTH 1