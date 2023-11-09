#include "run.hh"
#include "construction.hh"

MyRunAction::MyRunAction() {}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run *run)
{
    // MyHitsMap::Instance().PrintMap();
}

void MyRunAction::EndOfRunAction(const G4Run *)
{
}
