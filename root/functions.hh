#pragma once

#include <map>
#include <vector>
#include <iostream>

#include "TTree.h"

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::vector;

double get_mean(const vector<double> *data)
{
    double mean = 0;

    for (long int i = 0; i < (long int)data->size(); i++)
    {
        mean += data->at(i);
    }

    return mean / data->size();
}

void print_map(const map<int, double> &map)
{
    for (auto itr = map.begin(); itr != map.end(); itr++)
    {
        cout << itr->first << " : " << itr->second << endl;
    }
}

map<int, double> read_ntuple(TTree *tree)
{
    int ID;
    double Energy;
    tree->SetBranchAddress("ID", &ID);
    tree->SetBranchAddress("eVector", &Energy);

    map<int, vector<double> *> fMap;

    for (long int i = 0; i < (long int)tree->GetEntries(); i++)
    {
        tree->GetEntry(i);

        if (fMap.count(ID) != 1)
        {
            auto *v = new vector<double>(1, Energy);

            pair<int, vector<double> *> p(ID, v);
            fMap.insert(p);
        }
        else
            fMap[ID]->push_back(Energy);
    }

    map<int, double> result;
    for (auto itr = fMap.begin(); itr != fMap.end(); itr++)
    {
        pair<int, double> p(itr->first, get_mean(itr->second));
        result.insert(p);
    }

    return result;
}

map<int, vector<double>> reconstruct_grid(const int N)
{
    const double DIM = 0.5;
    const double STEP = DIM / N;

    map<int, vector<double>> result;

    // generate (x,y)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int ID = i * N + j;
            double x = -DIM + (2 * j + 1) * STEP;
            double y = -DIM + (2 * i + 1) * STEP;
            vector<double> xy = {x, y};

            pair<int, vector<double>> p(ID, xy);
            result.insert(p);
        }
    }

    return result;
}