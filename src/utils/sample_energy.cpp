#include "sample_energy.hh"

#include <string>
#include <fstream>

#include "Randomize.hh"

/**
 * The default destructor.
 */
SampleEnergy::~SampleEnergy()
{
    x.clear();
    f.clear();
    F.clear();
}

/**
 * Function for reading the .dat file
 * and generating the cumulative distribution.
 *
 * @param[in] fileName The name of the file where the energy distribution
 */
void SampleEnergy::ReadFile(const char *fileName)
{
    // get data from file
    std::fstream inFile;
    inFile.open(fileName, std::ios::in);

    if (!inFile.is_open())
    {
        G4cout << "SampleEnergy::SampleEnergy(): Error opening file " << fileName << G4endl;
        exit(0);
    }

    G4double energy, value;
    while (!inFile.eof())
    {
        inFile >> energy >> value;
        x.push_back(energy);
        f.push_back(value);
        F.push_back(0);
    }

    inFile.close();

    // add zero to front
    x.insert(x.begin(), 0);
    f.insert(f.begin(), 0);
    F.insert(F.begin(), 0);

    // build the cumulative
    F[0] = f[0];
    for (int i = 1; i < x.size(); i++)
        F[i] = F[i - 1] + f[i];

    // normalize
    double factor = 1. / F[x.size() - 1];

    for (int i = 1; i < x.size(); i++)
        F[i] *= factor;
}

/**
 * Function for sampling a pseudo-random value from the
 * energy distribution.
 */
G4double SampleEnergy::Sample()
{
    G4double u = G4UniformRand();
    // the rnd number is between two consecutive y_c values.
    // the following code should be modified using a binary search
    G4double energy;
    for (int i = 0; i < F.size(); i++)
    {
        if (u < F[i])
        {
            energy = (x[i + 1] - x[i]) / (F[i + 1] - F[i]) * (u - F[i]) + x[i];
            break;
        }
    }

    return energy;
}
