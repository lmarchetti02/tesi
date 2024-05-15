#pragma once

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"

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
class PixelsHit : public G4VHit
{
  public:
    PixelsHit();
    ~PixelsHit() override = default;
    PixelsHit(const PixelsHit &);

    const PixelsHit &operator=(const PixelsHit &);
    G4bool operator==(const PixelsHit &) const;

    void *operator new(size_t);
    void operator delete(void *);

  private:
    G4double energyDep;
    G4int detectorID;

  public:
    /**
     * Function for setting the `energyDep` data member.
     *
     * @param[in] eDep The energy deposition of the hit.
     */
    void SetEnergy(G4double eDep) { energyDep = eDep; }
    /**
     * Function for setting the `detectorID` data member.
     *
     * @param[in] ID The detector ID of the hit.
     */
    void SetID(G4int ID) { detectorID = ID; }
    /**
     * Function for accessing the `energyDep` data member.
     *
     * @return The energy deposition of the hit.
     */
    G4double GetEnergy() { return energyDep; }
    /**
     * Function for accessing the `detectorID` data member.
     *
     * @return The detector ID of the hit.
     */
    G4int GetID() { return detectorID; }
};

// Alias of the HitsCollection
typedef G4THitsCollection<PixelsHit> MyHitsCollection;

// Allocator of the Hits Class
extern G4ThreadLocal G4Allocator<PixelsHit> *MyHitAllocator;
