#include "pan.h"

#include <iostream>

Pan::Pan(){

}

Pan::~Pan(){

}

//Panning methods that take Dialog object, changes values and passes it back
void Pan::north(Dialog *dialog){
    dialog->setyOffSet(dialog->getyOffSet() + 10);
    dialog->getYQButton()->setText(QString::number(dialog->getyOffSet()));
}

void Pan::south(Dialog *dialog){
    dialog->setyOffSet(dialog->getyOffSet() - 10);
    dialog->getYQButton()->setText(QString::number(dialog->getyOffSet()));
}

void Pan::west(Dialog *dialog){
    dialog->setxOffSet(dialog->getxOffSet() + 10);
    dialog->getXQButton()->setText(QString::number(dialog->getxOffSet()));
}

void Pan::east(Dialog *dialog){
    dialog->setxOffSet(dialog->getxOffSet() - 10);
    dialog->getXQButton()->setText(QString::number(dialog->getxOffSet()));
}

//Drift panning methods that take a Dialog object, sets a drift motion and passes it back
void Pan::northRelease(Dialog *dialog){
    dialog->setyDrift(4);
}

void Pan::southRelease(Dialog *dialog){
    dialog->setyDrift(-4);
}

void Pan::westRelease(Dialog *dialog){
    dialog->setxDrift(4);
}

void Pan::eastRelease(Dialog *dialog){
    dialog->setxDrift(-4);
}




