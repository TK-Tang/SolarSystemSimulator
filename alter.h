#ifndef ZOOMSTRATEGY_H
#define ZOOMSTRATEGY_H

#include "config.h"

class Config;

class Alter{
public:

    virtual void zoomOut(Config *config) = 0;
    virtual void zoomIn(Config *config) = 0;

};

#endif // ZOOMSTRATEGY_H
