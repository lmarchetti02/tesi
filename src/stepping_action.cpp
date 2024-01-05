#include "stepping_action.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "event_action.hh"
#include "detector_construction.hh"

/**
 * The constructor.
 *
 * @param[in] eventAction The pointer to the event action instance.
 */
SteppingAction::SteppingAction(MyEventAction *eventAction) : eventAction(eventAction),
                                                             scoringVolume(nullptr),
                                                             scoringPlane(nullptr)
{
}

/**
 * Geant4 function for analyzing the steps of the events.
 *
 * @param[in] step The pointer to the current step.
 */
void SteppingAction::UserSteppingAction(const G4Step *step)
{
    // get scoring volumes
    if (!scoringVolume)
    {
        const auto detConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        scoringVolume = detConstruction->GetScoringVolume();
        scoringPlane = detConstruction->GetScoringPlane();
    }

    // get volume of the current step
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    if (volume == scoringPlane)
    {
        G4Track *track = step->GetTrack();
        if (track->GetParticleDefinition()->GetParticleName() == "gamma" && track->GetTrackID() != 1)
            eventAction->AddEnergyEscape(track->GetTotalEnergy() / MeV);
    }

    // check if we are in scoring volume
    if (volume != scoringVolume)
        return;
}
