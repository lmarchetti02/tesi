#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <memory>

#include "TTree.h"

using std::cout;
using std::endl;

namespace functions
{
    /**
     * Template for getting the sum of all the elements inside an `std::vector`.
     *
     * @tparam T The type of the elements of the vector.
     * @param[in] data The vector.
     *
     * @return The sum of the elements of the vector.
     */
    template <typename T>
    T sum(std::vector<T> data)
    {
        T result = 0;

        for (long int i = 0; i < (long int)data.size(); i++)
        {
            result += data.at(i);
        }

        return result;
    }

    /**
     * Template for getting the mean of the elements in an `std::vector`.
     *
     * @tparam T The type of the elements of the vector.
     * @param[in] data The vector.
     *
     * @return The mean of the elements of the vector.
     */
    template <typename T>
    T mean(std::vector<T> data)
    {
        T result = 0;

        result = sum(data) / data.size();

        return result;
    }

    /**
     * Function for printing a (int, double) map.
     *
     * @param[in] map The map to be printed.
     */
    void print_map(std::map<int, double> map)
    {
        for (auto itr = map.begin(); itr != map.end(); itr++)
        {
            cout << itr->first << " : " << itr->second << endl;
        }
    }

    /**
     * Function for printing a (int, (double, double, double)) map.
     *
     * @param[in] map The map to be printed.
     */
    void print_map(std::map<int, std::array<double, 3>> map)
    {
        for (auto itr = map.begin(); itr != map.end(); itr++)
        {
            cout << itr->first << " : ";
            cout << "(" << itr->second[0] << ", " << itr->second[1] << ", " << itr->second[2] << ")" << endl;
        }
    }

    /**
     * Template for printing an `std::vector`.
     *
     * @tparam T the type of the element of the vector.
     * @param[in] v
     */
    template <typename T>
    void print_vector(std::vector<double> v)
    {
        cout << "[";
        for (double d : v)
        {
            cout << d << ", ";
        }
        cout << "]" << endl;
    }

    /**
     * Function for getting the mean of the elements of the `std::vectors`
     * inside an `std::map`.
     *
     * @param[in] Map The map containing the vectors.
     *
     * @return An (int, mean(vector)) map.
     */
    std::map<int, double> map_mean(std::map<int, std::vector<double>> Map, int N)
    {
        std::map<int, double> result;

        for (auto itr = Map.begin(); itr != Map.end(); itr++)
        {
            std::pair<int, double> p(itr->first, mean(itr->second));
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
}