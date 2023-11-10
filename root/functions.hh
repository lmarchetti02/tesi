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

double get_mean(const vector<double> &data)
{
    double mean = 0;

    for (double value : data)
    {
        mean += value;
    }

    return mean / data.size();
}

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
