#ifndef DIALOG_H
#define DIALOG_H

#include "config.h"
#include "universecomponent.h"
#include "zodiac.h"
#include "pan.h"
#include "zoom.h"
#include "timestep.h"
#include "snap.h"
#include "infopanels.h"

#include <QDialog>
#include <QTimer>
#include <list>
#include <QInputDialog>
#include <QtGui>
#include <QPushButton>

class Pan;
class Snap;
class Zoom;
class Config;
class Timestep;
class InfoPanels;

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    virtual ~Dialog();

    Config* getConfig(){ return m_config; }

    UniverseComponent* getUniverse(){ return  m_universe; }

    int getWidth(){ return m_width; }
    int getHeight(){ return m_height; }

    // [!] Pan NSWE [!]
    // Calls panning methods in Pan class
    void panNorth(Pan *pan);

    void panSouth(Pan *pan);

    void panEast(Pan *pan);

    void panWest(Pan *pan);

    // Panning methods in Dialog that call the panning methods in the Pan class

    void panNorthRelease(Pan *pan);

    void panSouthRelease(Pan *pan);

    void panWestRelease(Pan *pan);

    void panEastRelease(Pan *pan);

    // [!] Offsets used to change the centre of view [!]
    // Getters and setter methods for the offsets
    // x + xOffSet = centre of view changed
    // y + yOffSet = centre of view changed
    // Centre of view rendered in the Dialog class in painter.translate method

    double getxOffSet(){ return m_xOffSet; }

    double getyOffSet(){ return m_yOffSet; }

    void setxOffSet(double x){ m_xOffSet = x; }

    void setyOffSet(double y){ m_yOffSet = y; }

    // Setters for pan drifting variables

    void setxDrift(double x){ m_xDrift = x; }

    void setyDrift(double y){ m_yDrift = y; }

    double getxDrift(){ return m_xDrift; }

    double getyDrift(){ return m_yDrift; }

    // Releasing zoom

    void zoomInRelease(Zoom *z);

    void zoomOutRelease(Zoom *z);

    void setZoomDrift( double drift ) { m_zoomDrift = drift; }

    //timestep changing methods

    void plusStep(Timestep *step);

    void minusStep(Timestep *step);

    //Getter for updating timesteps

    QPushButton* getDisplayTimeStep(){ return m_displayTimestep; }

    //Getter for updating X and Y centre of view coordinates on QButtons
    QPushButton* getXQButton(){ return m_x; }
    QPushButton* getYQButton(){ return m_y; }

    //Getters for infopanel buttons
    QPushButton* getNameData() { return m_nameData; }
    QPushButton* getParentNameData() { return m_parentNameData; }
    QPushButton* getMassData() { return m_massData; }
    QPushButton* getXData() { return m_QXData; }
    QPushButton* getYData() { return m_QYData; }

    void info(InfoPanels* info, QMouseEvent* event);


private slots:
    //prepare the next frame, called by the timer
    void nextFrame();
    //toggle the simultation's pause state
    void togglePause();
    //toggle rendering of Zodiacs
    void toggleZodiacs();
    //toggle rendering of labels
    void toggleLabels();

    //increase timestep
    void increaseStep();
    //decrease timestep
    void decreaseStep();
    //method for defaulting physics step size
    void defaultTimeStep();

    //method for zooming in tied to QPushButton
    void slotZoomIn();
    //method for zooming in tied to QPushButton
    void slotZoomOut();
    //method for defaulting zoom scale
    void defaultZoom();

    //method for binding arrow buttons to panning
    void qNorth();
    void qSouth();
    void qWest();
    void qEast();
    void qNorthWest();
    void qNorthEast();
    void qSouthWest();
    void qSouthEast();

private:
    //method called when the window is being redrawn
    void paintEvent(QPaintEvent *event);
    //pause (or unpause) the simulation
    void pause(bool pause);
    //handle key presses
    void keyPressEvent(QKeyEvent *event);
    //handle key release events
    void keyReleaseEvent(QKeyEvent *e);
    //handle for mouse click event
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Dialog* ui;
    QTimer* m_timer; //Timer object for triggering updates

    //buttons for UI
    QPushButton* m_buttonPause;
    QPushButton* m_buttonZodiacs;
    QPushButton* m_buttonLabels;

    //more buttons for calibrating time step
    QPushButton* m_increaseTimestep;
    QPushButton* m_decreaseTimestep;
    QPushButton* m_displayTimestep;

    //moar buttons for zooming in and out
    QPushButton* m_zoomOut;
    QPushButton* m_zoomIn;
    QPushButton* m_displayScale;

    //even moar buttons for displaying coordinates
    QPushButton* m_x;
    QPushButton* m_y;

    //so many moar buttons for panning
    QPushButton* m_north;
    QPushButton* m_south;
    QPushButton* m_west;
    QPushButton* m_east;
    QPushButton* m_northWest;
    QPushButton* m_northEast;
    QPushButton* m_southWest;
    QPushButton* m_southEast;

    //Static label display for planet data when clicked
    QPushButton* m_name;
    QPushButton* m_mass;
    QPushButton* m_parentName;
    QPushButton* m_QX;
    QPushButton* m_QY;

    //Actual data being displayed for planet data
    QPushButton* m_nameData;
    QPushButton* m_massData;
    QPushButton* m_parentNameData;
    QPushButton* m_QXData;
    QPushButton* m_QYData;

    int m_width; //width of the window
    int m_height; //height of the window
    bool m_paused; //is the simulation paused?
    bool m_renderZodiacs; //should Zodiacs be rendered?
    bool m_renderLabels; //should labels be rendered?
    long m_timestamp; //simulation time since simulation start
    UniverseComponent* m_universe; //The universe
    std::list<Zodiac>* m_zodiacs; //Vector of zodiac lines
    Config* m_config; //the singleton config instance

    //Offset values for panning around the simulation
    double m_yOffSet;
    double m_xOffSet;

    //Drift values for creating smooth movement when moving around the simulation
    double m_xDrift;
    double m_yDrift;

    //Drift values for creating smooth movement when zooming in and out
    double m_zoomDrift;

};

#endif // DIALOG_H
