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

    /**
     * Function for reading the info.txt file and extracting the
     * pixel size from it.
     *
     * @return The pixel size in meters.
     */
    double get_pixel_size()
    {
        std::fstream infoFile;
        double pixelSize = 0;

        infoFile.open("results/info.txt", std::ios::in);
        if (infoFile.is_open())
        {
            int counter = 0;
            std::string line;
            while (getline(infoFile, line))
            {
                if (counter >= 3 && counter < 4)
                {
                    std::string dim = line.substr(24, 4);
                    pixelSize = std::stod(dim) * 1e-3;
                }
                counter++;
            }
        }
        infoFile.close();

        return pixelSize;
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
        int Event;
        Tree->SetBranchAddress("ID", &ID);
        Tree->SetBranchAddress("Energy", &Energy);
        Tree->SetBranchAddress("Event", &Event);

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
        const double STEP = get_pixel_size() / 2;
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

    /**
     * Function for saving the results of the simulation in a file; in particular
     * (xPixel, yPixel, Edep).
     *
     * @param[in] pixelMap The map containing the centers of the pixels.
     * @param[in] hitsMap The map containing energy deposition of the pixels.
     */
    void save_results(std::map<int, std::array<double, 2>> pixelMap, std::map<int, double> hitsMap)
    {
        std::fstream dataFile;
        dataFile.open("results/data.txt", std::ios::out);
        if (dataFile.is_open())
        {
            dataFile << "RISULTATI DELLA SIMULAZIONE" << endl;
            dataFile << "---------------------------" << endl;

            unsigned long i = 0;
            while (i < pixelMap.size())
            {
                dataFile << pixelMap[i][0] << "," << pixelMap[i][1] << "," << hitsMap[i] << endl;
                i++;
            }
        }
        dataFile.close();
    }
}