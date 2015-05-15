#include "Core/mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/AbstractMaterialWidget.h"
#include "Core/MaterialLibrary.h"
#include "UI/MeshWidget.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QGLFormat format;
    format.setVersion(4,1);
    format.setProfile(QGLFormat::CoreProfile);

    m_openGLWidget = new OpenGLWidget(format,this);
    ui->gridLayout->addWidget(m_openGLWidget,0,1,2,2);

    createMenus();

    connect(m_openGLWidget,SIGNAL(pathTracerCreated()),this,SLOT(createMenus()));

}

MainWindow::~MainWindow(){

    //delete our singlton classes
    AbstractMaterialWidget::getInstance()->destroy();
    MaterialLibrary::getInstance()->destroy();
    // delete mesh UI
    delete m_meshToolbarButton;

    // delete light UI
    delete m_lightIntensityLabel;
    delete m_lightIntensitySlider;
    delete m_lightColourButton;
    delete m_lightColourDialog;
    delete m_lightTypeComboBox;
    delete m_lightTypeLabel;
    delete m_lightDockGridLayout;
    delete m_lightWidget;
    delete m_lightDockWidget;
    delete m_lightToolbarButton;

    // delete environment map UI
    delete m_environmentToolbarButton;
    delete m_environmentButton;
    delete m_environmentLineEdit;
    delete m_environmentGridLayout;
    delete m_environmentGroupBox;
    delete m_environmentDockWidget;

    delete m_fileMenu;

    delete m_saveImage;
    delete m_renderMenu;
    delete m_settingsMenu;

    delete m_menuBar;

    delete ui;

    delete m_openGLWidget;
}

void MainWindow::createMenus(){
    //--------------------------------------------------------------------------------------------------------------------
    //----------------------------Create our node graph widget instance---------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------

    //init our instance with this as the parent. this means when this class is deleted it also will be deleted
    AbstractMaterialWidget::getInstance(this)->hide();
    //init our materail library
    MaterialLibrary::getInstance()->hide();


    // A toolbar used to hold the button associated with different elements in the scene e.g. lighting, mesh options
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setOrientation(Qt::Vertical);
    ui->gridLayout->addWidget(toolBar, 0, 0, 2, 1);


    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Light functionality------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    QPixmap light("icons/light.png");
    QIcon lightBtnIcon(light);
    m_lightToolbarButton = new QToolButton();
    m_lightToolbarButton->setIcon(lightBtnIcon);
    m_lightToolbarButton->setToolTip("Light options");
    toolBar->addWidget(m_lightToolbarButton);
    toolBar->addSeparator();

    // Create the dock for the light options
    m_lightWidget = new QWidget();
    m_lightDockGridLayout = new QGridLayout();
    m_lightWidget->setLayout(m_lightDockGridLayout);
    m_lightDockWidget = new QDockWidget("Light Attributes");

    // Creae a drop down menu for the ligth type
    m_lightTypeLabel = new QLabel("Type:");
    m_lightTypeComboBox = new QComboBox();

    m_lightTypeComboBox->setMinimumWidth(150);
    m_lightTypeComboBox->addItem("Ambient Light");
    m_lightTypeComboBox->addItem("Area Light");
    m_lightTypeComboBox->addItem("Directional Light");
    m_lightTypeComboBox->addItem("Point Light");
    m_lightTypeComboBox->addItem("Spot Light");

    m_lightColourDialog = new QColorDialog();
    m_lightColourDialog->setHidden(true);
    m_lightColourButton = new QPushButton("Colour");
    m_lightIntensityLabel = new QLabel("Intensity:");
    m_lightIntensitySlider = new QSlider();
    m_lightIntensitySlider->setOrientation(Qt::Horizontal);

    // Add widgets to the light dock
    m_lightDockGridLayout->addWidget(m_lightTypeLabel, 0, 0, 1, 1);
    m_lightDockGridLayout->addWidget(m_lightTypeComboBox, 0, 1, 1, 1);
    m_lightDockGridLayout->addWidget(m_lightColourButton, 1, 0, 1, 2);
    m_lightDockGridLayout->addWidget(m_lightColourDialog);
    m_lightDockGridLayout->addWidget(m_lightIntensityLabel, 2, 0, 1, 1);
    m_lightDockGridLayout->addWidget(m_lightIntensitySlider, 2, 1, 1, 1);

    // Add a spacer on the bottom
    m_lightSpacer = new QSpacerItem(1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_lightDockGridLayout->addItem(m_lightSpacer, 3, 0, 1, 1);
    m_lightDockWidget->setWidget(m_lightWidget);
    m_lightDockWidget->setHidden(true);
//    this->addDockWidget(Qt::RightDockWidgetArea, m_lightDockWidget);



    LightManager::getInstance()->setHidden(true);
    this->addDockWidget(Qt::RightDockWidgetArea, LightManager::getInstance());
    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Mesh functionality-------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    // Button to access the mesh specific parameters
    QPixmap mesh("icons/teapot.png");
    QIcon meshBtnIcon(mesh);
    m_meshToolbarButton = new QToolButton();
    m_meshToolbarButton->setIcon(meshBtnIcon);
    m_meshToolbarButton->setToolTip("Mesh options");
    toolBar->addWidget(m_meshToolbarButton);
    toolBar->addSeparator();

    MeshWidget *meshWgt = new MeshWidget(this);
    meshWgt->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, meshWgt);
    connect(meshWgt,SIGNAL(updateScene()),m_openGLWidget,SLOT(sceneChanged()));
    connect(m_meshToolbarButton, SIGNAL(clicked()), meshWgt, SLOT(show()));

    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Environment map----------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    // Set up toolbar button
    QPixmap environ("icons/environment.png");
    QIcon environBtnIcon(environ);
    m_environmentToolbarButton = new QToolButton();
    m_environmentToolbarButton->setIcon(environBtnIcon);
    m_environmentToolbarButton->setToolTip("Environment Map Options");
    toolBar->addWidget(m_environmentToolbarButton);
    toolBar->addSeparator();

    // Set up widget
    m_environmentDockWidget = new QDockWidget();
    m_environmentDockWidget->setWindowTitle("Environment Map Attributes");
    m_environmentDockWidget->setHidden(true);
    m_environmentGroupBox = new QGroupBox();
    m_environmentGroupBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_environmentDockWidget->setWidget(m_environmentGroupBox);
    m_environmentGridLayout = new QGridLayout();
    m_environmentGroupBox->setLayout(m_environmentGridLayout);
    m_environmentLineEdit = new QLineEdit();
    m_environmentGridLayout->addWidget(m_environmentLineEdit, 0, 0, 1, 1);
    m_environmentButton = new QPushButton("Load");
    m_environmentGridLayout->addWidget(m_environmentButton, 0, 1, 1, 1);
    this->addDockWidget(Qt::RightDockWidgetArea, m_environmentDockWidget);

    //--------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------------Connections-------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    connect(m_lightToolbarButton, SIGNAL(clicked(bool)), m_lightToolbarButton, SLOT(setChecked(bool)));
//    connect(m_lightToolbarButton, SIGNAL(clicked()), m_lightDockWidget, SLOT(show()));
//    connect(m_lightColourButton, SIGNAL(clicked()), m_lightColourDialog, SLOT(show()));
    connect(m_lightToolbarButton, SIGNAL(clicked()), LightManager::getInstance(), SLOT(show()));


    connect(m_meshToolbarButton, SIGNAL(clicked(bool)), m_meshToolbarButton,  SLOT(setChecked(bool)));

    connect(m_environmentToolbarButton, SIGNAL(clicked(bool)), m_environmentToolbarButton, SLOT(setChecked(bool)));
    connect(m_environmentToolbarButton, SIGNAL(clicked()), m_environmentDockWidget, SLOT(show()));
    connect(m_environmentButton, SIGNAL(clicked()), m_openGLWidget, SLOT(loadEnvironmentMap()));
    connect(m_environmentButton, SIGNAL(clicked()), this, SLOT(displayEnvironmentMap()));

    m_menuBar = new QMenuBar(this);

    m_fileMenu = new QMenu("File");
    m_importAction = new QAction(tr("&Import"),this);
    connect(m_importAction,SIGNAL(triggered()),meshWgt,SLOT(importModel()));
    m_fileMenu->addAction(m_importAction);
    m_fileMenu->addAction("Save");
    m_menuBar->addAction(m_fileMenu->menuAction());

    m_renderMenu = new QMenu("Render");

    m_saveImage = new QAction(tr("&Image"), this);
//    m_saveImage->setShortcut(QKeySequence::Image);
    m_saveImage->setStatusTip(tr("Render to image"));
    connect(m_saveImage, SIGNAL(triggered()), m_openGLWidget, SLOT(saveImage()));

    m_renderMenu->addAction(m_saveImage);
    m_menuBar->addAction(m_renderMenu->menuAction());

    //add our settings button on our toolbar
    m_settingsMenu = new QMenu("Settings");
    m_menuBar->addAction(m_settingsMenu->menuAction());
    QAction *generalSettings = new QAction(tr("&General Settings"),this);
    generalSettings->setStatusTip(tr("Change general settings of Helios"));
    m_settingsMenu->addAction(generalSettings);

    // create our general settings widget
    GenSetDockWidget *genSetwdg = new GenSetDockWidget(this);
    genSetwdg->setHidden(true);

    connect(generalSettings, SIGNAL(triggered()), genSetwdg, SLOT(show()));
    connect(genSetwdg, SIGNAL(signalMoveRenderReduction(int)),m_openGLWidget,SLOT(setMoveRenderReduction(int)));
    connect(genSetwdg, SIGNAL(signalSetTimeOutDur(int)),m_openGLWidget,SLOT(setTimeOutDur(int)));

}

void MainWindow::displayEnvironmentMap(){
    m_environmentLineEdit->setText(m_openGLWidget->getEnvironmentMap());
}
