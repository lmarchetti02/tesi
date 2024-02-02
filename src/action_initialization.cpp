#include "action_initialization.hh"

#include "generator.hh"
#include "run_action.hh"
#include "event_action.hh"
#include "stepping_action.hh"

/**
 * The default constructor.
 */
ActionInitialization::ActionInitialization() {}

/**
 * Geant4 function for initializing the user action in the master thread.
 */
void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction);
}

/**
 * Geant4 function for initializing the user action in the worker threads.
 */
void ActionInitialization::Build() const
{
    // particle gun action
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);

    // run action
    RunAction *runAction = new RunAction;
    SetUserAction(runAction);

    // event action
    EventAction *eventAction = new EventAction(runAction, generator);
    SetUserAction(eventAction);

    // stepping action
    SetUserAction(new MySteppingAction(eventAction));
}
