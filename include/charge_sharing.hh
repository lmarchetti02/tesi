#pragma once

#include <array>
#include <ctime>
#include <memory>
#include <vector>
#include <map>

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#include "construction.hh"
#include "detector.hh"

using std::array;
using std::map;
using std::vector;

namespace charge_sharing
{
    template <typename T>
    T sum_vector(const vector<T> &vector)
    {
        T sum = 0;

        for (T element : vector)
        {
            sum += element;
        }
        return sum;
    }

    array<double, 2> sample(array<double, 2> center, double std)
    {
        const G4double X_MAX = MyDetectorConstruction::GetWorldDimensions()[0];
        const G4double Y_MAX = MyDetectorConstruction::GetWorldDimensions()[0];

        // check for values outside the pixel array
        G4double x, y;
        do
        {
            G4double x = G4RandGauss::shoot(center[0], std);
            G4double y = G4RandGauss::shoot(center[1], std);
        } while (abs(x) >= X_MAX && abs(y) >= Y_MAX);

        array<double, 2> s = {x, y};

        return s;
    }

    std::unique_ptr<vector<G4double>> split_energy(double totalEnergy, int N)
    {
        const G4double energy = totalEnergy / N;
        auto res = std::make_unique<vector<G4double>>();

        for (int i = 0; i < N; i++)
        {
            res->push_back(energy);
        }

        return res;
    }

    G4int which_pixel(array<G4double, 2> position)
    {
        G4double s = MyDetectorConstruction::GetPixelDimensions()[0];
        map<G4int, array<G4double, 2>> pixels = MyDetectorConstruction::GetPixelMap();

        G4int firstPixel = MySensitiveDetector::GetPixelFirstHit();

        if (abs(position[0] - pixels[firstPixel][0]) < s && abs(position[1] - pixels[firstPixel][1]) < s)
            return firstPixel;

        for (auto itr = pixels.begin(); itr != pixels.end(); itr++)
        {
            G4double x = itr->second[0];
            G4double y = itr->second[1];

            if (abs(position[0] - x) < s && abs(position[1] - y) < s)
                return itr->first;
        }

        G4cout << "No pixel found" << G4endl;
        return 0;
    }

    vector<G4double> add_charge_sharing(const vector<G4double> &energyVector, G4int nParts)
    {
        const G4double TOTAL_ENERGY = sum_vector(energyVector);
        const G4double ENERGY = TOTAL_ENERGY / nParts;
        const array<G4double, 2> FIRST_HIT = {MySensitiveDetector::GetFirstHit()[0], MySensitiveDetector::GetFirstHit()[1]};

        vector<G4double> result = energyVector;

        for (G4int i = 0; i < nParts; i++)
        {
            array<G4double, 2> randomXY = sample(FIRST_HIT, 11 * um);
            G4int randomID = which_pixel(randomXY);

            result[randomID] += ENERGY;
        }

        return result;
    }
}
