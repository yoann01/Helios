#ifndef MeshWidget_H
#define MeshWidget_H

/// @class MeshWidget
/// @date 29/01/14
/// @author Declan Russell
/// @brief This class is an extention of QWidget that adds all our mesh properties controls as default

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QPushButton>
#include <QFileDialog>
#include <QString>

class MeshWidget : public QWidget
{
    Q_OBJECT
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    //----------------------------------------------------------------------------------------------------------------------
    explicit MeshWidget(int _id = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~MeshWidget();
    //----------------------------------------------------------------------------------------------------------------------
signals:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our signal to deliver our tranforms, called by signalTransformChange()
    //----------------------------------------------------------------------------------------------------------------------
    void meshTransform(int _id, float _transX,float _transY,float _transZ,float _rotX,float _rotY,float _rotZ,float _scaleX,float _scaleY,float _scaleZ);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a signal to import mesh, called by signal import mesh if a mesh has been selected
    /// @param _id - the id of our mesh widget
    /// @param _path - the path to our mesh
    //----------------------------------------------------------------------------------------------------------------------
    void importMesh(int _id, std::string _path);
    //----------------------------------------------------------------------------------------------------------------------
public slots:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our signal to norify if any tranform spinbox have been changed
    //----------------------------------------------------------------------------------------------------------------------
    void signalTransformChange();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function called to import mesh when importMeshBtn pressed
    //----------------------------------------------------------------------------------------------------------------------
    void signalImportMesh();
    //----------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our inport mesh push button
    //----------------------------------------------------------------------------------------------------------------------
    QPushButton* m_importMeshBtn;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spacer for the widget
    //----------------------------------------------------------------------------------------------------------------------
    QSpacerItem* m_meshSpacer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for x rotation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshRotateXDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for y rotation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshRotateYDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for z rotation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshRotateZDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our rotate label
    //----------------------------------------------------------------------------------------------------------------------
    QLabel* m_meshRotateLabel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for x translation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshTranslateXDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for y translation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshTranslateYDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for z translation
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshTranslateZDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our translate label
    //----------------------------------------------------------------------------------------------------------------------
    QLabel* m_meshTranslateLabel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for x scale
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshScaleXDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for Y scale
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshScaleYDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our spinbox for z scale
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_meshScaleZDSpinBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our scale label
    //----------------------------------------------------------------------------------------------------------------------
    QLabel* m_meshScaleLabel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our grid layour for our widget
    //----------------------------------------------------------------------------------------------------------------------
    QGridLayout* m_meshGridLayout;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the mesh id
    //----------------------------------------------------------------------------------------------------------------------
    int m_meshId;
    //----------------------------------------------------------------------------------------------------------------------

};

#endif // MeshWidget_H