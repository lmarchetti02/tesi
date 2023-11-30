#pragma once

#include <map>
#include <vector>
#include <memory>
#include <array>
#include <fstream>
#include <limits>
#include <string>

#include "TTree.h"
#include "functions.hh"

namespace data
{
    double pixelSize = 50e-6;

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
     * Function for reconstructing the pixel array.
     *
     * @param[in] nPixel The number of pixels.
     *
     * @return A map with the ID of the pixel and an array with (x,y) of its center.
     */
    std::map<int, std::array<double, 2>> reconstruct_grid(int nPixel)
    {
        const double STEP = pixelSize / 2;
        const double DIM = STEP * nPixel;

        std::map<int, std::array<double, 2>> result;

        // generate (x,y)
        for (int i = 0; i < nPixel; i++)
        {
            for (int j = 0; j < nPixel; j++)
            {
                int ID = i * nPixel + j;
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