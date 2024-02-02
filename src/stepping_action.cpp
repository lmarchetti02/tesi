// PROF. MONACO

#include "stepping_action.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"

#include "event_action.hh"
#include "detector_construction.hh"

/**
 * The constructor.
 *
 * @param[in] eventAction The pointer to the event action instance.
 */
MySteppingAction::MySteppingAction(EventAction *eventAction) : eventAction(eventAction),
                                                                 scoringVolume(nullptr),
                                                                 scoringPlane(nullptr) {}

/**
 * Geant4 function for analyzing the steps of the events.
 *
 * @param[in] step The pointer to the current step.
 */
void MySteppingAction::UserSteppingAction(const G4Step *step)
{

    // get scoring volumes
    if (!scoringVolume)
    {
        const auto detConstruction = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        scoringVolume = detConstruction->GetScoringVolume();
        scoringPlane = detConstruction->GetScoringPlane();
    }

    // get volume of the current step
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4Track *track = step->GetTrack();
    G4double ene = track->GetTotalEnergy();

    // to take track of emission of fluorescent photons
    if (volume == scoringVolume)
    {
        G4double eneloss = step->GetTotalEnergyDeposit();
        const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
        G4int copyNo = touchable->GetCopyNumber();
        G4StepPoint *preStepPoint = step->GetPreStepPoint();
        const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();
        int compt = 0;
        if (CurrentProcess != 0)
        {
            const G4String &StepProcessName = CurrentProcess->GetProcessName();
            if (StepProcessName == "compt" && track->GetParticleDefinition()->GetParticleName() == "gamma")
            {
                compt = 1;
                eventAction->AddComptonInfo(track->GetTrackID(), track->GetParentID(), 0, copyNo, ene, eneloss, StepProcessName);
            }
        }

        if (track->GetParticleDefinition()->GetParticleName() == "gamma" && !compt)
        {
            if (track->GetParentID() == 1)
                eventAction->AddStepInfo(track->GetTrackID(), track->GetParentID(), 0, copyNo, ene, eneloss);
            else if (track->GetTrackID() == 1)
            {
                eventAction->SetInitialPhoton(copyNo, ene);
            }
        }
        else if (track->GetParticleDefinition()->GetParticleName() != "gamma")
        {
            eventAction->AddComptonInfo(track->GetTrackID(), track->GetParentID(), 1, copyNo, ene, eneloss, "");
            eventAction->AddStepInfo(track->GetTrackID(), track->GetParentID(), 1, copyNo, ene, eneloss);
        }
    }

    if (volume == scoringPlane)
    {

        const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
        G4int copyNo = touchable->GetCopyNumber();
        if (track->GetParticleDefinition()->GetParticleName() == "gamma" && track->GetTrackID() != 1)
        {
            eventAction->AddStepInfo(track->GetTrackID(), track->GetParentID(), -1, -1, track->GetTotalEnergy(), 0.);
        }
        if (track->GetParticleDefinition()->GetParticleName() == "gamma" && track->GetTrackID() == 1)
        {
            eventAction->AddStepInfo(track->GetTrackID(), track->GetParentID(), -2, -2, track->GetTotalEnergy(), 0.);
            if (copyNo == 2)
                eventAction->SetInitialPhoton(copyNo, ene);
        }
    }
}
