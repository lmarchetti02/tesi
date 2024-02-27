#include "hits.hh"

#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<PixelsHit> *MyHitAllocator;

/**
 * The default constructor.
 */
PixelsHit::PixelsHit() : energyDep(0.), detectorID(0) {}

/**
 * The copy constructor.
 */
PixelsHit::PixelsHit(const PixelsHit &otherHit) : G4VHit()
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;
}

/**
 * Overloading of the `=` operator.
 */
const PixelsHit &PixelsHit::operator=(const PixelsHit &otherHit)
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;

    return *this;
}

/**
 * Overloading of the `==` operator.
 */
G4bool PixelsHit::operator==(const PixelsHit &otherHit) const
{
    return (this == &otherHit) ? true : false;
}

/**
 * Overloading of the `new` operator
 */
void *PixelsHit::operator new(size_t)
{
    if (!MyHitAllocator)
        MyHitAllocator = new G4Allocator<PixelsHit>;
    return (void *)MyHitAllocator->MallocSingle();
}

/**
 * Overloading of the `new` operator
 */
void PixelsHit::operator delete(void *aHit)
{
    MyHitAllocator->FreeSingle((PixelsHit *)aHit);
}
