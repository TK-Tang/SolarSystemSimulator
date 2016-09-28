#ifndef PAN_H
#define PAN_H

#include "dialog.h"

class Dialog;

class Pan
{
public:
    Pan();
    virtual ~Pan();

    //Panning methods
    void north(Dialog* dialog);
    void south(Dialog* dialog);
    void east(Dialog* dialog);
    void west(Dialog* dialog);

    //Drifting methods for post-panning
    void northRelease(Dialog* dialog);
    void southRelease(Dialog* dialog);
    void westRelease(Dialog* dialog);
    void eastRelease(Dialog* dialog);
};

#endif // PAN_H
