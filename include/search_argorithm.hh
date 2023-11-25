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

        return res;
    }

    vector<G4int> contiguous_squares(G4int ID, G4int N)
    {
        vector<G4int> contiguousSquares;
        array<G4int, 8> moves = {1, N + 1, N, N - 1, -1, -N - 1, -N, -N + 1};

        enum nElements
        {
            CENTER = 8,
            BORDER = 5,
            CORNER = 3
        };

        switch (is_border(ID, N))
        {
        case 0: // center
            for (G4int i = 0; i < CENTER; i++)
            {
                contiguousSquares.push_back(ID += moves[i]);
            }
            break;
        case -10: // right border
            for (G4int i = 0; i < BORDER; i++)
            {
                contiguousSquares.push_back(ID += moves[2 + i]);
            }
            break;
        case 10: // left border
            for (G4int i = 0; i < BORDER; i++)
            {
                contiguousSquares.push_back(ID += moves[(6 + i) % 8]);
            }
            break;
        case 1: // bottom border
            for (G4int i = 0; i < BORDER; i++)
            {
                contiguousSquares.push_back(ID += moves[i]);
            }
            break;
        case -1: // top border
            for (G4int i = 0; i < BORDER; i++)
            {
                contiguousSquares.push_back(ID += moves[(4 + i) % 8]);
            }
            break;
        case 11: // SW corner
            for (G4int i = 0; i < CORNER; i++)
            {
                contiguousSquares.push_back(ID += moves[i]);
            }
            break;
        case -11: // NE corner
            for (G4int i = 0; i < CORNER; i++)
            {
                contiguousSquares.push_back(ID += moves[4 + i]);
            }
            break;
        case -9: // SE corner
            for (G4int i = 0; i < CORNER; i++)
            {
                contiguousSquares.push_back(ID += moves[2 + i]);
            }
            break;
        case 9: // NW corner
            for (G4int i = 0; i < CORNER; i++)
            {
                contiguousSquares.push_back(ID += moves[6 + i] % 8);
            }
            break;
        default:
            break;
        }

        return contiguousSquares;
    }

    map<G4int, array<G4double, 2>> remaining_squares(const map<G4int, array<G4double, 2>> &pixels, const vector<G4int> &contiguous_squares, G4int first)
    {
        auto pixels_new = pixels;

        pixels_new.erase(first);

        for (G4int ID : contiguous_squares)
        {
            pixels_new.erase(ID);
        }

        return pixels_new;
    }
}