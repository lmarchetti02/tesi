#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <memory>

#include "TTree.h"

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::vector;

namespace functions
{
    double sum(const vector<double> &data)
    {
        double sum = 0;

        for (long int i = 0; i < (long int)data.size(); i++)
        {
            sum += data.at(i);
        }

        return sum;
    }

    double mean(const vector<double> &data)
    {
        double mean = 0;

        mean = sum(data) / data.size();

        return mean;
    }

    void print_map(const map<int, double> &map)
    {
        for (auto itr = map.begin(); itr != map.end(); itr++)
        {
            cout << itr->first << " : " << itr->second << endl;
        }
    }

    void print_vector(const vector<double> &v)
    {
        cout << "[";
        for (double d : v)
        {
            cout << d << ", ";
        }
        cout << "]" << endl;
    }

    map<int, double> get_mean(std::unique_ptr<map<int, vector<double>>> &inMap, const int N)
    {
        map<int, double> result;

        for (auto itr = inMap->begin(); itr != inMap->end(); itr++)
        {
            pair<int, double> p(itr->first, mean(itr->second));
            result.insert(p);
        }

        for (int i = 0; i < N * N; i++)
        {
            if (!result.count(i))
            {
                result[i] = 0;
            }
        }

        return result;
    }

    map<int, double> energy_deposition(std::unique_ptr<map<int, vector<double>>> &inMap)
    {
        auto container = map<int, vector<double>>();

        for (auto itr = inMap->begin(); itr != inMap->end(); itr++)
        {
            long int evtCounter = 0;
            for (double d : itr->second)
            {
                container[evtCounter].push_back(d);
                evtCounter++;
            }
        }

        auto res = map<int, double>();
        for (auto itr = container.begin(); itr != container.end(); itr++)
        {
            res[itr->first] = sum(itr->second);
        }

        cout << endl;
        cout << "ENERGY DEPOSITION PER EVENT" << endl;
        cout << "-----------------" << endl;
        print_map(res);
        cout << endl;

        return res;
    }
}