#include "snap.h"
#include <iostream>
#include <cmath>
#include <math.h>

Snap::Snap()
{

}

Snap::~Snap(){

}

double Snap::snapZoom(Dialog *dialog){

    m_scale = 0;

    list(dialog->getUniverse());

    return m_scale / 500;
}

//Iterate through the tree of celestial objects
//[!!!] I DID NOT WRITE THIS CODE - I MORE OR LESS GOT THIS FROM AN EDLMS THREAD BY JIA LIU [!!!]
void Snap::list(UniverseComponent* root){
    if (UniverseComposite* parent = dynamic_cast<UniverseComposite*>(root)){
        for ( UniverseComponent* child : parent->getChildren()){
            list(child);
        }
    } else if (UniverseBody* body = dynamic_cast<UniverseBody*>(root)){
        if (body->getType() != star){
            if (m_scale < pow(pow(std::abs(body->getPositionX()), 2.0) + pow(std::abs(body->getPositionY()), 2.0), 0.5)){
                m_scale = pow(pow(std::abs(body->getPositionX()), 2.0) + pow(std::abs(body->getPositionY()), 2.0), 0.5);
            }
        }
    }
}

