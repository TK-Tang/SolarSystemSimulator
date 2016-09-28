#include "zoom.h"
#include "config.h"
#include <iostream>

Zoom::Zoom()
{

}

Zoom::~Zoom(){

}

//Method to zoom out - This also changes the render size of celestial objects
void Zoom::zoomOut(Config *config){


     config->setDistanceScale(config->getDistanceScale() + config->getDistanceScale() * 0.1);


     // The If statements are used to cap how small the planet/star can render to prevent zooming out into Oblivion
     // AKA the point where zooming in does not enlarge the object
     if (config->getRadiusScale() < 1.0e+08){

        config->setRadiusScale(config->getRadiusScale() +  config->getRadiusScale() * 0.5);
     }

     if(config->getLogPointRadius() < 1.0e+7){

        config->setLogPointRadius(config->getLogPointRadius()  + config->getLogPointRadius() * 0.5);
     }
}

//Method to zoom in - This also changes the render size of celestial objects
void Zoom::zoomIn(Config *config){

    //If statements to cap how close you can zoom in. Stops camera zooming in into oblivion
    // AKA the point where zooming out doesn't work.
    if (config->getDistanceScale() > 3.5e+08){

        config->setDistanceScale(config->getDistanceScale() - config->getDistanceScale() * 0.1);

    }

    // Caps the size of which the planet can enlarge to.
    if (config->getRadiusScale() > 1.0e+04){

        config->setRadiusScale(config->getRadiusScale() - config->getRadiusScale() * 0.5);
    }

    if (config->getLogPointRadius() > 1.0e+03){

        config->setLogPointRadius(config->getLogPointRadius() - config->getLogPointRadius() * 0.5);
    }
}

// Zoom methods for smooth drifting when zooming in and out
void Zoom::zoomInRelease(Dialog *dialog){
    double drift = dialog->getConfig()->getDistanceScale() * 0.1;
    dialog->setZoomDrift(drift);
}

void Zoom::zoomOutRelease(Dialog *dialog){
    double drift = dialog->getConfig()->getDistanceScale() * -0.1;
    dialog->setZoomDrift(drift);
}
