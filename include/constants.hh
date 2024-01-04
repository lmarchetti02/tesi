#pragma once

#include "G4SystemOfUnits.hh"
#include "globals.hh"

#define N_PIXELS 15
#define PIXEL_RATIO 5
#define N_SUBPIXEL N_PIXELS *PIXEL_RATIO

#define XY_PIXEL 22.5 * um
#define Z_PIXEL 0.5 * mm
#define XY_SUBPIXEL XY_PIXEL / PIXEL_RATIO

#define TGT_MATERIAL "G4_CADMIUM_TELLURIDE"

#define TOLL_WORLD 10 * um
#define Z_WORLD Z_PIXEL * 2

/**
 * Defines how the pixels should be irradiated:
 *  - 0 for uniform irradiation of the central pixel
 *  - 1 for uniform irradiation of the whole array
 *  - 2 for pointwise irradiation of the center pf the central pixel
 */
#define BEAM_WIDTH 1

/**
 * For charge sharing (see charge_sharing.hh):
 *  - `N_PARTS_ENERGY` is the number of parts in which each pixel energy
 *    is divided into
 *  - `SMEAR_WIDTH` id the standard deviation of the gaussian used for
 *    distributing the parts around
 */
#define N_PARTS_ENERGY 50
#define SMEAR_WIDTH 10. * um