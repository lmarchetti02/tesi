#pragma once

#include <map>
#include <vector>
#include <memory>
#include <array>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>

#include "TTree.h"

#include "functions.hh"

namespace data
{

    /**
     * Function for reading the "Hits" tree.
     *
     * @param[in] tree The pointer to the TTree where the hits are stored.
     * @param[in] nPixel The number of pixel per side of the detector.
     * @param[in] EventN The event number.
     *
     * @result An `std::map` with { detectorID, mean(energyDep) }.
     */
    std::map<int, double> read_hits_tree(TTree *Tree, int nPixel, int EventN)
    {
        int ID;
        double Energy;
        int Event;
        Tree->SetBranchAddress("ID", &ID);
        Tree->SetBranchAddress("Energy", &Energy);
        Tree->SetBranchAddress("Event", &Event);

        auto outMap = std::map<int, double>();
        for (int i = 0; i < nPixel * nPixel; i++)
        {
            outMap.insert(std::make_pair(i, 0));
        }

        for (int i = 0; i < Tree->GetEntries(); i++)
        {
            Tree->GetEntry(i);

            if (Event == EventN)
            {
                outMap[ID] = Energy;
            }
        }

        for (auto &e : outMap)
        {
            cout << e.first << " : " << e.second;
            cout << "\n";
        }

        return outMap;
    }

    /**
     * Function for reconstructing the pixel array.
     *
     * @param[in] nPixel The number of pixels.
     * @param[in] dimPixel The dimensions of the pixel.
     *
     * @return A map with the ID of the pixel and an array with (x,y) of its center.
     */
    std::map<int, std::array<double, 2>> reconstruct_grid(int nPixel, double dimPixel)
    {
        const double STEP = dimPixel / 2;
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

/**
 * Function for reading the "Hits" tree.
 *
 * @param[in] tree The pointer to the TTree where the hits are stored.
 * @param[in] nPixel The number of pixel per side of the detector.
 * @param[in] nEvents The total number of events.
 *
 * @result An `std::map` with { detectorID, mean(energyDep) }.
 */
// std::map<int, double> read_hits_tree(TTree *Tree, int nPixel, int nEvents)
// {
//     int ID;
//     double Energy;
//     int Event;
//     Tree->SetBranchAddress("ID", &ID);
//     Tree->SetBranchAddress("Energy", &Energy);
//     Tree->SetBranchAddress("Event", &Event);

//     // initialize map
//     auto outMap = std::map<int, std::vector<double>>();
//     for (int i = 0; i < nPixel * nPixel; i++)
//     {
//         auto v = std::vector<double>(nEvents, 0);
//         outMap.insert(std::make_pair(i, v));
//         v.clear();
//     }

//     for (int i = 0; i < Tree->GetEntries(); i++)
//     {
//         Tree->GetEntry(i);
//         outMap[ID][Event] = Energy;

//         // cout << ID << " " << Energy << " " << Event << endl;
//     }

//     // for (auto &e : outMap)
//     // {
//     //     cout << e.first << " : ";
//     //     for (double d : e.second)
//     //     {
//     //         cout << d << " ";
//     //     }
//     //     cout << "\n";
//     // }

//     return functions::map_mean(outMap, nPixel);
// }