#include "hits.hh"

#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;

/**
 * The default constructor.
 */
MyHit::MyHit() : energyDep(0.), detectorID(0) {}

/**
 * The copy constructor.
 */
MyHit::MyHit(const MyHit &otherHit) : G4VHit()
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;
}

/**
 * Overloading of the `=` operator.
 */
const MyHit &MyHit::operator=(const MyHit &otherHit)
{
    energyDep = otherHit.energyDep;
    detectorID = otherHit.detectorID;

    return *this;
}

/**
 * Overloading of the `==` operator.
 */
G4bool MyHit::operator==(const MyHit &otherHit) const
{
    return (this == &otherHit) ? true : false;
}

/**
 * Overloading of the `new` operator
 */
void *MyHit::operator new(size_t)
{
    if (!MyHitAllocator)
        MyHitAllocator = new G4Allocator<MyHit>;
    return (void *)MyHitAllocator->MallocSingle();
}

/**
 * Overloading of the `new` operator
 */
void MyHit::operator delete(void *aHit)
{
    MyHitAllocator->FreeSingle((MyHit *)aHit);
}
