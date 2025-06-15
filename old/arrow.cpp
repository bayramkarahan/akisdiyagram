/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <qmath.h>
#include <QPen>
#include <QPainter>

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem , QString startPolar, QString endPolar, QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    myStartPolar=startPolar;
    myEndPolar=endPolar;
      myContextMenu = contextMenu;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlags(ItemIsSelectable | ItemIsMovable);
    //setAcceptHoverEvents(true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]
void Arrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
          QWidget *)
{
   // painter->setPen(QPen(myColor, 1, Qt::SolidLine));
///qDebug()<<option->state <<QStyle::State_MouseOver;
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]
answerLabel.setText(answer);
painter->drawStaticText(QPoint(this->boundingRect().left()+this->boundingRect().width()/2+5,this->boundingRect().center().y()-20), answerLabel);

 /*  QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    /// qDebug()<<endPolygon;
    QPointF ep1 = endPolygon.first() + myEndItem->pos();
    QPointF ep2;
    QLineF endPolyLine;*/
    QPointF intersectPointEndPoint;

    /* for (int i = 1; i < endPolygon.count(); ++i) {
        ep2 = endPolygon.at(i) + myEndItem->pos();
        endPolyLine = QLineF(ep1, ep2);
        QLineF::IntersectType intersectType =endPolyLine.intersect(centerLine, &intersectPointEndPoint);

        if (intersectType == QLineF::BoundedIntersection)
            break;
        ep1 = ep2;
    }
     QPolygonF startPolygon = myStartItem->polygon();
     QPointF sp1 = startPolygon.first() + myStartItem->pos();
     QPointF sp2;
     QLineF startPolyLine;*/
     QPointF intersectPointStartPoint;

   /*  for (int i = 1; i < startPolygon.count(); ++i) {
        sp2 = startPolygon.at(i) + myStartItem->pos();
        startPolyLine = QLineF(sp1, sp2);
        QLineF::IntersectType intersectType =startPolyLine.intersect(centerLine, &intersectPointStartPoint);

        if (intersectType == QLineF::BoundedIntersection)
            break;
        sp1 = sp2;
    }
     setLine(QLineF(intersectPointEndPoint, intersectPointStartPoint));
*/
     //qDebug()<<"startItem:"<<myStartItem->pos()<<this->myStartPolar;
    // qDebug()<<"endItem:"<<myEndItem->pos()<<this->myEndPolar;

     /*****************************************************/
    int wStart=0,wEnd=0;
     if(myStartItem->myDiagramType==Diagram::DiagramType::Input)
     {
         wStart=15;
     }
     if(myEndItem->myDiagramType==Diagram::DiagramType::Input)
     {
         wEnd=15;
     }
     /*****************************************************/

     if(this->myStartPolar=="left")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().left()+wStart,myStartItem->boundingRect().center().y());
     if(this->myStartPolar=="right")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().right()-wStart,myStartItem->boundingRect().center().y());
         if(this->myStartPolar=="start")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().center().x(),myStartItem->boundingRect().top());
     if(this->myStartPolar=="end")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().center().x(),myStartItem->boundingRect().bottom());

     if(this->myEndPolar=="left")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().left()+wEnd,myEndItem->boundingRect().center().y());
     if(this->myEndPolar=="right")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().right()-wEnd,myEndItem->boundingRect().center().y());
     if(this->myEndPolar=="start")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().center().x(),myEndItem->boundingRect().top());
     if(this->myEndPolar=="end")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().center().x(),myEndItem->boundingRect().bottom());
     //  auto start = diagramItem->mapToScene(diagramItem->boundingRect().center());

   //  qDebug()<<intersectPointStartPoint<<intersectPointEndPoint;

     setLine(QLineF(intersectPointEndPoint, intersectPointStartPoint));


  //! [5] //! [6]

    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);

  arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

//! [6] //! [7]
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
       int aci=qFabs(angle/(1.5708/90))/10;
       painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 8.0);
        painter->drawLine(myLine);
        myLine.translate(0,-16.0);
        painter->drawLine(myLine);

       // qDebug()<<"angel"<<aci;
    }
}

void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(answer=="Y"){answer="N";update();return;}
    if(answer=="N"){answer="Y";update();return;}

    /*bool ok;
        QString text = QInputDialog::getText(0, "Input dialog",
                                            "Date of Birth:", QLineEdit::Normal,
                                            "", &ok);
       if (ok && !text.isEmpty()) {
           QDate date = QDate::fromString(text);
           QString age = text;
           QMessageBox::information (0, "The Age",
                                     QString("The age is %1").arg(age));
       }*/
}
//! [7]

