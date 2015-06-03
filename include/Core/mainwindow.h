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
#include "UI/GenSetDockWidget.h"
#include "Lights/LightManager.h"
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
    /// @brief Our Qt OpenGL context
    //------------------------------------------------------------------------------------------------------------------------------------
    OpenGLWidget *m_openGLWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief bool to indicate if our menues have been created
    //------------------------------------------------------------------------------------------------------------------------------------
    bool m_menuCreated;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Main window UI
    //------------------------------------------------------------------------------------------------------------------------------------
    Ui::MainWindow *ui;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A line edit to display environement map
    //------------------------------------------------------------------------------------------------------------------------------------
    QLineEdit *m_environmentLineEdit;
    //------------------------------------------------------------------------------------------------------------------------------------
private slots:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Slot when called creates all our menus
    //------------------------------------------------------------------------------------------------------------------------------------
    void createMenus();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief display the enironment map name in the environment map line edit
    //------------------------------------------------------------------------------------------------------------------------------------
    void displayEnvironmentMap();
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H
