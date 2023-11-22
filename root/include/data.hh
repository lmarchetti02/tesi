#pragma once

#include <map>
#include <vector>
#include <memory>
#include <array>

#include "TTree.h"
#include "functions.hh"

using std::array;
using std::map;
using std::pair;
using std::vector;

namespace data
{
    double pixelSize = 50e-6;

    map<int, double> read_hits(TTree *tree, const int N)
    {
        int ID;
        double Energy;
        tree->SetBranchAddress("ID", &ID);
        tree->SetBranchAddress("eVector", &Energy);

        auto fMap = std::make_unique<map<int, vector<double>>>();

        for (long int i = 0; i < (long int)tree->GetEntries(); i++)
        {
            tree->GetEntry(i);

            if (fMap->count(ID) != 1)
            {
                auto v = vector<double>(1, Energy);

                pair<int, vector<double>> p(ID, v);
                fMap->insert(p);
            }
            else
                fMap->at(ID).push_back(Energy);
        }

        // TODO: add energy deposition

        return functions::get_mean(fMap, N);
    }

    std::unique_ptr<map<int, array<double, 2>>> read_first_hit(TTree *tree)
    {
        double x, y;
        tree->SetBranchAddress("x", &x);
        tree->SetBranchAddress("y", &y);

        auto fhMap = std::make_unique<map<int, array<double, 2>>>();

        for (long int i = 0; i < (long int)tree->GetEntries(); i++)
        {
            tree->GetEntry(i);

            array<double, 2> xy = {x, y};
            pair<int, array<double, 2>> p(i, xy);

            fhMap->insert(p);
        }

        return fhMap;
    }

    map<int, array<double, 2>> reconstruct_grid(const int N)
    {
        const double STEP = pixelSize / 2;
        const double DIM = STEP * N;

        map<int, array<double, 2>> result;

        // generate (x,y)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int ID = i * N + j;
                double x = -DIM + (2 * j + 1) * STEP;
                double y = -DIM + (2 * i + 1) * STEP;
                array<double, 2> xy = {x, y};

                pair<int, array<double, 2>> p(ID, xy);
                result.insert(p);
            }
        }

        return result;
    }
}