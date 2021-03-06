#ifndef IMAGENODEPROXYWIDGET_H
#define IMAGENODEPROXYWIDGET_H

//------------------------------------------------------------------------------------------------------------------------------------
/// @class ImageNodeProxyWidget
/// @brief Extention of AbstractNodeProxyWidget that allows us to select an image and apply it to a attribute of a material.
/// @brief This Widget consists of a button to load in an image and a label to display the image.
/// @author Declan Russell
/// @date 05/05/2015
//------------------------------------------------------------------------------------------------------------------------------------

#include <QGraphicsItem>
#include <optixu/optixpp_namespace.h>
#include "AbstractNodeProxyWidget.h"
#include <NodeGraph/qneport.h>
#include <QLabel>
#include <QGroupBox>


class ImageNodeProxyWidget : public AbstractNodeProxyWidget
{
    Q_OBJECT
public:
    ImageNodeProxyWidget(QNEPort *_portConnected,optix::Material &_mat,QGraphicsItem *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default destructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~ImageNodeProxyWidget();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief mutator to the width and height of our image
    //------------------------------------------------------------------------------------------------------------------------------------
    inline void setImageSize(int _width,int _height){m_imgLabel->setMaximumWidth(_width); m_imgLabel->setMaximumHeight(_height);}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overite our setLinkedVar function to put our own functionality in
    //------------------------------------------------------------------------------------------------------------------------------------
    void setLinkedVar();
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief load an image into our widget
    /// @param _path - path to desired image
    //------------------------------------------------------------------------------------------------------------------------------------
    void loadImage(QString _path);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to image path
    //------------------------------------------------------------------------------------------------------------------------------------
    inline QString getImagePath(){return m_imagePath;}
    //------------------------------------------------------------------------------------------------------------------------------------

public slots:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief slot to import an image to our node
    //------------------------------------------------------------------------------------------------------------------------------------
    void getImage();
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our optix texture
    //------------------------------------------------------------------------------------------------------------------------------------
    optix::TextureSampler m_texure;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief label widget to hold our image
    //------------------------------------------------------------------------------------------------------------------------------------
    QLabel *m_imgLabel;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our group box
    //------------------------------------------------------------------------------------------------------------------------------------
    QGroupBox *m_groupBox;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief bool to indicate if we have a texture created
    //------------------------------------------------------------------------------------------------------------------------------------
    bool m_imgCreated;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief the image path
    //------------------------------------------------------------------------------------------------------------------------------------
    QString m_imagePath;
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // IMAGENODEPROXYWIDGET_H
