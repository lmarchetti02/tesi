#include "action.hh"

MyActionInitialization::MyActionInitialization() {}

/**
 * Geant4 function for initializing the user action in the master thread.
 */
void MyActionInitialization::Build() const
{
    // particle gun action
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    // run action
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);

    // event action
    MyEventAction *eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);
}

/**
 * Geant4 function for initializing the user action in the worker threads.
 */
void MyActionInitialization::BuildForMaster() const
{
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}
