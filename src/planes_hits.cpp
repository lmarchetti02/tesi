#include "planes_hits.hh"

#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<PlanesHits> *PlanesHitsAllocator;

/**
 * The default constructor.
 */
PlanesHits::PlanesHits() : energyDep(0.), detectorID(0) {}

/**
 * The copy constructor.
 */
PlanesHits::PlanesHits(const PlanesHits &otherHit) : G4VHit()
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;
}

/**
 * Overloading of the `=` operator.
 */
const PlanesHits &PlanesHits::operator=(const PlanesHits &otherHit)
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;

    return *this;
}

/**
 * Overloading of the `==` operator.
 */
G4bool PlanesHits::operator==(const PlanesHits &otherHit) const
{
    return (this == &otherHit) ? true : false;
}

/**
 * Overloading of the `new` operator
 */
void *PlanesHits::operator new(size_t)
{
    if (!PlanesHitsAllocator)
        PlanesHitsAllocator = new G4Allocator<PlanesHits>;
    return (void *)PlanesHitsAllocator->MallocSingle();
}

/**
 * Overloading of the `new` operator
 */
void PlanesHits::operator delete(void *aHit)
{
    PlanesHitsAllocator->FreeSingle((PlanesHits *)aHit);
}
