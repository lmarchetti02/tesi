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

map<vector<double>, vector<double>> reconstruct_grid(int N)
{
    const double DIM = 1.0;
    const double STEP = DIM / N;

    map<vector<double>, vector<double>> result;
    vector<double> values;

    for (int i = 0; i < N; i++)
    {
        values.push_back(i + STEP / 2);
    }

    pair<vector<double>, vector<double>> p(values, values);
    result.insert(p);

    return result;
}