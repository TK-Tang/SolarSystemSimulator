#include "dialog.h"
#include "ui_dialog.h"
#include "universecomposite.h"

#include <iostream>
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_width(1200)
    , m_height(1000)
    , m_paused(false)
    , m_renderZodiacs(true)
    , m_renderLabels(true)
    , m_timestamp(0)
    , m_config(Config::getInstance())
    , m_yOffSet(0)
    , m_xOffSet(0)
    , m_xDrift(0)
    , m_yDrift(0)
    , m_zoomDrift(0)
{
    m_config->read("config.txt");
    m_universe = m_config->parseUniverseBlocks();
    m_zodiacs = m_config->parseZodiacBlocks();
    m_universe->convertRelativeToAbsolute(0,0,0,0);

    //set backdrop to black
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    //make the window appear
    ui->setupUi(this);
    this->resize(m_width, m_height);

    //create the buttons
    m_buttonPause = new QPushButton("Pause", this);
    m_buttonZodiacs = new QPushButton("Zodiacs", this);
    m_buttonLabels = new QPushButton("Labels", this);
    m_buttonPause->setGeometry(QRect(QPoint(0, 0), QSize(75, 25)));
    m_buttonZodiacs->setGeometry(QRect(QPoint(75, 0), QSize(75, 25)));
    m_buttonLabels->setGeometry(QRect(QPoint(150, 0), QSize(75, 25)));
    connect(m_buttonPause, SIGNAL(released()), this, SLOT(togglePause()));
    connect(m_buttonZodiacs, SIGNAL(released()), this, SLOT(toggleZodiacs()));
    connect(m_buttonLabels, SIGNAL(released()), this, SLOT(toggleLabels()));

    //Display timestep
    //Qstring::number casts int to QString
    m_displayTimestep = new QPushButton(this);
    m_displayTimestep->setGeometry(QRect(QPoint(75,50),QSize(75,25)));
    m_displayTimestep->setText(QString::number(m_config->getPhysicsStepSize()));

    //Decrease timestep
    m_decreaseTimestep = new QPushButton(this);
    m_decreaseTimestep->setGeometry(QRect(QPoint(50,50),QSize(25,25)));
    m_decreaseTimestep->setText("x0.5");

    //Increase timestep
    m_increaseTimestep = new QPushButton(this);
    m_increaseTimestep->setGeometry(QRect(QPoint(150,50),QSize(25,25)));
    m_increaseTimestep->setText("x2.0");

    //Connect the button to increase the timestep
    connect(m_increaseTimestep, SIGNAL(released()), this, SLOT(increaseStep()));
    //Connect the button to decrease the timestep
    connect(m_decreaseTimestep, SIGNAL(released()), this, SLOT(decreaseStep()));
    //Connect buttot to set default values for physic step size
    connect(m_displayTimestep, SIGNAL(released()), this, SLOT(defaultTimeStep()));

    //Button for zooming out
    m_zoomOut = new QPushButton(this);
    m_zoomOut->setGeometry(QRect(QPoint(50,75),QSize(25,25)));
    m_zoomOut->setText("-");

    //Button for zooming in
    m_zoomIn= new QPushButton(this);
    m_zoomIn->setGeometry(QRect(QPoint(150,75),QSize(25,25)));
    m_zoomIn->setText("+");

    //Button for defaulting zoom scale and displaying zoom
    m_displayScale = new QPushButton(this);
    m_displayScale->setGeometry(QRect(QPoint(75,75),QSize(75,25)));
    m_displayScale->setText(QString::number(m_config->getDistanceScale()));

    //Connect button to zoom in method in Dialog
    connect(m_zoomIn, SIGNAL(released()), this, SLOT(slotZoomIn()));
    //Connect button to zoom out method in Dialog
    connect(m_zoomOut, SIGNAL(released()), this, SLOT(slotZoomOut()));
    //Connect button to default zoom
    connect(m_displayScale, SIGNAL(released()), this, SLOT(defaultZoom()));

    //Button for x coordinate
    m_x = new QPushButton(this);
    m_x->setGeometry(QRect(QPoint(50,125),QSize(62.5,25)));
    m_x->setText(QString::number(this->m_xOffSet));

    //Button for y coordinate
    m_y = new QPushButton(this);
    m_y->setGeometry(QRect(QPoint(112.5,125),QSize(62.5,25)));
    m_y->setText(QString::number(this->m_yOffSet));


    //Buttons for panning arrow pad
    m_north = new QPushButton(this);
    m_north->setGeometry(QRect(QPoint(100,150),QSize(25,25)));
    m_north->setText("↑");

    m_south = new QPushButton(this);
    m_south->setGeometry(QRect(QPoint(100,200),QSize(25,25)));
    m_south->setText("↓");

    m_west = new QPushButton(this);
    m_west->setGeometry(QRect(QPoint(75,175),QSize(25,25)));
    m_west->setText("←");

    m_east = new QPushButton(this);
    m_east->setGeometry(QRect(QPoint(125,175),QSize(25,25)));
    m_east->setText("→");

    //Connecting arrow keys to panning functions
    connect(m_north, SIGNAL(released()), this, SLOT(qNorth()));

    connect(m_south, SIGNAL(released()), this, SLOT(qSouth()));

    connect(m_east, SIGNAL(released()), this, SLOT(qEast()));

    connect(m_west, SIGNAL(released()), this, SLOT(qWest()));


    //Buttons for diagonal directions on the arrow pad
    m_northEast = new QPushButton(this);
    m_northEast->setGeometry(QRect(QPoint(125,150),QSize(25,25)));
    m_northEast->setText("↗");

    m_southEast = new QPushButton(this);
    m_southEast->setGeometry(QRect(QPoint(125,200),QSize(25,25)));
    m_southEast->setText("↘");

    m_northWest = new QPushButton(this);
    m_northWest->setGeometry(QRect(QPoint(75,150),QSize(25,25)));
    m_northWest->setText("↖");

    m_southWest = new QPushButton(this);
    m_southWest->setGeometry(QRect(QPoint(75,200),QSize(25,25)));
    m_southWest->setText("↙");

    //Connecting diagonal buttons to the diagonal arrows
    connect(m_northWest, SIGNAL(released()), this, SLOT(qNorthWest()));

    connect(m_northEast, SIGNAL(released()), this, SLOT(qNorthEast()));

    connect(m_southWest, SIGNAL(released()), this, SLOT(qSouthWest()));

    connect(m_southEast, SIGNAL(released()), this, SLOT(qSouthEast()));

    //Display tags for infomation panel
    m_name = new QPushButton(this);
    m_name->setGeometry(QRect(QPoint(m_width - 200, 0),QSize(100,25)));
    m_name->setText("Name: ");

    m_parentName = new QPushButton(this);
    m_parentName->setGeometry(QRect(QPoint(m_width - 200, 25),QSize(100,25)));
    m_parentName->setText("Parent Objects: ");

    m_mass = new QPushButton(this);
    m_mass->setGeometry(QRect(QPoint(m_width - 200, 50),QSize(100,25)));
    m_mass->setText("Mass: ");

    m_QX = new QPushButton(this);
    m_QX->setGeometry(QRect(QPoint(m_width - 200, 75),QSize(100,25)));
    m_QX->setText("X Position: ");

    m_QY = new QPushButton(this);
    m_QY->setGeometry(QRect(QPoint(m_width - 200, 100),QSize(100,25)));
    m_QY->setText("Y Position: ");

    //Display tags for infomation panel but with values
    m_nameData = new QPushButton(this);
    m_nameData->setGeometry(QRect(QPoint(m_width - 100, 0),QSize(100,25)));
    m_nameData->setText("");

    m_parentNameData = new QPushButton(this);
    m_parentNameData->setGeometry(QRect(QPoint(m_width - 100, 25),QSize(100,25)));
    m_parentNameData->setText("");

    m_massData = new QPushButton(this);
    m_massData->setGeometry(QRect(QPoint(m_width - 100, 50),QSize(100,25)));
    m_massData->setText("");

    m_QXData = new QPushButton(this);
    m_QXData->setGeometry(QRect(QPoint(m_width - 100, 75),QSize(100,25)));
    m_QXData->setText("");

    m_QYData = new QPushButton(this);
    m_QYData->setGeometry(QRect(QPoint(m_width - 100, 100),QSize(100,25)));
    m_QYData->setText("");




    //setup timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    pause(false);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_timer;
    delete m_buttonPause;
    delete m_buttonZodiacs;
    delete m_buttonLabels;
    delete m_universe;
    delete m_zodiacs;
}

void Dialog::toggleZodiacs()
{
    m_renderZodiacs = !m_renderZodiacs;
}

void Dialog::toggleLabels()
{
    m_renderLabels = !m_renderLabels;
}

void Dialog::togglePause()
{
    pause(!m_paused);
}

void Dialog::pause(bool pause)
{
    if(pause)
    {
        m_timer->stop();
        m_paused = true;
    }
    else
    {
        m_timer->start(1000 / m_config->getFramesPerSecond());
        m_paused = false;
    }
}

/*
 * Keyboard keys when pressed pans or zooms the window as well as pause.
 */
void Dialog::keyPressEvent(QKeyEvent *event)
{
    Zoom z;
    Pan move;
    switch(event->key()) {
    case Qt::Key_Space:
        pause(!m_paused);
        return;
    case Qt::Key_Minus:
        m_config->Config::zoomOut(&z);
        m_displayScale->setText(QString::number(m_config->getDistanceScale()));
        return;
    case Qt::Key_Plus:
        m_config->Config::zoomIn(&z);
        m_displayScale->setText(QString::number(m_config->getDistanceScale()));
        return;
    case Qt::Key_W:
        this->Dialog::panNorth(&move);
        return;
    case Qt::Key_S:
        this->Dialog::panSouth(&move);
        return;
    case Qt::Key_A:
        this->Dialog::panWest(&move);
        return;
    case Qt::Key_D:
        this->Dialog::panEast(&move);
        return;
    default:
        return;
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent* event){

    InfoPanels info;
    switch(event->button()) {
    case Qt::LeftButton:
        this->Dialog::info(&info, event);
        return;
    default:
        return;
    }
}

/*
 * Upon releasing keyboard presses - This calls methods and sets in place
 * values needed to create a drifting motion when panning and zooming in the window resulting
 * in a smooth movement navigating the simulation.
 */
void Dialog::keyReleaseEvent(QKeyEvent *e){

    Pan move;
    Zoom z;
    switch(e->key()){
    case Qt::Key_Minus:
        this->Dialog::zoomOutRelease(&z);
        return;
    case Qt::Key_Plus:
        this->Dialog::zoomInRelease(&z);
        return;
    case Qt::Key_W:
        this->Dialog::panNorthRelease(&move);
        return;
    case Qt::Key_S:
        this->Dialog::panSouthRelease(&move);
        return;
    case Qt::Key_A:
        this->Dialog::panWestRelease(&move);
        return;
    case Qt::Key_D:
        this->Dialog::panEastRelease(&move);
        return;
    default:
        return;
    }

}

void Dialog::nextFrame()
{
    //reset the forces stored in every object to 0
    m_universe->resetForces();

    //update the forces acting on every object in the universe,
    //from every object in the universe
    m_universe->addAttractionFrom(*m_universe);

    //update the velocity and position of every object in the universe
    int step = m_config->getPhysicsStepSize()/ m_config->getOvercalculatePhysicsAmount() ;
    for(int i = 0; i < m_config->getPhysicsStepSize(); i += step)
    {
        //update physics
        m_universe->updatePosition(step);
        //some time has passed
        m_timestamp += step;
    }

    //update the window (this will trigger paintEvent)
    update();
}

void Dialog::paintEvent(QPaintEvent *event){

    //Code chunk used to create a drifting motion on the X Axis
    if (m_xDrift > 0.5 || m_xDrift < -0.5){

        m_xOffSet = m_xOffSet + m_xDrift;
        m_xDrift = m_xDrift * 0.98;
        m_x->setText(QString::number(this->m_xOffSet));

    } else {

        m_xDrift = 0;
    }

    //Code chunk used to create a drifting motion on the Y Axis
    if (m_yDrift > 0.5 || m_yDrift < -0.5){

        m_yOffSet = m_yOffSet + m_yDrift;
        m_yDrift = m_yDrift * 0.98;
        m_y->setText(QString::number(this->m_yOffSet));

    } else {

        m_yDrift = 0;
    }

    // Code chunk used to create a drifting motion when zooming in and out
    if (m_zoomDrift > 1000 || m_zoomDrift < -1000){
        if ( m_config->getDistanceScale() > 3.5e+08){
            m_config->setDistanceScale(m_config->getDistanceScale() - m_zoomDrift);
            m_zoomDrift = m_zoomDrift * 0.8;
        }

    } else {

        m_zoomDrift  = 0;
    }


    //suppress 'unused variable' warning
    Q_UNUSED(event);

    //redraw the universe
    QPainter painter(this);

    //offset the painter so (0,0) is the center of the window
    painter.translate(m_width/2 + m_xOffSet, m_height/2 + m_yOffSet);

    if(m_renderZodiacs)
    {
        for(auto zodiac : *m_zodiacs)
        {
            zodiac.render(painter);
        }
    }

    m_universe->render(painter);

    if(m_renderLabels)
    {
        m_universe->renderLabel(painter);
    }
}

//Methods tied to increasing the time step
void Dialog::increaseStep(){
    Timestep step;
    Dialog::plusStep(&step);
}

void Dialog::plusStep(Timestep *step){
    step->upStep(this);
}

//Methods tied to decreasing the time step
void Dialog::decreaseStep(){
    Timestep step;
    Dialog::minusStep(&step);
}

void Dialog::minusStep(Timestep *step){
    step->downStep(this);
}

/*
 * Panning method in Dialog. It passes itself as a Dialog object to modify X and Y offset variables used for
 * panning around the map into the Pan class
 */
void Dialog::panNorth(Pan *move){
    move->Pan::north(this);
}

void Dialog::panSouth(Pan *move){
    move->Pan::south(this);
}

void Dialog::panEast(Pan *move){
    move->Pan::east(this);
}

void Dialog::panWest(Pan *move){
    move->Pan::west(this);
}

/*
 * Similar to above comments except this sets in values for drifting motion when releasing key presses
 */
void Dialog::panNorthRelease(Pan *move){
    move->Pan::northRelease(this);
}

void Dialog::panSouthRelease(Pan *move){
    move->Pan::southRelease(this);
}

void Dialog::panEastRelease(Pan *move){
    move->Pan::eastRelease(this);
}

void Dialog::panWestRelease(Pan *move){
    move->Pan::westRelease(this);
}

/*
 * Methods used to pass itself to a Zoom class to modify properties of the m_config
 * in the Dialog object for zooming in and out
 */
void Dialog::zoomInRelease(Zoom *z){
    z->zoomInRelease(this);
}

void Dialog::zoomOutRelease(Zoom *z){
    z->zoomOutRelease(this);
}

/*
 * Method used for zooming in and out when using UI buttons
 */
void Dialog::slotZoomIn(){
    Zoom z;
    this->Dialog::zoomInRelease(&z);

    //Update values in display buttons for distance scale
    m_displayScale->setText(QString::number(m_config->getDistanceScale()));
}

void Dialog::slotZoomOut(){
    Zoom z;
    this->Dialog::zoomOutRelease(&z);

    //Update values in display buttons for distance scale
    m_displayScale->setText(QString::number(m_config->getDistanceScale()));
}

// Method used to set default time step
void Dialog::defaultTimeStep(){
    m_config->setPhysicsStepSize(40320);
    m_displayTimestep->setText(QString::number(m_config->getPhysicsStepSize()));
}

//Method used to set camera zoom and panning back to default
void Dialog::defaultZoom(){
    this->setxOffSet(0);
    this->setyOffSet(0);
    m_config->setLogPointRadius(1e+06);
    m_config->setRadiusScale(1e+07);
    m_displayScale->setText(QString::number(m_config->getDistanceScale()));
    m_x->setText(QString::number(this->m_xOffSet));
    m_y->setText(QString::number(this->m_yOffSet));

    Snap s;
    m_config->setDistanceScale(s.snapZoom(this));
}

/*
 * Methods used for panning. These methods are connected the arrow keys.
 * They call key press methods used for panning - so they end up
 * passing Dialog object to the Pan class for  processing like a Visitor design pattern
 * (Seperating algorithms from objects itself)
 */
void Dialog::qNorth(){
    Pan move;
    this->panNorth(&move);
    this->Dialog::panNorthRelease(&move);
}

void Dialog::qSouth(){
    Pan move;
    this->panSouth(&move);
    this->Dialog::panSouthRelease(&move);
}

void Dialog::qWest(){
    Pan move;
    this->panWest(&move);
    this->Dialog::panWestRelease(&move);
}

void Dialog::qEast(){
    Pan move;
    this->panEast(&move);
    this->Dialog::panEastRelease(&move);
}

void Dialog::qNorthWest(){
    Pan move;
    this->panWest(&move);
    this->panNorth(&move);
    this->Dialog::panWestRelease(&move);
    this->Dialog::panNorthRelease(&move);
}

void Dialog::qNorthEast(){
    Pan move;
    this->panEast(&move);
    this->panNorth(&move);
    this->Dialog::panEastRelease(&move);
    this->Dialog::panNorthRelease(&move);

}

void Dialog::qSouthWest(){
    Pan move;
    this->panWest(&move);
    this->panSouth(&move);
    this->Dialog::panWestRelease(&move);
    this->Dialog::panSouthRelease(&move);

}

void Dialog::qSouthEast(){
    Pan move;
    this->panSouth(&move);
    this->panEast(&move);
    this->Dialog::panSouthRelease(&move);
    this->Dialog::panEastRelease(&move);

}

void Dialog::info(InfoPanels *info,QMouseEvent *event){
    info->InfoPanels::setInfo(this, event);
}
