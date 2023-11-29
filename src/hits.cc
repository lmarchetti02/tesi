#include "hits.hh"

#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;

MyHit::MyHit()
{
    energyDep = 0.0;
    detectorID = 0;
}

MyHit::~MyHit() {}

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

/**
 * Function for setting the `energyDep` data member.
 *
 * @param[in] eDep The energy deposition of the hit.
 */
void MyHit::SetEnergy(G4double eDep)
{
    energyDep = eDep;
}

/**
 * Function for setting the `detectorID` data member.
 *
 * @param[in] eDep The detector ID of the hit.
 */
void MyHit::SetID(G4int ID)
{
    detectorID = ID;
}

/**
 * Function for accessing the `energyDep` data member.
 *
 * @return The energy deposition of the hit.
 */
G4double MyHit::GetEnergy()
{
    return energyDep;
}

/**
 * Function for accessing the `detectorID` data member.
 *
 * @return The detector ID of the hit.
 */
G4int MyHit::GetID()
{
    return detectorID;
}
