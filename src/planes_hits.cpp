#include "planes_hits.hh"

#include "G4SystemOfUnits.hh"

#include "generator.hh"

G4ThreadLocal G4Allocator<PlanesHits> *PlanesHitsAllocator;

/**
 * The default constructor.
 */
PlanesHits::PlanesHits() : escaped(false) {}

/**
 * The copy constructor.
 */
PlanesHits::PlanesHits(const PlanesHits &otherHit) : G4VHit()
{
    escaped = otherHit.escaped;
}

/**
 * Overloading of the `=` operator.
 */
const PlanesHits &PlanesHits::operator=(const PlanesHits &otherHit)
{
    escaped = otherHit.escaped;

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

void PlanesHits::SetEscaped(G4int planeID, G4int parentID, G4double energy)
{
    if (planeID == 2 && !parentID && energy == PrimaryGenerator::GetEnergy())
        escaped = true;
}
