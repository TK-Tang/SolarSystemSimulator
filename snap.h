#ifndef SNAP_H
#define SNAP_H

#include "dialog.h"
#include "universecomponent.h"
#include "universecomposite.h"

class Snap
{
public:
    Snap();
    virtual ~Snap();

    double m_scale;

    double snapZoom(Dialog *dialog);
    void list(UniverseComponent* root);
};

#endif // SNAP_H
