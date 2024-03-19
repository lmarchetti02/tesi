#include <array>
#include <cmath>
#include <fstream>

#include "globals.hh"

namespace spectrum
{
  constexpr G4int size = 100;

  /**
   * Function that defines the form of the
   * energy distribution.
   */
  G4double energy_distribution(G4double x)
  {
    // G4double mu = 0.06;
    // G4double sigma = 0.005;

    // return (1 / (sigma * sqrt(2 * M_PI))) * exp(-((x - mu) * (x - mu)) / (2 * sigma * sigma));

    return 20 - 200 * x;
  }

  /**
   * Function for generating the (x,y) points that
   * describe the energy distribution.
   */
  void generate_points(std::array<G4double, size> &x, std::array<G4double, size> &y)
  {
    constexpr G4double min = 0.;
    constexpr G4double max = 0.1;
    constexpr G4double step = (max - min) / size;

    for (G4int i = 0; i < size; i++)
      x[i] = min + step * i;

    for (G4int i = 0; i < size; i++)
      y[i] = energy_distribution(x[i]);
  }

  /**
   * Function for saving the energy distribution to
   * a file that can be imported into the simulation.
   */
  void save_to_file()
  {
    std::array<G4double, size> x{};
    std::array<G4double, size> y{};

    generate_points(x, y);

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
