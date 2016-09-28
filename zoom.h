#ifndef ZOOM_H
#define ZOOM_H

#include "config.h"
#include "alter.h"
#include "dialog.h"

class Dialog;

class Zoom : public Alter{

    public:
        Zoom();
        virtual ~Zoom();

        //Zooming in and out methods that take a Config object for processing
        void zoomOut(Config *config);
        void zoomIn(Config *config);

        //Drift methods for zooming
        void zoomInRelease(Dialog *dialog);
        void zoomOutRelease(Dialog *dialog);
   };


#endif // ZOOM_H
