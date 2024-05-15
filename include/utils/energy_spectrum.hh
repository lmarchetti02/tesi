#pragma once

#include "G4Types.hh"
#include "constants.hh"

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>

namespace spectrum
{
constexpr G4int size = 100;

/**
 * Function for generating a gaussian distribution.
 *
 * @param[in] x The point at which the gaussian is evaluated
 * @return The f(x) corresponding to the input x.
 */
inline G4double gaussian_distribution(G4double x)
{
    G4double mu = 0.06;
    G4double sigma = 0.005;

    return (1 / (sigma * sigma * sqrt(2 * M_PI))) * exp(-((x - mu) * (x - mu)) / (2 * sigma * sigma));
}

/**
 * Function for generating a uniform distribution.
 *
 * @param[in] x The point at which the distribution is evaluated
 * @return The f(x) corresponding to the input x.
 */
inline G4double uniform_distribution(G4double x) { return 1. / MAX_ENE; }

/**
 * Function for generating a linear distribution.
 *
 * @param[in] x The point at which the linear function is evaluated
 * @return The f(x) corresponding to the input x.
 */
inline G4double linear_distribution(G4double x)
{
    return (MAX_ENE - x) * 2 / (MAX_ENE * MAX_ENE);

    return 0;
}

/**
 * Function for saving the .dat files
 * containing the energy spectrums.
 */
inline void save_spectrum_files()
{
    using std::filesystem::path;
    typedef G4double (*distribution)(G4double);
    const std::map<path, distribution> files_map = {{DAT_PATHS[0], gaussian_distribution}, {DAT_PATHS[1], uniform_distribution}, {DAT_PATHS[2], linear_distribution}};

    for (auto &[path, dist] : files_map) {
        std::fstream out_file;
        out_file.open(path, std::ios::out);
        if (!out_file.is_open())
            std::runtime_error("Impossible to open .dat file.");

        const G4double min = 0.;
        const G4double max = MAX_ENE;
        const G4double step = (max - min) / size;

        for (G4int i = 0; i < size; i++) {
            G4double x = min + step * i;
            out_file << x << "  " << dist(x) << "\n";
        }

        out_file.close();
    }
}
} // namespace spectrum
