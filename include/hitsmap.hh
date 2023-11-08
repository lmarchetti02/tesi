#pragma once

#include <vector>
#include <map>

#include "globals.hh"

using std::map;
using std::pair;
using std::vector;

class MyHitsMap
{
private:
    map<G4int, vector<G4double>> HitsMap;

public:
    MyHitsMap(G4int);
    ~MyHitsMap();

    void PrintMap() const;
};
