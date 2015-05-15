#ifndef OSLVARPOINTBLOCK_H
#define OSLVARPOINTBLOCK_H

#include "NodeGraph/OSLAbstractVarBlock.h"
#include <optixu/optixpp_namespace.h>
#include <QGraphicsScene>

class OSLVarPointBlock : public OSLAbstractVarBlock
{
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    OSLVarPointBlock(QGraphicsScene *_scene,optix::Material _mat,QGraphicsItem * parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default destructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~OSLVarPointBlock(){}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our save function for point node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void save(QDataStream &ds);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our load function for point node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void load(QDataStream &, QMap<quint64, QNEPort *> &portMap);
    //------------------------------------------------------------------------------------------------------------------------------------


};

#endif // OSLVARPOINTBLOCK_H
