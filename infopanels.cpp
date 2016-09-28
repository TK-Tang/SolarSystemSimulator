#include "infopanels.h"

#include <iostream>
#include <math.h>
#include <cmath>

InfoPanels::InfoPanels(){

}

InfoPanels::~InfoPanels(){

}

void InfoPanels::setInfo(Dialog *dialog, QMouseEvent *event){

    m_clickX = dialog->getWidth() / 2 - event ->x();
    m_clickY = dialog->getHeight() / 2 - event->y();

    m_scale = dialog->getConfig()->getDistanceScale() * -1;
    m_dialog = dialog;

    cycle(dialog->getUniverse());
}

void InfoPanels::cycle(UniverseComponent* root){
    if (UniverseComposite* parent = dynamic_cast<UniverseComposite*>(root)){
        for ( UniverseComponent* child : parent->getChildren()){
            cycle(child);
        }
    } else if (UniverseBody* body = dynamic_cast<UniverseBody*>(root)){

          if(abs(body->getPositionX()/m_scale - m_clickX) < 10 && abs(body->getPositionY()/m_scale - m_clickY) < 10){


            m_dialog->getNameData()->setText(QString::fromStdString(body->getName()));
            m_dialog->getParentNameData()->setText(QString::fromStdString(body->getParentName()));
            m_dialog->getMassData()->setText(QString::number(body->getMass()));
            m_dialog->getXData()->setText(QString::number(body->getPositionX()));
            m_dialog->getYData()->setText(QString::number(body->getPositionY()));
        }
    }
}
