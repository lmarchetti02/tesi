#pragma once

#include "Randomize.hh"
#include "constants.hh"
#include "detector_construction.hh"

#include <array>
#include <vector>

/**
 * Library containing the necessary methods for adding the
 * charge sharing to the results of the simulation.
 */
namespace charge_sharing
{
/**
 * Function for returning an array containing (x,y) of a point in the detector plane,
 * chose randomly according to a 2D gaussian.
 *
 * @param[in] center The array representing the center of the gaussian.
 * @param[in] std The standard deviation of the gaussian.
 *
 * @return The aforementioned random array.
 */
inline std::array<double, 2> sample(std::array<double, 2> center, const double std)
{
    // check for values outside the pixel array
    G4double x, y;
    x = G4RandGauss::shoot(center[0], std);
    y = G4RandGauss::shoot(center[1], std);

    std::array<double, 2> point = {x, y};

    return point;
}

/**
 * Function for determining which pixel corresponds to a given point (x,y).
 *
 * @param[in] position The array representing the point (x,y).
 *
 * @return The ID of the detector corresponding to (x,y).
 */
inline G4int which_pixel(const std::array<G4double, 2> position)
{
    const G4int j = ((position[0] + XY_WORLD) / XY_SUBPIXEL) * 0.5;
    const G4int i = ((position[1] + XY_WORLD) / XY_SUBPIXEL) * 0.5;
    G4int ID;
    if ((i >= 0 && i < N_SUBPIXEL) && (j >= 0 && j < N_SUBPIXEL))
        ID = i * N_SUBPIXEL + j;
    else
        ID = -1;

    return ID;
}

/**
 * Function that adds the charge sharing by following this algorithm:
 *  1. the energy deposition of a pixel is split into a sufficiently high number of parts;
 *  2. each part is moved around in the detector plane, following a 2D symmetrical gaussian distribution;
 *  3. each random (x,y) is converted into an ID and the energy is added to that pixel.
 *
 * @param[in] energyVector The vector containing the energy depositions before the charge sharing.
 * @param[in] nParts The number of parts the energy has to be divided into.
 *
 * @return The vector containing the energy depositions after the charge sharing.
 */
inline std::vector<G4double> add_charge_sharing(std::vector<G4double> energyVector, const G4int nParts)
{
    auto result = std::vector<G4double>(N_SUBPIXEL * N_SUBPIXEL, 0);

    for (G4int ID = 0; ID < energyVector.size(); ID++) {
        if (energyVector[ID] != 0) {
            const G4double PIXEL_ENERGY = energyVector[ID];
            const G4double ENERGY = PIXEL_ENERGY / nParts;
            std::array<G4double, 2> XY_CENTER = DetectorConstruction::GetPixelMap()[ID];

            if (RND_GAUSS_CENTER) {
                XY_CENTER[0] += (2 * G4UniformRand() * XY_SUBPIXEL - XY_SUBPIXEL);
                XY_CENTER[1] += (2 * G4UniformRand() * XY_SUBPIXEL - XY_SUBPIXEL);
            }

            for (G4int i = 0; i < nParts; i++) {
                std::array<G4double, 2> randomXY = sample(XY_CENTER, SMEAR_WIDTH);
                G4int randomID = which_pixel(randomXY);

                if (randomID != -1) result[randomID] += ENERGY;
            }
        }
    }

    return result;
}
} // namespace charge_sharing
