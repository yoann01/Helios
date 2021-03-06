/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNEBLOCK_H
#define QNEBLOCK_H

//----------------------------------------------------------------------------------------------------------------------
/// @class QNEBlock
/// @brief This class originally written by STANISLAW ADASZEWSKI has been modified
/// @brief to be used for a node based user interface for creating OSL shaders.
/// @brief This Block class handles our node in our scene. It manages all inputs and
/// @brief outputs to the node which will be determined by the OSL shader read in.
/// @date Modifed from 18/03/2015
/// @author STANISLAW ADASZEWSKI modified by Declan Russell
/// @brief [Accessed 2015]. availible from http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//----------------------------------------------------------------------------------------------------------------------

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "NodeGraph/qneport.h"

class QNEBlock : public QGraphicsPathItem
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this means you can define a new QGraphicsItem type. Woudln't of done it like this myself but its a dependency in STANISLAW ADASZEWSKI's implementation
    //----------------------------------------------------------------------------------------------------------------------
    enum { Type = QGraphicsItem::UserType + 3 };
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our defualt constructor
    //----------------------------------------------------------------------------------------------------------------------
    QNEBlock(QGraphicsItem *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a port to our node. This has been modified to also set the varibel type and initial paramiters.
    //----------------------------------------------------------------------------------------------------------------------
    QNEPort* addPort(const QString &name, bool isOutput, std::vector<std::string> _initParams, QNEPort::variableType _type = QNEPort::TypeVoid, int flags = 0, int ptr = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds an input port to our node. Modified to also set the varible type and initial paramiters.
    //----------------------------------------------------------------------------------------------------------------------
    QNEPort *addInputPort(const QString &name, std::vector<std::string> _initParams, QNEPort::variableType _type = QNEPort::TypeVoid);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds an output port to our node. Modified to also set the varible type and initial paramiters.
    //----------------------------------------------------------------------------------------------------------------------
    QNEPort *addOutputPort(const QString &name, std::vector<std::string> _initParams, QNEPort::variableType _type = QNEPort::TypeVoid);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mutator to hard code the width of our node.
    //----------------------------------------------------------------------------------------------------------------------
    inline void setWidth(int _w){width = _w;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mutator to hard code the height of our node.
    //----------------------------------------------------------------------------------------------------------------------
    inline void setHeight(int _h){height = _h;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief virtual function to return the name of our block
    //----------------------------------------------------------------------------------------------------------------------
    virtual inline std::string getBlockName() {return m_blockName;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief save function modified to be virtual to allow implimentation of new blocks
    //----------------------------------------------------------------------------------------------------------------------
    virtual void save(QDataStream&);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load fucntion modified to be virtual to allow implimentaiton of new blocks
    //----------------------------------------------------------------------------------------------------------------------
    virtual void load(QDataStream&, QMap<quint64, QNEPort*> &portMap);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief overwritten paint function for our own drawing of this graphics item
    //----------------------------------------------------------------------------------------------------------------------
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the ports connected to this block
    //----------------------------------------------------------------------------------------------------------------------
    QVector<QNEPort*> ports();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief changed to virtual so we can overide this with our inherited classes
    //----------------------------------------------------------------------------------------------------------------------
    virtual int type() const { return Type; }
    //----------------------------------------------------------------------------------------------------------------------



protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the name of our block
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_blockName;
    //----------------------------------------------------------------------------------------------------------------------

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief horizontal margin for drawing our block
    //----------------------------------------------------------------------------------------------------------------------
	int horzMargin;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vertical margin for drawing our block
    //----------------------------------------------------------------------------------------------------------------------
	int vertMargin;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief width for drawing our block
    //----------------------------------------------------------------------------------------------------------------------
	int width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief height for drawing our block
    //----------------------------------------------------------------------------------------------------------------------
	int height;
    //----------------------------------------------------------------------------------------------------------------------




};

#endif // QNEBLOCK_H
