#pragma once

#include <array>
#include <iostream>
#include <chrono>
#include <ctime>

#include "TRandom2.h"

#include "functions.hh"
#include "data.hh"

using std::array;
using std::cout;
using std::endl;

namespace charge_sharing
{
    array<double, 2> sample(array<double, 2> center, double std)
    {
        TRandom2 *rand = new TRandom2(time(0));

        double x = rand->Gaus(center[0], std);
        double y = rand->Gaus(center[1], std);

        array<double, 2> s = {x, y};

        return s;
    }

    vector<double> split_energy(double totalEnergy, int N)
    {
        const double energy = totalEnergy / N;
        vector<double> res = vector<double>();

        for (int i = 0; i < N; i++)
        {
            res.push_back(energy);
        }

        return res;
    }

    int which_pixel(array<double, 2> position, int N)
    {
        double s = data::pixelSize / 2;
        map<int, array<double, 2>> pixels = data::reconstruct_grid(N);

        for (auto itr = pixels.begin(); itr != pixels.end(); itr++)
        {
            double x = itr->second[0];
            double y = itr->second[1];

            if (abs(position[0] - x) < s && abs(position[1] - y) < s)
                return itr->first;
        }

        cout << "No pixel found" << endl;
        return 0;
    }

}
