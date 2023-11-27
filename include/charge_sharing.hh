#pragma once

#include <array>
#include <ctime>
#include <memory>
#include <vector>
#include <map>
#include <cmath>

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#include "construction.hh"
#include "sensitive_detector.hh"

using std::array;
using std::map;
using std::vector;

namespace charge_sharing
{
    /*
    Template for getting the sum of all the elements in a vector.
    */
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

    /*
    Function for computing the absolute value of a G4double.
    */
    G4double absolute(G4double value)
    {
        if (value >= 0)
            return value;
        else if (value < 0)
            return -value;

        return 0;
    }

    /*
    Function for returning an array containing (x,y) of a point in the detector plane,
    chose randomly according to a 2D gaussian.
    */
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

    /*
    Function for determining which pixel corresponds to a given point (x,y).
    */
    G4int which_pixel(array<G4double, 2> position)
    {
        G4double xWorld = MyDetectorConstruction::GetWorldDimensions()[0];
        G4double yWorld = MyDetectorConstruction::GetWorldDimensions()[1];
        G4double xDet = MyDetectorConstruction::GetPixelDimensions()[0];
        G4double yDet = MyDetectorConstruction::GetPixelDimensions()[1];
        G4int nPixel = MyDetectorConstruction::GetNPixel();

        G4int j = ((position[0] + xWorld) / xDet - 1) * 0.5;
        G4int i = ((position[1] + yWorld) / yDet - 1) * 0.5;
        G4int ID = i * nPixel + j;

        return ID;
    }

    vector<G4double> add_charge_sharing(const vector<G4double> &energyVector, G4int nParts)
    {
        const G4double TOTAL_ENERGY = sum_vector(energyVector);
        const G4double ENERGY = TOTAL_ENERGY / nParts;
        const array<G4double, 2> FIRST_HIT = {MySensitiveDetector::GetFirstHit()[0], MySensitiveDetector::GetFirstHit()[1]};

        // G4cout << "First pixel: " << which_pixel(FIRST_HIT) << G4endl;

        G4int N = MyDetectorConstruction::GetNPixel();
        auto result = vector<G4double>(N * N, 0);

        for (G4int i = 0; i < nParts; i++)
        {
            array<G4double, 2> randomXY = sample(FIRST_HIT, 11 * um);
            G4int randomID = which_pixel(randomXY);

            result[randomID] += ENERGY;
        }

        return result;
    }
}
