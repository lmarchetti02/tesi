#include "action.hh"

MyActionInitialization::MyActionInitialization() {}
MyActionInitialization::~MyActionInitialization() {}

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

void MyActionInitialization::BuildForMaster() const
{
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}
