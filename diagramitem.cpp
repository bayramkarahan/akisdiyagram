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

#include "diagramitem.h"
#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include<QThread>
#include<QCheckBox>

//extern QString varMain0;

//! [0]
DiagramItem::DiagramItem(Diagram::DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
startArrow=0;
endArrow=0;
leftArrow=0;
rightArrow=0;
    drm=0;
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    //label=new QStaticText();
    label.setText("");
    //label->setStyleSheet("background: red");
    //label->show();
    myBackground=QColor(0,0,0,0);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    for(int i = 0; i < 8; i++){
        cornerGrabber[i] = new DotSignal(this);
    }

   // setPositionGrabbers();
//hideGrabbers();

setVisibilityGrabbers();
//drm=false;
}
void DiagramItem::setVisibilityGrabbers()
{
///8 noktanın hangilerinin gözüküp gözükmayaceği buradan belirleniyor
    cornerGrabber[GrabberTop]->setVisible(true);
    cornerGrabber[GrabberBottom]->setVisible(true);
    cornerGrabber[GrabberLeft]->setVisible(true);
    cornerGrabber[GrabberRight]->setVisible(true);
    cornerGrabber[GrabberTopLeft]->setVisible(false);
    cornerGrabber[GrabberTopRight]->setVisible(false);
    cornerGrabber[GrabberBottomLeft]->setVisible(false);
    cornerGrabber[GrabberBottomRight]->setVisible(false);

}

void DiagramItem::setPositionGrabbers()
{

    ///burada noktaların pozisyonlarının ayarlandığı bölüm
    QRectF tmpRect = this->boundingRect();
    int h= cornerGrabber[GrabberRight]->boundingRect().height();
    int w= cornerGrabber[GrabberRight]->boundingRect().width();
    int wx=0;
    if(myDiagramType==Diagram::DiagramType::Input)
    {
        wx=w*2+w;
    }

    cornerGrabber[GrabberTop]->setPos(tmpRect.left() + tmpRect.width()/2-w, tmpRect.top());
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTop]->setZValue(-100);
    cornerGrabber[GrabberTop]->setDotFlags(GrabberTop);


    cornerGrabber[GrabberBottom]->setPos(tmpRect.left() + tmpRect.width()/2-w, tmpRect.bottom()-h-h);
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottom]->setDotFlags(GrabberBottom);

    cornerGrabber[GrabberLeft]->setPos(tmpRect.left()+wx, tmpRect.top() + tmpRect.height()/2-h);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberLeft]->setDotFlags(GrabberLeft);

      cornerGrabber[GrabberRight]->setPos(tmpRect.right()-w-w-wx, tmpRect.top() + tmpRect.height()/2-h);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberRight]->setDotFlags(GrabberRight);

/**********************************************************************************************************************/

    cornerGrabber[GrabberTopLeft]->setPos(tmpRect.topLeft().x(), tmpRect.topLeft().y());
    cornerGrabber[GrabberTopLeft]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTopLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTopLeft]->setZValue(-100);
    cornerGrabber[GrabberTopLeft]->setDotFlags(GrabberTopLeft);


    cornerGrabber[GrabberTopRight]->setPos(tmpRect.topRight().x()-8, tmpRect.topRight().y());
    cornerGrabber[GrabberTopRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTopRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTopRight]->setZValue(-100);
    cornerGrabber[GrabberTopRight]->setDotFlags(GrabberTopRight);

    cornerGrabber[GrabberBottomLeft]->setPos(tmpRect.bottomLeft().x(), tmpRect.bottomLeft().y()-8);
    cornerGrabber[GrabberBottomLeft]->setFlag(QGraphicsItem::ItemIsSelectable,true);
    cornerGrabber[GrabberBottomLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottomLeft]->setZValue(-100);
    cornerGrabber[GrabberBottomLeft]->setDotFlags(GrabberBottomLeft);

    cornerGrabber[GrabberBottomRight]->setPos(tmpRect.bottomRight().x()-8, tmpRect.bottomRight().y()-8);
    cornerGrabber[GrabberBottomRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberBottomRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottomRight]->setZValue(-100);
    cornerGrabber[GrabberBottomRight]->setDotFlags(GrabberBottomRight);

update();

}

QPointF DiagramItem::previousPosition() const
{
    return m_previousPosition;
}

void DiagramItem::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}
//! [0]
void DiagramItem::hideGrabbers()
{
    for(int i = 0; i < 8; i++){
        cornerGrabber[i]->setVisible(false);
    }
}

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
  qDebug()<<"ok silinecek";
    int index = arrows.indexOf(arrow);

    if (index != -1)
    {
         qDebug()<<"ok silindi";
        arrows.removeAt(index);
    }
}
//! [1]
void DiagramItem::removeArrowPolar(Arrow *arrow,QString polar){
    if(polar=="left")leftArrow=0;
    if(polar=="right")rightArrow=0;
    if(polar=="start")startArrow=0;
    if(polar=="end")endArrow=0;
}
//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
bool DiagramItem::addArrowState(Arrow *arrow,QString polar,QString rota)
{
   // qDebug()<<"nesne"<<this->myDiagramType;
    if(this->myDiagramType==Diagram::DiagramType::Start
            ||this->myDiagramType==Diagram::DiagramType::End)
    {
        int polarCount=0;
        if(startArrow!=0) polarCount++;
        if(endArrow!=0) polarCount++;
        if(leftArrow!=0) polarCount++;
        if(rightArrow!=0) polarCount++;
       if(polarCount>0)return false;
    }
    if(this->myDiagramType==Diagram::DiagramType::Input
            ||this->myDiagramType==Diagram::DiagramType::Process)
    {
        int polarCount=0;
        if(startArrow!=0) polarCount++;
        if(endArrow!=0) polarCount++;
        if(leftArrow!=0) polarCount++;
        if(rightArrow!=0) polarCount++;
       if(polarCount>2)return false;
    }
    if(this->myDiagramType==Diagram::DiagramType::Conditional
            ||this->myDiagramType==Diagram::DiagramType::Link)
    {
        int polarCount=0;
        if(startArrow!=0) polarCount++;
        if(endArrow!=0) polarCount++;
        if(leftArrow!=0) polarCount++;
        if(rightArrow!=0) polarCount++;
       if(polarCount>2)return false;
    }
    //arrows.append(arrow);
    if(polar=="start"&&startArrow==0) { startArrow=arrow;startArrowRota=rota;return true;}
    if(polar=="start"&&startArrow!=0) { return false;}

    if(polar=="end"&&endArrow==0){endArrow=arrow;endArrowRota=rota;return true;}
    if(polar=="end"&&endArrow!=0){return false;}

    if(polar=="left"&&leftArrow==0) { leftArrow=arrow;leftArrowRota=rota;return true;}
    if(polar=="left"&&leftArrow!=0) { return false;}

    if(polar=="right"&&rightArrow==0){rightArrow=arrow;rightArrowRota=rota; return true;}
    if(polar=="right"&&rightArrow!=0)return false;
}

void DiagramItem::setText(QString text,QColor color)
{
   /// label.setText(text);
   //  label.setText(QString::number(myDiagramType));
    myBackground=color;
    //painter->setBrush(myBackground);
    update();

}
//! [3]
//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}
//! [4]
//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]
//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]
void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotateState=  !rotateState;
    //qDebug() <<"çift tıklama1 "<<rotateState;

    if(this->myDiagramType==Diagram::DiagramType::Input)
    {
        QDialog * d = new QDialog();
        // QComboBox * comboBoxA = new QComboBox();
      //  comboBoxA->addItems(QStringList() <<varMain0<<varMain1 << varMain2);
      //  QComboBox * comboBoxB = new QComboBox();
      //  comboBoxB->addItems(QStringList() << "=" << "<" << ">");
        QLineEdit * lineEditA = new QLineEdit();
        QLineEdit * lineEditB = new QLineEdit();
        QLineEdit * lineEditC = new QLineEdit();
        QCheckBox *cha=new QCheckBox("Girdi Değeri");
        QCheckBox *chb=new QCheckBox("Girdi Değeri");
        QCheckBox *chc=new QCheckBox("Girdi Değeri");

        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

        QVBoxLayout * vbox = new QVBoxLayout();
        QHBoxLayout * hbox1 = new QHBoxLayout();
        hbox1->addWidget(lineEditA);
        hbox1->addWidget(cha);

        QHBoxLayout * hbox2 = new QHBoxLayout();
        hbox2->addWidget(lineEditB);
        hbox2->addWidget(chb);

        QHBoxLayout * hbox3 = new QHBoxLayout();
        hbox3->addWidget(lineEditC);
        hbox3->addWidget(chc);

        vbox->addLayout(hbox1);
        vbox->addLayout(hbox2);
        vbox->addLayout(hbox3);
        vbox->addWidget(buttonBox);

        d->setLayout(vbox);
        lineEditA->setText(varMain0);
        lineEditB->setText(varMain1);
        lineEditC->setText(varMain2);
        cha->setChecked(input0);
        chb->setChecked(input1);
        chc->setChecked(input2);


        int result = d->exec();
        if(result == QDialog::Accepted)
        {
            if(lineEditA->text()!="")varMain0=lineEditA->text();else varMain0="";
            if(lineEditB->text()!="")varMain1=lineEditB->text();else varMain1="";
            if(lineEditC->text()!="")varMain2=lineEditC->text();else varMain2="";
            input0=cha->checkState();
            input1=chb->checkState();
            input2=chc->checkState();
        }

        label.setText("");
        if(varMain0!=""&&input0)label.setText(varMain0);
        if(varMain1!=""&&input1)label.setText(label.text()+", "+varMain1);
        if(varMain2!=""&&input2)label.setText(label.text()+", "+varMain2);
          if(label.text()!="")
        {
            if(label.text()[0]==",")
            {
                QStringRef z = label.text().midRef(1);
                label.setText(z.toString());
            }
        }

     }
    if(this->myDiagramType==Diagram::DiagramType::Output)
    {
        QDialog * d = new QDialog();
        QVBoxLayout * vbox = new QVBoxLayout();
        QComboBox * comboBoxA = new QComboBox();
        comboBoxA->addItems(QStringList()<<"" <<varMain0<<varMain1 << varMain2);
        QComboBox * comboBoxB = new QComboBox();
        comboBoxB->addItems(QStringList()<<"" <<varMain0<<varMain1 << varMain2);
        QComboBox * comboBoxC = new QComboBox();
        comboBoxC->addItems(QStringList()<<"" <<varMain0<<varMain1 << varMain2);

        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

        vbox->addWidget(comboBoxA);
        vbox->addWidget(comboBoxB);
        vbox->addWidget(comboBoxC);
        vbox->addWidget(buttonBox);

        d->setLayout(vbox);
        comboBoxA->setCurrentText(var0);
        comboBoxB->setCurrentText(var1);
        comboBoxC->setCurrentText(var2);

        int result = d->exec();
        if(result == QDialog::Accepted)
        {
            if(comboBoxA->currentText()!="")var0=comboBoxA->currentText();else var0="";
            if(comboBoxB->currentText()!="")var1=comboBoxB->currentText();else var1="";
            if(comboBoxC->currentText()!="")var2=comboBoxC->currentText();else var2="";

        }
        label.setText("");
        if(var0!="")label.setText(var0);
        if(var1!="")label.setText(label.text()+", "+var1);
        if(var2!="")label.setText(label.text()+", "+var2);
        if(label.text()!="")
        {
            if(label.text()[0]==",")
            {
                QStringRef z = label.text().midRef(1);
                label.setText(z.toString());
            }
        }
    }
    if(this->myDiagramType==Diagram::DiagramType::Process)
    {
        QDialog * d = new QDialog();
        QHBoxLayout * vbox = new QHBoxLayout();
        QComboBox * comboBoxA = new QComboBox();
        comboBoxA->addItems(QStringList()<<"" <<varMain0<<varMain1 << varMain2);
        QLineEdit * lineEditB = new QLineEdit();
        QLineEdit * lineEditC = new QLineEdit();

        QComboBox * comboBoxD = new QComboBox();
        comboBoxD->addItems(QStringList()<<"" <<"+"<<"-" << "/"<<"*");

        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

        vbox->addWidget(comboBoxA);
         vbox->addWidget(new QLabel("="));
        vbox->addWidget(lineEditB);
        vbox->addWidget(comboBoxD);
        vbox->addWidget(lineEditC);
        vbox->addWidget(buttonBox);

        d->setLayout(vbox);
        comboBoxA->setCurrentText(var0);
        lineEditB->setText(var1);
        lineEditC->setText(var2);
        comboBoxD->setCurrentText(varOperator0);


        int result = d->exec();
        if(result == QDialog::Accepted)
        {
            if(comboBoxA->currentText()!="")var0=comboBoxA->currentText();else var0="";
            if(lineEditB->text()!="")var1=lineEditB->text();else var1="";
            if(lineEditC->text()!="")var2=lineEditC->text();else var2="";
            if(comboBoxD->currentText()!="")varOperator0=comboBoxD->currentText();else varOperator0="";
        }
        label.setText("");
        if(varOperator0!=""&&var0!=""&&var1!=""&&var2!="")
        {
            label.setText(var0+" = "+var1+" "+varOperator0+" "+var2);
        }
           else
            label.setText("");
    }
    if(this->myDiagramType==Diagram::DiagramType::Loop)
    {
        QDialog * d = new QDialog();
        QVBoxLayout * vbox = new QVBoxLayout();
        QLineEdit * lineEditA = new QLineEdit("k");
        QLineEdit * lineEditB = new QLineEdit("0");
        QLineEdit * lineEditC = new QLineEdit("1");
        QLineEdit * lineEditD = new QLineEdit("1");
        lineEditA->setFixedWidth(40);
        lineEditB->setFixedWidth(40);
        lineEditC->setFixedWidth(40);
        lineEditD->setFixedWidth(40);
        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));
        QHBoxLayout * hbox1 = new QHBoxLayout();
        hbox1->addWidget(lineEditA);
        hbox1->addWidget(new QLabel("="));
        hbox1->addWidget(lineEditB);
        QHBoxLayout * hbox2 = new QHBoxLayout();
        //hbox2->addWidget(lineEditC);
        hbox2->addWidget(new QLabel(lineEditA->text()+" < "));
        hbox2->addWidget(lineEditC);
        QHBoxLayout * hbox3 = new QHBoxLayout();
        hbox3->addWidget(new QLabel(lineEditA->text()+" = "+lineEditA->text()+" + "));
        hbox3->addWidget(lineEditD);
        vbox->addLayout(hbox1);
        vbox->addLayout(hbox2);
        vbox->addLayout(hbox3);
        vbox->addWidget(buttonBox);

        d->setLayout(vbox);
        if(var0!="") lineEditA->setText(var0);
        if(varStartValue0!="")lineEditB->setText(varStartValue0);
        if(varEndValue0!="") lineEditC->setText(varEndValue0);
        if(varStepValue0!="") lineEditD->setText(varStepValue0);

        int result = d->exec();
        if(result == QDialog::Accepted)
        {
            if(lineEditA->text()!="") var0=lineEditA->text();else var0="";
            if(lineEditB->text()!="") varStartValue0=lineEditB->text();else varStartValue0="";
            if(lineEditC->text()!="") varEndValue0=lineEditC->text();else varEndValue0="";
            if(lineEditD->text()!="") varStepValue0=lineEditD->text();else varStepValue0="";

            if(varStartValue0!=""&&var0!=""&&varEndValue0!=""&&varStepValue0!="")
        {
            label.setText(var0+" = "+varStartValue0+"; "+var0+" < "+varEndValue0+"; "+var0+" = "+var0+" + "+varStepValue0);
        }
           else
            label.setText("");

    }
    }
    if(this->myDiagramType==Diagram::DiagramType::Conditional)
    {

        QDialog * d = new QDialog();
        QVBoxLayout * vbox = new QVBoxLayout();
        QComboBox * comboBoxA0 = new QComboBox();
        comboBoxA0->addItems(QStringList()<<"" <<varMain0<<varMain1 << varMain2);
        QComboBox * comboBoxB0 = new QComboBox();
        comboBoxB0->addItems(QStringList() <<""<< "=" << "<" << ">");
        QLineEdit * lineEditA0 = new QLineEdit();

        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

        vbox->addWidget(comboBoxA0);
        vbox->addWidget(comboBoxB0);
        vbox->addWidget(lineEditA0);
        vbox->addWidget(buttonBox);

        d->setLayout(vbox);
        comboBoxA0->setCurrentText(var0);
        comboBoxB0->setCurrentText(varOperator0);
        lineEditA0->setText(varConditional0);

        int result = d->exec();
        if(result == QDialog::Accepted)
        {
            if(comboBoxA0->currentText()!="")var0=comboBoxA0->currentText();else var0="";
            if(comboBoxB0->currentText()!="")varOperator0=comboBoxB0->currentText();else varOperator0="";
            if(lineEditA0->text()!="")varConditional0=lineEditA0->text();else varConditional0="";
        }
        label.setText("");
        if(varOperator0!=""&&var0!=""&&varConditional0!="")
        {
            label.setText(var0+" "+varOperator0+" "+varConditional0);
        }
           else
            label.setText("");
    }

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  // qDebug() <<"move yapıldı";
 //  qDebug() <<"üzerine geldi"<<drm<<rotateState;
   if(drm)
   {
    QPointF pt = event->pos();
    if(m_actionFlags == ResizeState){
        switch (m_cornerFlags) {
        case Top:
            resizeTop(pt);
            break;
        case Bottom:
            resizeBottom(pt);
            break;
        case Left:
            resizeLeft(pt);
            break;
        case Right:
            resizeRight(pt);
            break;
        case TopLeft:
            resizeTop(pt);
            resizeLeft(pt);
            break;
        case TopRight:
           resizeTop(pt);
            resizeRight(pt);
            rotateItem(pt);
            break;
        case BottomLeft:
            resizeBottom(pt);
            resizeLeft(pt);
            break;
        case BottomRight:
            resizeBottom(pt);
            resizeRight(pt);
            break;
        default:
            if (m_leftMouseButtonPressed&&drm) {
               // qDebug() <<"taşınıyor";
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - m_previousPosition.x();
                auto dy = event->scenePos().y() - m_previousPosition.y();
                moveBy(dx,dy);
                setPreviousPosition(event->scenePos());
                emit signalMove(this, dx, dy);

            }
            break;
        }
    } else {
        //qDebug() <<"abc";
        switch (m_cornerFlags) {
        case Top:{

            resizeTop(pt);
            break;
        }
        case Right:{
            resizeRight(pt);

            break;
        }
        case Left: {
          //  Scene * _scene = dynamic_cast<Scene *>(parent);
            ///if(sekilTr!=Diagram::DiagramType::Pdf) scn->removeItem(this);
            resizeLeft(pt);

           // qDebug() <<"siliniyorrr";

           break;
        }
        case Bottom: {
            resizeBottom(pt);
            break;
        }
        default:
           if (m_leftMouseButtonPressed&&drm) {
             //  qDebug() <<"gidiyor";
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - m_previousPosition.x();
                auto dy = event->scenePos().y() - m_previousPosition.y();
                moveBy(dx,dy);
                setPreviousPosition(event->scenePos());
                //emit signalMove(this, dx, dy);

            }

            break;
        }
    }
   }
   ///scn->update();
   //QGraphicsItem::mouseMoveEvent(event);
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() & Qt::LeftButton) {
        drm=true;
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());
        emit clicked(this);
       // qDebug() <<"rectangle nesnesine tıklama yapıldı";


    }
  //  QGraphicsItem::mousePressEvent(event);
//QGraphicsScene::mousePressEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() <<"rectangle nesnesine release yapıldı";
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = false;
        /****************************************************/

        /* dclick=(!dclick)?true:false;
        if(dclick)
        {
            setPositionGrabbers();
            setVisibilityGrabbers();
          //  QGraphicsItem::hoverEnterEvent(event);
        }
        else
        {
            m_cornerFlags = 0;
           hideGrabbers();
           setCursor(Qt::CrossCursor);
          // drm=false;
        }
        qDebug() <<"tek tıklama tıklama "<<dclick;*/

        /*****************************************************/
    }
   // QGraphicsItem::mouseReleaseEvent(event);
}
void DiagramItem::renk()
{
    qDebug() <<"renk"<<drm;//<<myDiagramType;
    renkdrm=true;
}
void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() <<"üzerine geldi"<<drm;
  /* if(drm)
   {
    setPositionGrabbers();
    setVisibilityGrabbers();

   }*/
    QGraphicsItem::hoverEnterEvent(event);
}

void DiagramItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//renkdrm=false;
//update();
   //  qDebug() <<"üzerine ayrıldı"<<drm;
  /*
     m_cornerFlags = 0;
    hideGrabbers();
    setCursor(Qt::CrossCursor);
    drm=false;
*/
    QGraphicsItem::hoverLeaveEvent( event );

}

void DiagramItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   // renkdrm=true;
   // qDebug() <<"üzerindesin"<<drm<<renkdrm;

  /*  if(drm)
    {
    //qDebug() <<"hoverMoveEvent";
    QPointF pt = event->pos();              // The current position of the mouse
    qreal drx = pt.x() - this->boundingRect().right();    // Distance between the mouse and the right
    qreal dlx = pt.x() - this->boundingRect().left();     // Distance between the mouse and the left

    qreal dby = pt.y() - this->boundingRect().top();      // Distance between the mouse and the top
    qreal dty = pt.y() - this->boundingRect().bottom();   // Distance between the mouse and the bottom

    // If the mouse position is within a radius of 7
    // to a certain side( top, left, bottom or right)
    // we set the Flag in the Corner Flags Register

    m_cornerFlags = 0;
    int alan=25;
    if( (dby < alan && dby > -alan)||(dby < -alan && dby > alan) ) m_cornerFlags |= Top;       // Top side
    if( (dty < alan && dty > -alan)|| (dty < -alan && dty > alan)) m_cornerFlags |= Bottom;    // Bottom side
    if( (drx < alan && drx > -alan )||(drx < -alan && drx > alan )) m_cornerFlags |= Right;     // Right side
    if( (dlx < alan && dlx > -alan)||  (dlx < -alan && dlx > alan)) m_cornerFlags |= Left;      // Left side


        switch (m_cornerFlags) {
        case Top:
        {
            qDebug() <<"top";
            break;

        }
        case Right: {
                  qDebug() <<"right";
                    break;
                }
        case Left:
        {   
            qDebug() <<"Left";
            break;
                }
        case Bottom: {
            qDebug() <<"bottom";
            break;
        }
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }

    }
*/
    QGraphicsItem::hoverMoveEvent( event );
}

void DiagramItem::resizeLeft(const QPointF &pt)
{
    qDebug()<<"resizeLeft";
    /*
    QRectF tmpRect = rect();
    // if the mouse is on the right side we return
    if( pt.x() > tmpRect.right() )
        return;
    qreal widthOffset =  ( pt.x() - tmpRect.right() );
    // limit the minimum width
    if( widthOffset > -10 )
        return;
    // if it's negative we set it to a positive width value
    if( widthOffset < 0 )
        tmpRect.setWidth( -widthOffset );
    else
        tmpRect.setWidth( widthOffset );
    // Since it's a left side , the rectange will increase in size
    // but keeps the topLeft as it was
    tmpRect.translate( rect().width() - tmpRect.width() , 0 );
    prepareGeometryChange();
    // Set the ne geometry
    setRect( tmpRect );
    // Update to see the result
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeRight(const QPointF &pt)
{
      qDebug()<<"resizeRight";
    /*
    QRectF tmpRect = rect();
    if( pt.x() < tmpRect.left() )
        return;
    qreal widthOffset =  ( pt.x() - tmpRect.left() );
    if( widthOffset < 10 ) /// limit
        return;
    if( widthOffset < 10)
        tmpRect.setWidth( -widthOffset );
    else
        tmpRect.setWidth( widthOffset );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeBottom(const QPointF &pt)
{
     qDebug()<<"resizeBottom";
    /*QRectF tmpRect = rect();
    if( pt.y() < tmpRect.top() )
        return;
    qreal heightOffset =  ( pt.y() - tmpRect.top() );
    if( heightOffset < 11 ) /// limit
        return;
    if( heightOffset < 0)
        tmpRect.setHeight( -heightOffset );
    else
        tmpRect.setHeight( heightOffset );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeTop(const QPointF &pt)
{
     qDebug()<<"resizeTop";
   /* DiagramItem
    QRectF tmpRect = rect();
    if( pt.y() > tmpRect.bottom() )
        return;
    qreal heightOffset =  ( pt.y() - tmpRect.bottom() );
    if( heightOffset > -11 ) /// limit
        return;
    if( heightOffset < 0)
        tmpRect.setHeight( -heightOffset );
    else
        tmpRect.setHeight( heightOffset );
    tmpRect.translate( 0 , rect().height() - tmpRect.height() );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::rotateItem(const QPointF &pt)
{
     qDebug()<<"rotateItem";
   /* QRectF tmpRect = rect();
    QPointF center;
    if(rotateState)    center = boundingRect().bottomRight();
    else center = boundingRect().center();

    QPointF corner;
    switch (m_cornerFlags) {
    case TopLeft:
        corner = tmpRect.topLeft();
        break;
    case TopRight:
        corner = tmpRect.topRight();
        break;
    case BottomLeft:
        corner = tmpRect.bottomLeft();
        break;
    case BottomRight:
        corner = tmpRect.bottomRight();
        break;
    default:
        break;
    }

    QLineF lineToTarget(center,corner);
    QLineF lineToCursor(center, pt);
    // Angle to Cursor and Corner Target points
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    qreal angleToCursor = ::acos(lineToCursor.dx() / lineToCursor.length());

    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    if (lineToCursor.dy() < 0)
        angleToCursor = TwoPi - angleToCursor;
    angleToCursor = normalizeAngle((Pi - angleToCursor) + Pi / 2);

    // Result difference angle between Corner Target point and Cursor Point
    auto resultAngle = angleToTarget - angleToCursor;

    QTransform trans = transform();
    trans.translate( center.x(), center.y());
    trans.rotateRadians(rotation() + resultAngle, Qt::ZAxis);
    trans.translate( -center.x(),  -center.y());
    setTransform(trans);
   // cx= boundingRect().bottomRight().x();
   // cy= boundingRect().bottomRight().y();
    cx=corner.x();//boundingRect().width();
    cy=corner.y();//-boundingRect().height();
    */
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    Diagram *item=new Diagram();

    myPolygon=item->sekilStore(myDiagramType,this->boundingRect());
    if(myDiagramType==Diagram::DiagramType::Start)label.setText("Başla");
    if(myDiagramType==Diagram::DiagramType::End)label.setText("Son");
    if(myDiagramType==Diagram::DiagramType::Link)label.setText("Bağ");

    //if(myDiagramType==Diagram::DiagramType::Input)label.setText("Input");
    //if(myDiagramType==Diagram::DiagramType::Conditional)label.setText("Conditional");
    //if(myDiagramType==Diagram::DiagramType::Process)label.setText("Process");
    QString lbl=label.text();
    label.setTextWidth(lbl.length()*8);
    painter->drawStaticText(QPoint(this->boundingRect().center().x()-label.textWidth()/2,this->boundingRect().center().y()), label);

    // qDebug()<<myDiagramType;
   /* if(rotateState)
    {
        QStaticText text;
        text.setTextWidth(this->boundingRect().width()*0.9);
        text.setText("<font size=1>sa<br/></font>");
       // qDebug()<<arrows;
        painter->drawStaticText(QPoint(this->boundingRect().center().x(),this->boundingRect().center().y()), text);
    }else
    {
        QStaticText text;
        text.setTextWidth(this->boundingRect().width()*0.9);
        text.setText("<font size=1>as</font>");
        painter->drawStaticText(QPoint(this->boundingRect().center().x(),this->boundingRect().center().y()), text);
    }*/
    if(renkdrm)
    {
        // painter->setPen(QPen(QColor(255,0,0,75), 3, Qt::DashLine));
        // renkdrm=false;
        cornerGrabber[GrabberRight]->renkdrm=renkdrm;
        cornerGrabber[GrabberLeft]->renkdrm=renkdrm;
        cornerGrabber[GrabberBottom]->renkdrm=renkdrm;
        cornerGrabber[GrabberTop]->renkdrm=renkdrm;

    }else
    {
        //painter->setPen(QPen(QColor(0,0,0,255), 3, Qt::SolidLine));
        cornerGrabber[GrabberRight]->renkdrm=renkdrm;
        cornerGrabber[GrabberLeft]->renkdrm=renkdrm;
        cornerGrabber[GrabberBottom]->renkdrm=renkdrm;
        cornerGrabber[GrabberTop]->renkdrm=renkdrm;

    }
    painter->setBrush(myBackground);
    painter->drawPolygon(myPolygon);
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
 setPositionGrabbers();
//qDebug()<<m_leftMouseButtonPressed;
    if (rotateState) {
       // drm=true;
        rotateState=false;
       // painter->setBackground(QColor(200,0,0,200));
        painter->setPen(QPen(QColor(0,0,0,255), 3, Qt::DashLine));
        QPolygonF myPol =myPolygon;
        myPol.translate(0, 4.0);

       painter->drawPolygon(myPolygon);
        //myPol.translate(0,-8.0);
       // painter->drawPolygon(myPol);

    }

    update();

    // setVisibilityGrabbers();
}

