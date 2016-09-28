#include "timestep.h"

Timestep::Timestep(){

}

Timestep::~Timestep(){

}

//Doubles the timestep
//Updates the display
void Timestep::upStep(Dialog *dialog){
    dialog->getConfig()->setPhysicsStepSize( dialog->getConfig()->getPhysicsStepSize() * 2);
    dialog->getDisplayTimeStep()->setText(QString::number(dialog->getConfig()->getPhysicsStepSize()));
}

//Decreases the timestep by half
//Updates the display
void Timestep::downStep(Dialog *dialog){
    dialog->getConfig()->setPhysicsStepSize( (dialog->getConfig()->getPhysicsStepSize()) / 2);
    dialog->getDisplayTimeStep()->setText(QString::number(dialog->getConfig()->getPhysicsStepSize()));
}
