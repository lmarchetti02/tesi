#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class MyHit : public G4VHit
{
public:
    MyHit();
    ~MyHit();
    MyHit(const MyHit &);
    const MyHit &operator=(const MyHit &);
    G4bool operator==(const MyHit &) const;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    void Print();

private:
    G4double energyDep; // Photon energy energy
    G4int detectorID;   // Position of the hit

public:
    // Set functions to store information on hits
    inline void SetEnergy(G4double fEn) { energyDep = fEn; }
    inline void SetID(G4int ID) { detectorID = ID; }

    // Get functions to access information on hits
    inline G4double GetEnergy() { return energyDep; }
    inline G4int GetID() { return detectorID; }
};

// define the type of hit collection
typedef G4THitsCollection<MyHit> MyHitsCollection;

// memory safety functions
extern G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;

inline void *MyHit::operator new(size_t)
{
    if (!MyHitAllocator)
        MyHitAllocator = new G4Allocator<MyHit>;
    return (void *)MyHitAllocator->MallocSingle();
}

inline void MyHit::operator delete(void *aHit)
{
    MyHitAllocator->FreeSingle((MyHit *)aHit);
}