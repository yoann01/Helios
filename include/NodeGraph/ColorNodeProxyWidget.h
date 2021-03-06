#ifndef COLORNODEPROXYWIDGET_H
#define COLORNODEPROXYWIDGET_H

//------------------------------------------------------------------------------------------------------------------------------------
/// @class ColorNodeProxyWidget
/// @brief Extention of AbstractNodeProxyWidget that allows us to select a color and apply it to a attribute of a material
/// @author Declan Russell
/// @date 05/05/2015
//------------------------------------------------------------------------------------------------------------------------------------

#include <QGraphicsItem>
#include <optixu/optixpp_namespace.h>
#include <QGroupBox>
#include <QColorDialog>
#include "NodeGraph/AbstractNodeProxyWidget.h"
#include "NodeGraph/qneport.h"

class ColorNodeProxyWidget : public AbstractNodeProxyWidget
{
    Q_OBJECT
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ColorNodeProxyWidget(QNEPort *_portConnected, optix::Material &_mat, QGraphicsItem *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief defualt destructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~ColorNodeProxyWidget();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overite our setLinkedVar function to put our own functionality in
    //------------------------------------------------------------------------------------------------------------------------------------
    void setLinkedVar();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our save function for float 3 node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void save(QDataStream &ds);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our load function for float 3 node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void load(QDataStream &, QMap<quint64, QNEPort *> &portMap);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief red intensity
    //------------------------------------------------------------------------------------------------------------------------------------
    float m_red;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief green intensity
    //------------------------------------------------------------------------------------------------------------------------------------
    float m_green;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief blue intensity
    //------------------------------------------------------------------------------------------------------------------------------------
    float m_blue;
    //------------------------------------------------------------------------------------------------------------------------------------
public slots:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief a slot to set the varibles in our material when our spin box values is changed
    /// @param _col - color selected
    //------------------------------------------------------------------------------------------------------------------------------------
    void setMaterialVars(QColor _col);
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our colour button
    //------------------------------------------------------------------------------------------------------------------------------------
    QPushButton *m_colBtn;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Color Dialog widget
    //------------------------------------------------------------------------------------------------------------------------------------
    QColorDialog *m_colorDialog;
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // COLORNODEPROXYWIDGET_H
