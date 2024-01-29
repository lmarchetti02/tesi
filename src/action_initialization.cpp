#include "action_initialization.hh"

#include "generator.hh"
#include "run_action.hh"
#include "event_action.hh"
#include "stepping_action.hh"

/**
 * The default constructor.
 */
MyActionInitialization::MyActionInitialization() {}

/**
 * Geant4 function for initializing the user action in the master thread.
 */
void MyActionInitialization::BuildForMaster() const
{
    SetUserAction(new MyRunAction);
}

/**
 * Geant4 function for initializing the user action in the worker threads.
 */
void MyActionInitialization::Build() const
{
    // particle gun action
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    // run action
    MyRunAction *runAction = new MyRunAction;
    SetUserAction(runAction);

    // event action
    MyEventAction *eventAction = new MyEventAction(runAction, generator);
    SetUserAction(new MyEventAction(runAction));

    // stepping action
    SetUserAction(new MySteppingAction(eventAction));
}
