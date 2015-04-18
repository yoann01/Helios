#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//----------------------------------------------------------------------------------------------------------------------------------------
#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QGridLayout>
#include <QDockWidget>
#include <QSpacerItem>
#include <QComboBox>
#include <QLabel>
#include <QColorDialog>
#include <QPushButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QLineEdit>
//----------------------------------------------------------------------------------------------------------------------------------------
#include "Core/OpenGLWidget.h"
#include "UI/MeshWidget.h"
#include "UI/MeshDockWidget.h"
#include "UI/GenSetDockWidget.h"
//----------------------------------------------------------------------------------------------------------------------------------------
namespace Ui {
class MainWindow;
}
//----------------------------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    explicit MainWindow(QWidget *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~MainWindow();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Creates our top toolbar
    //------------------------------------------------------------------------------------------------------------------------------------
    void createMenus();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Our Qt OpenGL context
    //------------------------------------------------------------------------------------------------------------------------------------
    OpenGLWidget *m_openGLWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Main window UI
    //------------------------------------------------------------------------------------------------------------------------------------
    Ui::MainWindow *ui;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A toolbar for holding buttons associatied with lighting, models etc.
    //------------------------------------------------------------------------------------------------------------------------------------
    QToolBar *m_toolBar;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A toolBar button used for light functionality
    //------------------------------------------------------------------------------------------------------------------------------------
    QToolButton *m_lightToolbarButton;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A widget for holding light options
    //------------------------------------------------------------------------------------------------------------------------------------
    QWidget *m_lightWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A grid layout for use with light options
    //------------------------------------------------------------------------------------------------------------------------------------
    QGridLayout *m_lightDockGridLayout;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A dock widget for holding light options
    //------------------------------------------------------------------------------------------------------------------------------------
    QDockWidget *m_lightDockWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Spacer for the light dock
    //------------------------------------------------------------------------------------------------------------------------------------
    QSpacerItem *m_lightSpacer;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Light type label
    //------------------------------------------------------------------------------------------------------------------------------------
    QLabel *m_lightTypeLabel;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Combo box for selecting light type
    //------------------------------------------------------------------------------------------------------------------------------------
    QComboBox *m_lightTypeComboBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A colour picker for use with lights
    //------------------------------------------------------------------------------------------------------------------------------------
    QColorDialog *m_lightColourDialog;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Button for choosing light colour
    //------------------------------------------------------------------------------------------------------------------------------------
    QPushButton *m_lightColourButton;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Light intensity label
    //------------------------------------------------------------------------------------------------------------------------------------
    QLabel *m_lightIntensityLabel;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Light intensity slider
    //------------------------------------------------------------------------------------------------------------------------------------
    QSlider *m_lightIntensitySlider;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Toolbar button for opening mesh options
    //------------------------------------------------------------------------------------------------------------------------------------
    QToolButton *m_meshToolbarButton;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Dock widget for mesh options
    //------------------------------------------------------------------------------------------------------------------------------------
    MeshDockWidget *m_meshDockWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A toolBar button used for environment map functionality
    //------------------------------------------------------------------------------------------------------------------------------------
    QToolButton *m_environmentToolbarButton;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A dock widget for the environment map settings
    //------------------------------------------------------------------------------------------------------------------------------------
    QDockWidget *m_environmentDockWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A group box for the environment dock
    //------------------------------------------------------------------------------------------------------------------------------------
    QGroupBox *m_environmentGroupBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A grid layout for the environment group box
    //------------------------------------------------------------------------------------------------------------------------------------
    QGridLayout *m_environmentGridLayout;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A line edit to display environement map
    //------------------------------------------------------------------------------------------------------------------------------------
    QLineEdit *m_environmentLineEdit;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A button to load new environment map
    //------------------------------------------------------------------------------------------------------------------------------------
    QPushButton *m_environmentButton;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Menu Bar
    //------------------------------------------------------------------------------------------------------------------------------------
    QMenuBar *m_menuBar;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief File Menu
    //------------------------------------------------------------------------------------------------------------------------------------
    QMenu *m_fileMenu;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Render Menu
    //------------------------------------------------------------------------------------------------------------------------------------
    QMenu *m_renderMenu;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our settings menu on our menu bar
    //------------------------------------------------------------------------------------------------------------------------------------
    QMenu *m_settingsMenu;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our general settings dockable widget
    //------------------------------------------------------------------------------------------------------------------------------------
    GenSetDockWidget *m_genSetDockWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Save image action
    //------------------------------------------------------------------------------------------------------------------------------------
    QAction *m_saveImage;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Import model action for toolbar
    //------------------------------------------------------------------------------------------------------------------------------------
    QAction *m_importAction;
    //------------------------------------------------------------------------------------------------------------------------------------
private slots:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief display the enironment map name in the environment map line edit
    //------------------------------------------------------------------------------------------------------------------------------------
    void displayEnvironmentMap();
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H
