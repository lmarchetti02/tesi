#include "hitsmap.hh"

// constructor
MyHitsMap::MyHitsMap(G4int N)
{
    for (int i = 0; i < N; i++)
    {
        pair<G4int, vector<G4double>> p(i, vector<G4double>(1, 0.0));
        HitsMap.insert(p);
    }
}

MyHitsMap::~MyHitsMap() {}

void MyHitsMap::PrintMap() const
{
    for (auto itr = HitsMap.begin(); itr != HitsMap.end(); itr++)
    {
        G4cout << itr->first << " : ";
        for (G4double energy : itr->second)
        {
            G4cout << energy << " ";
        }
        G4cout << G4endl;
    }
}
