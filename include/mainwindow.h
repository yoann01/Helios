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
//----------------------------------------------------------------------------------------------------------------------------------------
#include "OpenGLWidget.h"
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
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Main window UI
    //------------------------------------------------------------------------------------------------------------------------------------
    Ui::MainWindow *ui;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Our Qt OpenGL context
    //------------------------------------------------------------------------------------------------------------------------------------
    OpenGLWidget *m_openGLWidget;
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
    /// @brief Widget for holding mesh options
    //------------------------------------------------------------------------------------------------------------------------------------
    QWidget *m_meshWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Grid layout for mesh options
    //------------------------------------------------------------------------------------------------------------------------------------
    QGridLayout *m_meshDockGridLayout;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Dock widget for mesh options
    //------------------------------------------------------------------------------------------------------------------------------------
    QDockWidget *m_meshDockWidget;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Label for mesh translate
    //------------------------------------------------------------------------------------------------------------------------------------
    QLabel *m_meshTranslateLabel;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for x translations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshTranslateXDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for y translations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshTranslateYDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for z translations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshTranslateZDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for x rotations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshRotateXDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for x rotations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshRotateYDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A double spin box for x rotations
    //------------------------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox *m_meshRotateZDSpinBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Mesh rotation label
    //------------------------------------------------------------------------------------------------------------------------------------
    QLabel *m_meshRotateLabel;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Mesh dock spacer
    //------------------------------------------------------------------------------------------------------------------------------------
    QSpacerItem *m_meshSpacer;
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H