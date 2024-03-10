#include <array>
#include <cmath>
#include <fstream>

#include "globals.hh"

namespace spectrum
{
  constexpr G4int size = 20;

  G4double energy_distribution(G4double x)
  {
    G4double mu = 0.06;
    G4double sigma = 0.01;

    return (1 / (sigma * sqrt(2 * M_PI))) * exp(-((x - mu) * (x - mu)) / (2 * sigma * sigma));
  }

  void generate_points(std::array<G4double, size> &x,
                       std::array<G4double, size> &y)
  {
    constexpr G4double min = 0.02;
    constexpr G4double max = 0.1;
    constexpr G4double step = (max - min) / size;

    for (G4int i = 0; i < size; i++)
      x[i] = min + step * i;

    for (G4int i = 0; i < size; i++)
      y[i] = energy_distribution(x[i]);
  }

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
        out_file << std::setprecision(3) << x[i] << "  " << y[i] << "\n";
    }
    out_file.close();
  }
} // namespace spectrum
