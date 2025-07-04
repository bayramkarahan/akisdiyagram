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

#include "diagramscene.h"
#include "arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = Diagram::Start;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(Diagram::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]
bool DiagramScene::haveStateItem(Diagram::DiagramType diagramItemType)
{
    bool have=false;
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == DiagramItem::Type)
        {
            if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==diagramItemType)
                have=true;
        }
    }

    return have;
}
//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    bool insertState=false;
    DiagramItem *item;
  //  qDebug()<<myMode<<DiagramScene::MoveItem;

    switch (myMode) {
        case InsertItem:
         if(haveStateItem(myItemType)&&myItemType==Diagram::DiagramType::Start) insertState=true;
        if(haveStateItem(myItemType)&&myItemType==Diagram::DiagramType::End) insertState=true;

        if(!insertState)
        {
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            if(myItemType==Diagram::DiagramType::Loop)
            {
                ///qDebug()<<"Döngü Ekledin";
                //alt link
                DiagramItem *endLinkItem = new DiagramItem(Diagram::DiagramType::Link, myItemMenu);
                endLinkItem->setBrush(myItemColor);
                addItem(endLinkItem);
                endLinkItem->setPos(mouseEvent->scenePos().x(),mouseEvent->scenePos().y()+150);
                //sol alt link
                DiagramItem *leftEndLinkItem = new DiagramItem(Diagram::DiagramType::Link, myItemMenu);
                leftEndLinkItem->setBrush(myItemColor);
                addItem(leftEndLinkItem);
                leftEndLinkItem->setPos(mouseEvent->scenePos().x()-250,mouseEvent->scenePos().y()+150);
                //sol link
                DiagramItem *leftLinkItem = new DiagramItem(Diagram::DiagramType::Link, myItemMenu);
                leftLinkItem->setBrush(myItemColor);
                addItem(leftLinkItem);
                leftLinkItem->setPos(mouseEvent->scenePos().x()-250,mouseEvent->scenePos().y());

                Arrow *arrow1 = new Arrow(item,endLinkItem,"end","start",myItemMenu);
                arrow1->setColor(myLineColor);
                item->addArrowState(arrow1,"end","O");
                endLinkItem->addArrowState(arrow1,"start","I");
                arrow1->setZValue(-1000.0);
                addItem(arrow1);
                arrow1->updatePosition();

                Arrow *arrow2 = new Arrow(endLinkItem,leftEndLinkItem,"left","right",myItemMenu);
                arrow2->setColor(myLineColor);
                endLinkItem->addArrowState(arrow2,"left","O");
                leftEndLinkItem->addArrowState(arrow2,"right","I");
                arrow2->setZValue(-1000.0);
                addItem(arrow2);
                arrow2->updatePosition();

                Arrow *arrow3 = new Arrow(leftEndLinkItem,leftLinkItem,"start","end",myItemMenu);
                arrow3->setColor(myLineColor);
                leftEndLinkItem->addArrowState(arrow3,"start","O");
                leftLinkItem->addArrowState(arrow3,"end","I");
                arrow3->setZValue(-1000.0);
                addItem(arrow3);
                arrow3->updatePosition();

                Arrow *arrow4 = new Arrow(leftLinkItem,item,"right","left",myItemMenu);
                arrow4->setColor(myLineColor);
                leftLinkItem->addArrowState(arrow4,"right","O");
                item->addArrowState(arrow4,"left","I");
                arrow4->setZValue(-1000.0);
                addItem(arrow4);
                arrow4->updatePosition();
                item->var0="k";
                item->varStartValue0="0";
                item->varEndValue0="1";
                item->varStepValue0="1";
                item->label.setText(item->var0+" = "+
                                    item->varStartValue0+
                                    "; "+item->var0+
                                    " < "+item->varEndValue0+
                                    "; "+item->var0+" = "+
                                    item->var0+" + "+
                                    item->varStepValue0);

             }

        }
        emit itemInserted(myItemType);
            break;
//! [6] //! [7]
        case InsertLine:

            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
//! [7] //! [8]
    case InsertText:
        textItem = new DiagramTextItem(myItemMenu);
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);
        break;

    case MoveItem:
        //qDebug()<<"seçme modunda";
       /* int boyut=30;
        int xp=mouseEvent->scenePos().x();
        int yp=mouseEvent->scenePos().y();
        QRect rect(QPoint(xp-boyut,yp-boyut),QPoint(xp+boyut,yp+boyut));
        QPainterPath pp;
        pp.addRect(rect);
        setSelectionArea(pp);

        Arrow *ar1;
        foreach (QGraphicsItem *item,selectedItems()) {
            if (item->type() == Arrow::Type)
            {
                ar1=qgraphicsitem_cast<Arrow *>(item);
                ar1->setFlag(QGraphicsItem::ItemIsSelectable);
            }

        }
        QList<QGraphicsItem*> list = items();
           foreach(QGraphicsItem* item, list){
               item->setSelected(true);
              // qDebug()<<"selection: "<<item->isSelected();
           }
        //QTransform deviceTransform;
        //QGraphicsItem *selectItem = itemAt(startItems.first(), deviceTransform);
*/
        break;
        //! [8] //! [9]
   /* default:
        ;*/
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
         QList<QGraphicsItem *> endItems = items(line->line().p2());
         /**************************************************************************************/
         foreach (QGraphicsItem *item, items()) {
             DiagramItem *eleman = qgraphicsitem_cast<DiagramItem *>(item);
             if(eleman!=0)
             {
             eleman->renkdrm=false;
             }
         }
     /****************************************************************************************/
         DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.last());
        if(endItem!=0)
        {
            //qDebug()<<"tür";
            if (endItem->type() == DiagramItem::Type){

                endItem->renkdrm=true;
             }
        }
/****************************************************************************************************/
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    update();
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    foreach (QGraphicsItem *item, items()) {
        DiagramItem *eleman = qgraphicsitem_cast<DiagramItem *>(item);
        if(eleman!=0)
        {
        eleman->renkdrm=false;
        }
    }

    QPointF startPoint,endPoint;
    if (line != 0 && myMode == InsertLine) {

        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        /************************************************/
        startPoint=line->line().p1();
        endPoint=line->line().p2();
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            ///burada önemli işlemler yapıldı..
            QString startPolar=polarDiagramItem(startItem,startPoint,startItem,endItem);
            QString endPolar= polarDiagramItem(endItem,endPoint,startItem,endItem);
           /// qDebug()<<"son"<<startPolar<<endPolar;
            if(startPolar!=""&&endPolar!="")
            {
                Arrow *arrow = new Arrow(startItem,endItem,startPolar,endPolar,myItemMenu);
                arrow->setColor(myLineColor);
                bool startAddStatus=startItem->addArrowState(arrow,startPolar,"O");
                bool endAddStatus=endItem->addArrowState(arrow,endPolar,"I");
               /// qDebug()<<"AddStatus:"<<startAddStatus<<endAddStatus;
               if(startItem->myDiagramType==Diagram::DiagramType::Loop)arrow->answer="N";
                if(startAddStatus&&endAddStatus)
                {
                    arrow->setZValue(-1000.0);
                    addItem(arrow);
                    arrow->updatePosition();
                }else
                {
                    if(startAddStatus&&startPolar=="left") {startItem->leftArrow=0;startItem->leftArrowRota="";}
                    if(startAddStatus&&startPolar=="right") {startItem->rightArrow=0;startItem->rightArrowRota="";}
                    if(startAddStatus&&startPolar=="start"){ startItem->startArrow=0;startItem->startArrowRota="";}
                    if(startAddStatus&&startPolar=="end") {startItem->endArrow=0;startItem->endArrowRota="";}

                    if(endAddStatus&&endPolar=="left") {endItem->leftArrow=0;endItem->leftArrowRota="";}
                    if(endAddStatus&&endPolar=="right") {endItem->rightArrow=0;endItem->rightArrowRota="";}
                    if(endAddStatus&&endPolar=="start") {endItem->startArrow=0;endItem->startArrowRota="";}
                    if(endAddStatus&&endPolar=="end") {endItem->endArrow=0;endItem->endArrowRota="";}

                }
            }
        }
    }
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();
}
//! [13]
QString DiagramScene::polarDiagramItem(DiagramItem *diagramItem, QPointF point,DiagramItem *myStartItem,DiagramItem *myEndItem)
{
    /****************************************************/
    /*  QLineF centerLine(myStartItem->pos(), myEndItem->pos());

        QPolygonF startPolygon = myStartItem->polygon();
        QPointF sp1 = startPolygon.first() + myStartItem->pos();
        QPointF sp2;
        QLineF startPolyLine;
        QPointF intersectPointStartPoint;

        for (int i = 1; i < startPolygon.count(); ++i) {
           sp2 = startPolygon.at(i) + myStartItem->pos();
           startPolyLine = QLineF(sp1, sp2);
           QLineF::IntersectType intersectType =startPolyLine.intersect(centerLine, &intersectPointStartPoint);

           if (intersectType == QLineF::BoundedIntersection)
               break;
           sp1 = sp2;
       }
    /***********************************************/
    //int x=intersectPointStartPoint.x();
    //int y=intersectPointStartPoint.y();
    int x=point.x();
    int y=point.y();


     int tx=0,ty=0;
    bool left=false;
    bool right=false;
    bool start=false;
    bool end=false;
    tx=diagramItem->boundingRect().width()/4;
    ty=diagramItem->boundingRect().height()/4;

    //auto cxy = diagramItem->mapToScene(diagramItem->boundingRect().center());
    int startx,starty,leftx,lefty,rightx,righty,endx,endy;
    int cx=diagramItem->mapToScene(diagramItem->boundingRect().center()).toPoint().x();
    int cy=diagramItem->mapToScene(diagramItem->boundingRect().center()).toPoint().y();

    startx=cx;
    starty=cy-ty*2;
    endx=cx;
    endy=cy+ty*2;
    leftx=cx-tx*2;
    lefty=cy;
    rightx=cx+tx*2;
    righty=cy;
    //qDebug()<<point<<cx<<cy;
    //tx=tx*3/4;
    //ty=ty*3/4;
  /*
    if(x-tx<leftx&&x+tx>leftx) left=true;
    if(x-tx<rightx&&x+tx>rightx) right=true;
    if(y-ty<starty&&y+ty>starty) start=true;
    if(y-ty<endy&&y+ty>endy) end=true;
    */
    if(x>leftx-tx*1.8&&x<leftx+tx*1.8) left=true;
    if(x>rightx-tx*1.8&&x<rightx+tx*1.8) right=true;
    if(y>starty-ty*1.8&&y<starty+ty*1.8) start=true;
    if(y>endy-ty*1.8&&y<endy+ty*1.8) end=true;
   //qDebug()<<left<<right<<start<<end;
    if(left&&!right&&!start&&!end) return "left";
    if(!left&&right&&!start&&!end) return "right";
    if(!left&&!right&&start&&!end) return "start";
    if(!left&&!right&&!start&&end) return "end";

    return "";
}
//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}


//! [14]
