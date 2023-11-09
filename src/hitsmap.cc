#include "hitsmap.hh"

// get singleton instance
MyHitsMap &MyHitsMap::Instance()
{
    static MyHitsMap instance;
    return instance;
}

// constructor
void MyHitsMap::InitializeMap(G4int N)
{
    HitsMap = new map<G4int, vector<G4double>>();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pair<G4int, vector<G4double>> p(i * N + j, vector<G4double>(1, 0.0));
            HitsMap->insert(p);
        }
    }
}

void MyHitsMap::AddEmptyColumn()
{
    for (auto itr = HitsMap->begin(); itr != HitsMap->end(); itr++)
    {
        itr->second.push_back(0.0);
    }
}

// add hit to map
void MyHitsMap::AddHit(pair<G4int, G4double> pair, G4int nRun)
{
    auto eItr = HitsMap->find(pair.first);
    if (eItr != HitsMap->end())
    {
        if (eItr->second.size() < nRun)
            eItr->second.push_back(pair.second);
        else
            eItr->second[nRun] += pair.second;
        eItr->second[nRun] += pair.second;
    }
}

// print map
void MyHitsMap::PrintMap() const
{
    if (HitsMap->empty())
    {
        G4cout << "Empty map!" << G4endl;
        return;
    }
    for (auto itr = HitsMap->begin(); itr != HitsMap->end(); itr++)
    {
        G4cout << itr->first << " : ";
        for (G4double energy : itr->second)
        {
            G4cout << energy << " ";
        }
        G4cout << G4endl;
    }
}
