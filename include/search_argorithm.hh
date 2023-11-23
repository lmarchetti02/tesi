#pragma once

#include <array>
#include <memory>
#include <vector>
#include <map>

#include "G4SystemOfUnits.hh"
#include "globals.hh"

using std::array;
using std::map;
using std::vector;

namespace search_algorithm
{

    G4int is_border(G4int ID, G4int nPixel)
    {
        G4int res = 0;

        if (ID % nPixel == 0)
            res += 10; // left border
        else if ((ID + 1) % nPixel == 0)
            res -= 10; // right border

        if (ID % nPixel == ID)
            res += 1; // bottom border
        else if (ID % (nPixel * nPixel) == ID)
            res -= -1; // top border
    }

    void contiguous_squares(G4int ID, G4int N)
    {
        vector<G4int> contiguousSquares;
        array<G4int, 8> moves = {1, N + 1, N, N - 1, -1, -N - 1, -N, -N + 1};
        // array<G4int, 8> movesB = {-1, -N - 1, -N, -N + 1, 1, N + 1, N, N - 1};

        enum nElements
        {
            C = 8,
            LB = 5,
            RB = 5,
            TB = 5,
            BB = 5,
            SEC = 3,
            SWC = 3,
            NEC = 3,
            NWC = 3
        };

        switch (is_border(ID, N))
        {
        case 0:
            for (G4int i = 0; i < C; i++)
            {
                contiguousSquares.push_back(ID += moves[i]);
            }
            break;
        case -10:
            for (G4int i = 0; i < LB; i++)
            {
                contiguousSquares.push_back(ID += moves[2 + i]);
            }
            break;
        case 10:
            for (G4int i = 0; i < RB; i++)
            {
                contiguousSquares.push_back(ID += moves[(6 + i) % 8]);
            }
            break;
        }
    }
}