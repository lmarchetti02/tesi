#pragma once

#include <vector>
#include <map>
#include <memory>

#include "globals.hh"
#include "G4AutoDelete.hh"

using std::map;
using std::pair;
using std::vector;

class MyHitsMap
{
private:
    map<G4int, vector<G4double>> *HitsMap;

protected:
    MyHitsMap() = default;

public:
    // get instance
    static MyHitsMap &Instance();

    // necessary for singleton
    MyHitsMap(const MyHitsMap &) = delete;
    MyHitsMap(MyHitsMap &&) = delete;
    MyHitsMap &operator=(const MyHitsMap &) = delete;
    MyHitsMap &operator=(MyHitsMap &&) = delete;
    ~MyHitsMap() { ; }

    void InitializeMap(G4int);
    void AddEmptyColumn();

    void AddHit(pair<G4int, G4double>, G4int);

    void PrintMap() const;

    void ClearMap() {}
};
