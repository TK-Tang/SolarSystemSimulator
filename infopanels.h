#ifndef INFOPANELS_H
#define INFOPANELS_H

#include "dialog.h"
#include "universecomponent.h"

#include <QKeyEvent>

class InfoPanels
{
public:
    InfoPanels();
    virtual ~InfoPanels();

    double m_clickX;
    double m_clickY;
    double m_scale;
    Dialog* m_dialog;

    void setInfo(Dialog* dialog, QMouseEvent* event);
    void cycle(UniverseComponent* root);
};

#endif // INFOPANELS_H
