#pragma once

#include <array>
#include <cmath>
#include <fstream>

#include "globals.hh"

#include "constants.hh"

namespace spectrum
{
  constexpr G4int size = 100;

  /**
   * Function for generating a gaussian distribution.
   *
   * @param[in] x The point at which the gaussian is evaluated
   * @param[in] xMax The maximum allowed energy
   */
  G4double gaussian_distribution(G4double x, G4double xMax)
  {
    G4double mu = 0.06;
    G4double sigma = 0.005;

    if (x < xMax)
      return (1 / (sigma * sigma * sqrt(2 * M_PI))) * exp(-((x - mu) * (x - mu)) / (2 * sigma * sigma));

    return 0;
  }

  /**
   * Function for generating a linear distribution.
   *
   * @param[in] x The point at which the linear function is evaluated
   * @param[in] xMax The maximum allowed energy
   */
  G4double linear_distribution(G4double x, G4double xMax)
  {
    if (x < xMax)
      return (xMax - x) * 2 / (xMax * xMax);

    return 0;
  }

  /**
   * Function for generating a uniform distribution.
   *
   * @param[in] x The point at which the distribution is evaluated
   * @param[in] xMax The maximum allowed energy
   */
  G4double uniform_distribution(G4double x, G4double xMax)
  {
    if (x < xMax)
      return 1. / xMax;

    return 0;
  }

  /**
   * Function for generating the (x, f(x)) points
   * that define the energy distribution.
   *
   * @param[inout] x The reference to the vector of the x points
   * @param[inout] y The reference to the vector of the f(x) points
   * @param[in] beamType The type of spectrum
   * @param[in] xMax The maximum value fo x
   */
  void generate_points(std::array<G4double, size> &x, std::array<G4double, size> &y, G4int beamType, G4double xMax)
  {
    G4double min = 0.;
    G4double max = xMax;
    G4double step = (max - min) / size;

    for (G4int i = 0; i < size; i++)
      x[i] = min + step * i;

    for (G4int i = 0; i < size; i++)
    {
      switch (beamType)
      {
      case 1:
        y[i] = gaussian_distribution(x[i], xMax);
        break;
      case 2:
        y[i] = uniform_distribution(x[i], xMax);
        break;
      case 3:
        y[i] = linear_distribution(x[i], xMax);
        break;
      default:
        y[i] = 0;
        break;
      }
    }
  }

  /**
   * Function for saving the distribution
   * to a .dat file.
   *
   * @param[in] beamType The type of spectrum
   * @param[in] xMax The maximum value of x
   */
  void save_to_file(G4int beamType, G4double xMax)
  {
    std::array<G4double, size> x{};
    std::array<G4double, size> y{};

    generate_points(x, y, beamType, xMax);

    std::fstream out_file;
    out_file.open("spectrum.dat", std::ios::out);
    if (out_file.is_open())
    {
      for (G4int i = 0; i < size; i++)
        out_file << x[i] << "  " << y[i] << "\n";
    }
    out_file.close();
  }

} // namespace spectrum
