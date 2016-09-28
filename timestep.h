#ifndef TIMESTEP_H
#define TIMESTEP_H

#include "dialog.h"


class Timestep
{
public:
    Timestep();
    virtual ~Timestep();

    //Header methods for increasing/decreasing the time step
    void upStep(Dialog *dialog);
    void downStep(Dialog *dialog);
};

#endif // TIMESTEP_H
