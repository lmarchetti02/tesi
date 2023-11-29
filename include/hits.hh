#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/**
 * Custom Hits Class
 * ---
 *
 * It defines a custom type of hit, inheriting from the builtin `G4Hit`.
 * For each step in the sensitive detector, a hit object is created and
 * stored in a HitsCollection. The `MyHit` object contains information about the
 * step, which can be customized in this class.
 *
 * In particular, for each hit, the information saved is:
 *  - the deposited energy of the step;
 *  - the ID of the detector inside which the step has been created.
 */
class MyHit : public G4VHit
{
public:
    MyHit();
    ~MyHit();
    MyHit(const MyHit &);

    const MyHit &operator=(const MyHit &);
    G4bool operator==(const MyHit &) const;

    void *operator new(size_t);
    void operator delete(void *);

private:
    G4double energyDep;
    G4int detectorID;

public:
    void SetEnergy(G4double);
    void SetID(G4int ID);

    G4double GetEnergy();
    G4int GetID();
};

// Alias of the HitsCollection
typedef G4THitsCollection<MyHit> MyHitsCollection;

// Allocator of the Hits Class
extern G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;
