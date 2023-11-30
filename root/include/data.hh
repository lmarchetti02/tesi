#pragma once

#include <map>
#include <vector>
#include <memory>
#include <array>
#include <fstream>

#include "TTree.h"
#include "functions.hh"

namespace data
{
    double pixelSize = 50e-6;

    void read_simulation_info()
    {
        std::fstream infoFile;

        infoFile.open("../results/info.txt", std::ios::in);
        if (infoFile.is_open())
        {
        }
    }

    /**
     * Function for reading the "Hits" tree.
     *
     * @param[in] tree The pointer to the TTree where the hits are stored.
     * @param[in] N The number of pixel per side of the detector.
     *
     * @result An `std::map` with { detectorID, mean(energyDep) }.
     */
    std::map<int, double> read_hits_tree(TTree *Tree, int nPixel)
    {
        int ID;
        double Energy;
        Tree->SetBranchAddress("ID", &ID);
        Tree->SetBranchAddress("eVector", &Energy);

        auto fMap = std::map<int, std::vector<double>>();

        for (long int i = 0; i < (long int)Tree->GetEntries(); i++)
        {
            Tree->GetEntry(i);

            if (fMap.count(ID) != 1)
            {
                auto v = std::vector<double>(1, Energy);

                std::pair<int, std::vector<double>> p(ID, v);
                fMap.insert(p);
            }
            else
                fMap.at(ID).push_back(Energy);
        }

        return functions::map_mean(fMap, nPixel);
    }

    /**
     * Function for reconstructing
     */
    std::map<int, std::array<double, 2>> reconstruct_grid(const int N)
    {
        const double STEP = pixelSize / 2;
        const double DIM = STEP * N;

        std::map<int, std::array<double, 2>> result;

        // generate (x,y)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int ID = i * N + j;
                double x = -DIM + (2 * j + 1) * STEP;
                double y = -DIM + (2 * i + 1) * STEP;
                std::array<double, 2> xy = {x, y};

                std::pair<int, std::array<double, 2>> p(ID, xy);
                result.insert(p);
            }
        }

        return result;
    }
}