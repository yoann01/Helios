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

#include "NodeGraph/qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "NodeGraph/qneconnection.h"

QNEPort::QNEPort(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
	label = new QGraphicsTextItem(this);

	radius_ = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
	setPath(p);

    setPen(QPen(Qt::black));
	setBrush(Qt::red);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;

    //init our variable type - Dec
    m_varibleType = TypeVoid;
}

QNEPort::~QNEPort()
{
	foreach(QNEConnection *conn, m_connections)
		delete conn;
}

void QNEPort::setNEBlock(QNEBlock *b)
{
	m_block = b;
}

void QNEPort::setName(const QString &n)
{
	name = n;
    std::string params;
    for(unsigned int i=0;i<m_initParams.size();i++)
    {
        params+=m_initParams[i];
        if(i<m_initParams.size()-1)
        {
            params+=",";
        }
    }
    label->setPlainText(n + params.c_str());
}

void QNEPort::setIsOutput(bool o)
{
	isOutput_ = o;
    QFontMetrics fm(scene()->font());
//	QRect r = fm.boundingRect(name);

	if (isOutput_)
    {
		label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
    }
    else
    {
		label->setPos(radius_ + margin, -label->boundingRect().height()/2);
    }
}

int QNEPort::radius()
{
	return radius_;
}

bool QNEPort::isOutput()
{
	return isOutput_;
}
    //----mod by Declan Russell----
QVector<QNEConnection*>& QNEPort::connections()
{
	return m_connections;
}

void QNEPort::setPortFlags(int f)
{
	m_portFlags = f;

	if (m_portFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
		label->setFont(font);
		setPath(QPainterPath());
	} else if (m_portFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
		label->setFont(font);
		setPath(QPainterPath());
    }
}

void QNEPort::setVaribleType(QNEPort::variableType _type)
{
    m_varibleType = _type;
    switch(_type)
    {
    case(TypeColour): setBrush(Qt::yellow); break;
    case(TypePoint): setBrush(Qt::red); break;
    case(TypeNormal): setBrush(QBrush(QColor(255,0,255))); break;
    case(TypeFloat): setBrush(Qt::green); break;
    case(TypeInt): setBrush(Qt::darkCyan); break;
    case(TypeString): setBrush(Qt::darkMagenta); break;
    case(TypeMatrix): setBrush(Qt::blue); break;
    default:break;
    }
}

void QNEPort::setInitParams(std::vector<std::string> _initParams)
{
    m_initParams = _initParams;
    if(m_initParams.size()>0)
    {
        std::string params;
        for(unsigned int i=0;i<_initParams.size();i++)
        {
            params+=_initParams[i];
            if(i<_initParams.size()-1)
            {
                params+=",";
            }
        }
        label->setPlainText(name + "=" + params.c_str());
    }
    else{
        label->setPlainText(name);
    }
}

QNEBlock* QNEPort::block() const
{
	return m_block;
}

quint64 QNEPort::ptr()
{
	return m_ptr;
}

void QNEPort::setPtr(quint64 p)
{
	m_ptr = p;
}

bool QNEPort::isConnected(QNEPort *other)
{
	foreach(QNEConnection *conn, m_connections)
		if (conn->port1() == other || conn->port2() == other)
			return true;

	return false;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		foreach(QNEConnection *conn, m_connections)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}
	return value;
}
