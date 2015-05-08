#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

/// @class AbstractMaterialWidget
/// @author Declan Russell
/// @date 05/03/15
/// @brief an abstract material widget that creates a simple setup
/// @brief so that we can quickly create a UI for an optix material.
/// @brief this class also deals with its own garbage collection for
/// @brief deleting any wigets added to it.

#include <QWidget>
#include <optixu/optixpp.h>
#include <QGridLayout>
#include <QGroupBox>
#include <QLayoutItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "NodeGraph/qnodeseditor.h"
#include "NodeGraph/qneblock.h"
#include "NodeGraph/OSLNodesEditor.h"


#include <QDockWidget>

class AbstractMaterialWidget : public QDockWidget
{
    Q_OBJECT
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our default destructor. Deals with our gabage collection
    //------------------------------------------------------------------------------------------------------------------------------------
    ~AbstractMaterialWidget();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to our instance of our class
    //------------------------------------------------------------------------------------------------------------------------------------
    static AbstractMaterialWidget *getInstance(QWidget *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief set the name of our material
    //------------------------------------------------------------------------------------------------------------------------------------
    inline void setName(std::string _name){m_materialName = _name;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to the name of our material
    //------------------------------------------------------------------------------------------------------------------------------------
    inline std::string getName(){return m_materialName;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessot to our material
    //------------------------------------------------------------------------------------------------------------------------------------
    inline optix::Material &getMaterial(){return m_material;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief adds a shader node to our user interface
    //------------------------------------------------------------------------------------------------------------------------------------
    void addShaderNode();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief query if a material has been created
    //------------------------------------------------------------------------------------------------------------------------------------
    inline bool materialCreated(){return m_matCreated;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief removes our singleton class from existence
    //------------------------------------------------------------------------------------------------------------------------------------
    inline void destroy(){delete m_instance;}
    //------------------------------------------------------------------------------------------------------------------------------------
signals:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief signal to export our OptiX material if updated
    //------------------------------------------------------------------------------------------------------------------------------------
    void updateMaterial(optix::Material _mat);
    //------------------------------------------------------------------------------------------------------------------------------------
public slots:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Create a slot to show our context menu if we right click anywhere on our widget
    //------------------------------------------------------------------------------------------------------------------------------------
    void showContextMenu(const QPoint& pos);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Slot to create a material from our OSL node graph
    //------------------------------------------------------------------------------------------------------------------------------------
    void createOptixMaterial();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief applies the current createed material to mesh of choosing
    /// @param _mesh - the mesh name to apply the material to
    //------------------------------------------------------------------------------------------------------------------------------------
    void applyMaterialToMesh(std::string _mesh);
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    explicit AbstractMaterialWidget(QWidget *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our instance of our node graph widget
    //------------------------------------------------------------------------------------------------------------------------------------
    static AbstractMaterialWidget * m_instance;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief a bool to indicate if a material has been successfuly created
    //------------------------------------------------------------------------------------------------------------------------------------
    bool m_matCreated;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief an array to store all our nodes
    //------------------------------------------------------------------------------------------------------------------------------------
    std::vector<QNEBlock*> m_nodes;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief QNodeEditor that manges loading and saving of our node graph
    //------------------------------------------------------------------------------------------------------------------------------------
    OSLNodesEditor *m_nodeEditor;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief QGraphics scene which will hold the node interface
    //------------------------------------------------------------------------------------------------------------------------------------
    QGraphicsScene *m_nodeInterfaceScene;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief QGraphicsView which will hold our material graph scene
    //------------------------------------------------------------------------------------------------------------------------------------
    QGraphicsView *m_graphicsView;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief the name of our material
    //------------------------------------------------------------------------------------------------------------------------------------
    std::string m_materialName;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Our optix material
    //------------------------------------------------------------------------------------------------------------------------------------
    optix::Material m_material;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief the layout of our group box
    //------------------------------------------------------------------------------------------------------------------------------------
    QGridLayout *m_groupBoxLayout;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief a group box to hold all our widgets buttons
    //------------------------------------------------------------------------------------------------------------------------------------
    QGroupBox *m_widgetGroupBox;
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // MATERIALWIDGET_H
