#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

/**
 * Custom Hits Class
 * ---
 *
 * It defines a custom type of hit, inheriting from the builtin `G4Hit`.
 * For each step in the sensitive detector, a hit object is created and
 * stored in a HitsCollection. The `PlanesHits` object contains information about the
 * step, which can be customized in this class.
 *
 * In particular, for each hit, the information saved is:
 *  - the deposited energy of the step;
 *  - the ID of the detector inside which the step has been created.
 */
class PlanesHits : public G4VHit
{
public:
    PlanesHits();
    ~PlanesHits() override = default;
    PlanesHits(const PlanesHits &);

    const PlanesHits &operator=(const PlanesHits &);
    G4bool operator==(const PlanesHits &) const;

    void *operator new(size_t);
    void operator delete(void *);

private:
    G4bool escaped;

public:
    G4bool GetEscaped() const { return escaped; }
    void SetEscaped(G4int planeID, G4int parentID, G4double energy);
};

// Alias of the HitsCollection
typedef G4THitsCollection<PlanesHits> PlanesHitsCollection;

// Allocator of the Hits Class
extern G4ThreadLocal G4Allocator<PlanesHits> *PlanesHitsAllocator;
