#include "hits.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<MyHit> *MyHitAllocator;

MyHit::MyHit()
{
    energyDep = 0.0;
    detectorID = 0;
}

MyHit::~MyHit() {}

MyHit::MyHit(const MyHit &right) : G4VHit()
{
    energyDep = right.energyDep;
    detectorID = right.detectorID;
}

const MyHit &MyHit::operator=(const MyHit &right)
{
    energyDep = right.energyDep;
    detectorID = right.detectorID;

    return *this;
}

G4bool MyHit::operator==(const MyHit &right) const
{
    return (this == &right) ? true : false;
}

void MyHit::Print()
{

    G4cout << energyDep / keV << G4endl;
    G4cout << detectorID / mm << G4endl;
}
