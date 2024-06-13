#include "sample_energy.hh"

#include "G4ios.hh"
#include "Randomize.hh"
#include "constants.hh"

#include <fstream>

/**
 * The default constructor.
 *
 * It creates the cumulative distribution
 * of all the possible spectrums and stores
 * them into vectors.
 */
SampleEnergy::SampleEnergy()
{

    for (int i = 0; i < DAT_PATHS.size(); i++) {
        x.push_back(std::vector<G4double>());
        f.push_back(std::vector<G4double>());
        F.push_back(std::vector<G4double>());
    }

    for (int i = 0; i < DAT_PATHS.size(); i++) {
        // get data from file
        std::fstream inFile;
        inFile.open(DAT_PATHS[i], std::ios::in);

        if (!inFile.is_open()) {
            G4cout << "SampleEnergy::SampleEnergy(): Error opening file " << DAT_PATHS[i] << G4endl;
            std::exit(0);
        }

        G4double energy, value;
        while (!inFile.eof()) {
            inFile >> energy >> value;
            if (i == 3) energy /= 1000.; // from keV to MeV
            x[i].push_back(energy);
            f[i].push_back(value);
            F[i].push_back(0);
        }

        inFile.close();

        // add zero to front
        x[i].insert(x[i].begin(), 0);
        f[i].insert(f[i].begin(), 0);
        F[i].insert(F[i].begin(), 0);

        // build the cumulative
        F[i][0] = f[i][0];
        for (int j = 1; j < x[i].size(); j++)
            F[i][j] = F[i][j - 1] + f[i][j];

        // normalize
        double factor = 1. / F[i][x[i].size() - 1];

        for (int j = 1; j < x[i].size(); j++)
            F[i][j] *= factor;
    }
}

/**
 * Function for sampling a pseudo-random value from the
 * energy distribution.
 *
 * @param[in] type The energy spectrum.
 */
G4double SampleEnergy::Sample(G4int type)
{
    // correct for array index
    type -= 1;

    G4double u = G4UniformRand();
    // the rnd number is between two consecutive y_c values.
    // the following code should be modified using a binary search
    G4double energy{};
    for (int i = 0; i < F[type].size(); i++) {
        if (u < F[type][i]) {
            energy = (x[type][i + 1] - x[type][i]) / (F[type][i + 1] - F[type][i]) * (u - F[type][i]) + x[type][i];
            break;
        }
    }

    return energy;
}
