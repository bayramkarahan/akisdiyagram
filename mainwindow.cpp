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
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QThread>

const int InsertTextButton = 10;


//! [0]
MainWindow::MainWindow()
{
    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(Diagram::DiagramType)),
            this, SLOT(itemInserted(Diagram::DiagramType)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowIcon(QIcon(":images/prg.png"));
    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle("Akış Diyagramı 1.3");

    this->setWindowState(Qt::WindowMaximized);
   // sceneScaleChanged("75%");
    stopAction->setEnabled(false);
}
//! [0]

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
qDebug()<<"main move";
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
qDebug()<<"mainpress";
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
qDebug()<<"mainrelease";
}

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{

    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Mavi Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("Beyaz Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gri Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(Diagram::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]
void MainWindow::deleteArrow(Arrow *item)
{
    scene->removeItem(item);
    Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
    arrow->startItem()->removeArrowPolar(arrow,arrow->myStartPolar);
    arrow->endItem()->removeArrowPolar(arrow,arrow->myEndPolar);
    delete item;
}

//! [3]
void MainWindow::deleteItem()
{
    bool delStatus=false;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        delStatus=false;
        if (item->type() == Arrow::Type)
        {
            deleteArrow(qgraphicsitem_cast<Arrow *>(item));
            delStatus=true;
        }
        if(delStatus==false)
        {
            if (item->type() == DiagramItem::Type){
                // qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
                DiagramItem *ditem=qgraphicsitem_cast<DiagramItem *>(item);
                if(ditem->leftArrow!=0)deleteArrow(ditem->leftArrow);
                if(ditem->rightArrow!=0)deleteArrow(ditem->rightArrow);
                if(ditem->startArrow!=0)deleteArrow(ditem->startArrow);
                if(ditem->endArrow!=0)deleteArrow(ditem->endArrow);
                scene->removeItem(item);
                delete item;
            }
            if (item->type() == DiagramTextItem::Type){
                // qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();

                scene->removeItem(item);
                delete item;
            }
        }
    }
    scene->update();
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        // if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
        if (item->zValue() >= zValue)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        //if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
         if (item->zValue() >= zValue)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(Diagram::DiagramType diagramItemType)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(diagramItemType))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, tr("Akış Diyagramı"),
                       tr("Bu uygulama Linux tabanlı sistemlerde(<b>Pardus</b>); "
                          "<br/>Programlama mantığını anlama, <b>Akış Diyagramı</b> oluştumak ve"
                          "<br/><b>Gerçek Zamanlı Akış Diyagramı Çalıştırmak</b> için yazılmıştır"
                          "<br/>"
                          "<br/>*****************************************************************************"
                          "<br/>   Copyright (C) 2023 by Bayram KARAHAN                                    "
                          "<br/>\tkod.pardus.org.tr/karahan/akisdiyagrami"
                          "<br/>\tgithub.com/bayramkarahan/akisdiyagrami"
                          "<br/>\tbayramkarahan.blogspot.com"
                          "<br/>\tbayramk@gmail.com  "
                          "<br/>*****************************************************************************"

                          "<br/>akisdiyagramı 1.0:Temel özellikler."
                          "<br/>akisdiyagramı 1.1:Bağlantı renklendirmeleri eklendi."
                          "<br/>akisdiyagramı 1.2:Ok ve text nesnesine sağtuş menüsü eklendi."
                          "<br/>akisdiyagramı 1.3:Sağtuş menü özellikleri bütün nesnelere eklendi."

                          "<br/>*****************************************************************************"
                           "<br/>   This program is free software; you can redistribute it and/or modify    "
                           "<br/>   it under the terms of the GNU General Public License as published by    "
                           "<br/>   the Free Software Foundation; either version 3 of the License, or       "
                           "<br/>   (at your option) any later version.                                     "
                           "<br/>                                                                           "
                           "<br/>   This program is distributed in the hope that it will be useful,         "
                           "<br/>   but WITHOUT ANY WARRANTY; without even the implied warranty of          "
                           "<br/>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           "
                           "<br/>   GNU General Public License for more details.                            "
                           "<br/>                                                                           "
                           "<br/>  You should have received a copy of the GNU General Public License       "
                           "<br/>   along with this program; if not, write to the                          "
                           "<br/>  Free Software Foundation, Inc.,                                         "
                           "<br/>  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          "));
}
//! [20]

void MainWindow::worker()
{

    diagramItem->setText("stop",QColor(255,0,0,100));
    /*************************************************/

    if(diagramItem->myDiagramType==Diagram::DiagramType::Input)
    {
        if(varMain0!=""&&diagramItem->input0)
        {
            bool ok;
            QString text = QInputDialog::getText(0, "Girdi",
                                                 varMain0, QLineEdit::Normal,
                                                 "", &ok);
            if (ok && !text.isEmpty()) {
               // diagramItems[index]->varAnswer0= text;
               varAnswerMain0=text;
            }
        }     
        if(varMain1!=""&&diagramItem->input1)
        {
            bool ok;
            QString text = QInputDialog::getText(0, "Girdi",
                                                 varMain1, QLineEdit::Normal,
                                                 "", &ok);
            if (ok && !text.isEmpty()) {
                 varAnswerMain1=text;
            }
        }
        if(varMain2!=""&&diagramItem->input2)
        {
            bool ok;
            QString text = QInputDialog::getText(0, "Girdi",
                                                 varMain2, QLineEdit::Normal,
                                                 "", &ok);
            if (ok && !text.isEmpty()) {
               // diagramItems[index]->varAnswer0= text;
               varAnswerMain2=text;
            }
        }
    }
    if(diagramItem->myDiagramType==Diagram::DiagramType::Output)
    {
        if(diagramItem->var0!=""&&varAnswerMain0!="")
        {     diagramItem->varAnswer0= varAnswerMain0;
            QMessageBox::information (0, "Çıktı\t",
            QString(diagramItem->var0+"= %1").arg( diagramItem->varAnswer0));
        }
        if(diagramItem->var1!=""&&varAnswerMain1!="")
        {     diagramItem->varAnswer1= varAnswerMain1;
            QMessageBox::information (0, "Çıktı\t",
            QString(diagramItem->var1+"= %1").arg( diagramItem->varAnswer1));
        }
        if(diagramItem->var2!=""&&varAnswerMain2!="")
        {     diagramItem->varAnswer2= varAnswerMain2;
            QMessageBox::information (0, "Çıktı\t",
            QString(diagramItem->var2+"= %1").arg( diagramItem->varAnswer2));
        }
    }
    if(diagramItem->myDiagramType==Diagram::DiagramType::Process)
    {
        bool okvar0=false,okvar1=false,okvar2=false,x0x1=false,x0x2=false;
        int x0 = diagramItem->var0.toInt(&okvar0, 10);
        int x1 = diagramItem->var1.toInt(&okvar1, 10);
        int x2 = diagramItem->var2.toInt(&okvar2, 10);
        ///qDebug()<<"a1"<<okvar0<<okvar1<<okvar2<<x0<<x1<<x2;
       // if(diagramItem->var0==varMain0&&diagramItem->var0==diagramItem->var1&&!okvar1) x0x1=true;
        //if(diagramItem->var0==varMain0&&diagramItem->var0==diagramItem->var2&&!okvar2) x0x2=true;
        ///qDebug()<<"a2"<<okvar0<<okvar1<<okvar2<<x0<<x1<<x2;

        if(!okvar0&&diagramItem->var0==varMain0){x0=varAnswerMain0.toInt();}
        if(!okvar0&&diagramItem->var0==varMain1){x0=varAnswerMain1.toInt();}
        if(!okvar0&&diagramItem->var0==varMain2){x0=varAnswerMain2.toInt();}

        if(!okvar1&&diagramItem->var1==varMain0){x1=varAnswerMain0.toInt();}
        if(!okvar1&&diagramItem->var1==varMain1){x1=varAnswerMain1.toInt();}
        if(!okvar1&&diagramItem->var1==varMain2){x1=varAnswerMain2.toInt();}

        if(!okvar2&&diagramItem->var2==varMain0){x2=varAnswerMain0.toInt();}
        if(!okvar2&&diagramItem->var2==varMain1){x2=varAnswerMain1.toInt();}
        if(!okvar2&&diagramItem->var2==varMain2){x2=varAnswerMain2.toInt();}

       // if(!okvar1&&x0x1){x1=x0;okvar1=true;}
        //if(!okvar2&&x0x2){x2=x0;okvar2=true;}
        ///qDebug()<<"a3"<<okvar0<<okvar1<<okvar2<<x0<<x1<<x2;

        if(diagramItem->varOperator0=="+"){x0=x1+x2;}
        if(diagramItem->varOperator0=="-"){x0=x1-x2;}
        if(diagramItem->varOperator0=="/"){x0=x1/x2;}
        if(diagramItem->varOperator0=="*"){x0=x1*x2;}
       /// qDebug()<<"a4"<<okvar0<<okvar1<<okvar2<<x0<<x1<<x2;

        if(diagramItem->var0==varMain0)
        {
            diagramItem->varAnswer0=QString::number(x0);
            varAnswerMain0=QString::number(x0);
        }
        if(diagramItem->var0==varMain1)
        {
            diagramItem->varAnswer1=QString::number(x0);
            varAnswerMain1=QString::number(x0);
        }
        if(diagramItem->var0==varMain2)
        {
            diagramItem->varAnswer2=QString::number(x0);
            varAnswerMain2=QString::number(x0);
        }
     }
    if(diagramItem->myDiagramType==Diagram::DiagramType::Loop)
    {

        bool okanswer0=false,okstart0=false,okend0=false,okstep0=false;
        int answerValue = diagramItem->varAnswer0.toInt(&okanswer0, 10);
        int startValue = diagramItem->varStartValue0.toInt(&okstart0, 10);
        int endValue = diagramItem->varEndValue0.toInt(&okend0, 10);
        int stepValue = diagramItem->varStepValue0.toInt(&okstep0, 10);

       /// qDebug()<<"loop process answer"<<answerValue<<"start"<<startValue<<"end"<<endValue<<"step"<<stepValue;
        if(diagramItem->counter==0) answerValue=startValue-stepValue;
        answerValue=answerValue+stepValue;
        diagramItem->counter++;
        diagramItem->varAnswer0=QString::number(answerValue);
        varLoopAnswerMain0=QString::number(answerValue);

    }
    /*************************************************/
    scene->update();
    timer.start(2000);
    loop.exec();
    diagramItem->setText("stop",QColor(255,255,0,0));
}
void MainWindow::stop()
{
    runState=false;
}
bool MainWindow::runTest()
{
    bool startItemHaveState=true;
    bool endItemHaveState=true;
    bool arrowHaveState=true;
    bool loopSetHaveState=true;
     foreach (QGraphicsItem *item, scene->items()) {
         if (item->type() == Arrow::Type) arrowHaveState=false;
         if (item->type() == DiagramItem::Type)
         {
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Start) startItemHaveState=false;
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::End) endItemHaveState=false;
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Loop)
             {
                 qgraphicsitem_cast<DiagramItem *>(item)->varAnswer0="0";
                 qgraphicsitem_cast<DiagramItem *>(item)->counter=0;

                 varLoopAnswerMain0="0";

                 if(qgraphicsitem_cast<DiagramItem *>(item)->label.text()=="")
                 {
                     loopSetHaveState=false;
                   //  qDebug()<<"loop test"<<qgraphicsitem_cast<DiagramItem *>(item)->label.text();
                 }
             }

         }
     }
     if(startItemHaveState)
     {
         QMessageBox::information (0, "Hata\t","Başla Nesnesi Yok.");return false;
     }

     if(endItemHaveState)
     {QMessageBox::information (0, "Hata\t","Son Nesnesi Yok.");return false; }

      if(arrowHaveState)
      {QMessageBox::information (0, "Hata\t","Bağlantı Yok.");return false;}

      if(loopSetHaveState==false)
      {QMessageBox::information (0, "Hata\t","Döngü Düzenlenmemiş.");return false;}

      return true;
}
void MainWindow::run()
{
    runAction->setEnabled(false);
     stopAction->setEnabled(true);
   // qDebug()<<"run:"<<runTest();
    if(runTest()==false){ stopAction->setEnabled(false);
                          runAction->setEnabled(true);return;}
    runState=true;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));



    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == DiagramItem::Type)
        {
            if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Start)
            {
                diagramItem=qgraphicsitem_cast<DiagramItem *>(item);
                worker();
            }
        }
    }

   do{
        if(runState){
        diagramItem=detectRouteItem(diagramItem);
           // worker();
        if(diagramItem!=0)worker();
            else{
            QMessageBox::information (0, "Hata\t","Akış nesnesini başka bir akış nesnesine bağlamamışsınız.\nBağlantılarınızı kontrol ediniz!!");
            runAction->setEnabled(true);
            stopAction->setEnabled(false);
            return;
        }

        }else break;

        //detectRouteItem(diagramItem)
    }while(diagramItem->myDiagramType!=Diagram::End);
    diagramItem=detectRouteItem(diagramItem);
    //worker();
    if(diagramItem!=0)worker();
        else{
        QMessageBox::information (0,"Hata\t","Akış nesnesini başka bir akış nesnesine bağlamamışsınız.\nBağlantılarınızı kontrol ediniz!!");
        runAction->setEnabled(true);
        stopAction->setEnabled(false);
        return;
    }
     runAction->setEnabled(true);
      stopAction->setEnabled(false);
}
DiagramItem* MainWindow::detectRouteItem(DiagramItem *item)
{
    qDebug()<<"Rota Kontrol..";
    ///burada rota belirlenmesi yapılıyor.
    /// burada kıyaslama işlemleri yapılıyor
if(item->myDiagramType==Diagram::End) return item;
    /***********************************************/
    bool result=true;
    if(item->myDiagramType==Diagram::DiagramType::Conditional)
    {
       //varMain0,varConditionalMain0,varOperatorMain0
        bool okvar=false,okoperator=false,okconditional=false,okanswermain=false;
        QString itemconditional,varanswermain;

        if(diagramItem->var0==varMain0)
        {
            itemconditional=item->varConditional0;
            varanswermain=varAnswerMain0;
        }
        if(diagramItem->var0==varMain1)
        {
            itemconditional=item->varConditional1;
            varanswermain=varAnswerMain1;

        }
        if(diagramItem->var0==varMain2)
        {
            itemconditional=item->varConditional2;
            varanswermain=varAnswerMain2;

        }
        int varConditional = itemconditional.toInt(&okconditional, 10);
        int varAnswerMain = varanswermain.toInt(&okanswermain, 10);

    //qDebug()<<item->var0<<item->varOperator0<<varAnswerMain<<varConditional;
     if(item->var0!=""&&item->varOperator0=="="&&varAnswerMain==varConditional)
         result=true;
     else if(item->var0!=""&&item->varOperator0=="<"&&varAnswerMain<varConditional)
     result=true;
     else if(item->var0!=""&&item->varOperator0==">"&&varAnswerMain>varConditional)
         result=true;
     else
         result=false;

     //qDebug()<<"result"<<result;
    }
    if(item->myDiagramType==Diagram::DiagramType::Loop)
    {
        bool okanswer0=false,okstart0=false,okend0=false,okstep0=false;
        int answerValue = diagramItem->varAnswer0.toInt(&okanswer0, 10);
        int startValue = diagramItem->varStartValue0.toInt(&okstart0, 10);
        int endValue = diagramItem->varEndValue0.toInt(&okend0, 10);
        int stepValue = diagramItem->varStepValue0.toInt(&okstep0, 10);
        if(answerValue==0) answerValue=startValue;

           if(answerValue<endValue)
         result=true;
     else
         result=false;

  /// qDebug()<<"loop test answer:"<<answerValue<<"end"<<endValue<<"result"<<result;

    }

    /***********************************************/
      if(item->startArrow!=0&&item->startArrowRota=="O"
            &&item->startArrow->answer=="Y"&&result==true)
   return item->startArrow->myEndItem;

    if(item->endArrow!=0&&item->endArrowRota=="O"
            &&item->endArrow->answer=="Y"&&result==true)
        return item->endArrow->myEndItem;

    if(item->leftArrow!=0&&item->leftArrowRota=="O"
            &&item->leftArrow->answer=="Y"&&result==true)
   return item->leftArrow->myEndItem;

    if(item->rightArrow!=0&&item->rightArrowRota=="O"
            &&item->rightArrow->answer=="Y"&&result==true)
   return item->rightArrow->myEndItem;

    /*****************************************/
    if(item->startArrow!=0&&item->startArrowRota=="O"
          &&item->startArrow->answer=="N"&&result==false)
 return item->startArrow->myEndItem;

  if(item->endArrow!=0&&item->endArrowRota=="O"
          &&item->endArrow->answer=="N"&&result==false)
      return item->endArrow->myEndItem;

  if(item->leftArrow!=0&&item->leftArrowRota=="O"
          &&item->leftArrow->answer=="N"&&result==false)
 return item->leftArrow->myEndItem;

  if(item->rightArrow!=0&&item->rightArrowRota=="O"
          &&item->rightArrow->answer=="N"&&result==false)
 return item->rightArrow->myEndItem;

    // qDebug()<<"right"<<item->rightArrow->answer;
  item=0;

    return item;
}

///! [21]
void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Başla"), Diagram::Start), 0, 0);
    layout->addWidget(createCellWidget(tr("Son"), Diagram::End), 0, 1);
    layout->addWidget(createCellWidget(tr("İşlem"), Diagram::Process),1, 0);
    layout->addWidget(createCellWidget(tr("Giriş"), Diagram::Input), 1, 1);
    layout->addWidget(createCellWidget(tr("Karar"), Diagram::Conditional), 2, 0);
     layout->addWidget(createCellWidget(tr("Bağlantı"), Diagram::Link), 2,1);
    layout->addWidget(createCellWidget(tr("Çıktı"), Diagram::Output), 3,0);
    layout->addWidget(createCellWidget(tr("Döngü"), Diagram::Loop), 3,1);

//!
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    textButton->setFixedSize(75,40);
    textButton->setAutoRaise(true);

    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 25));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    textLayout->setContentsMargins(1,0, 0,0);
    textLayout->setSpacing(1);

    QWidget *textWidget = new QWidget;
    textWidget->setFixedSize(80,60);
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 8, 0);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);
    layout->setContentsMargins(1,0, 0,0);
    layout->setSpacing(1);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Mavi Izgara"),
                                                           ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Beyaz Izgara"),
                                                           ":/images/background2.png"),1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gri Izgara"),
                                                           ":/images/background3.png"), 2, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Izgarasız"),
                                                           ":/images/background4.png"),3,0);

    //backgroundLayout->setRowStretch(2, 10);
   // backgroundLayout->setColumnStretch(2, 10);
    backgroundLayout->setContentsMargins(1,0, 0,0);
    backgroundLayout->setSpacing(1);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


//! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Akış Şekilleri"));
    toolBox->addItem(backgroundWidget, tr("Arka Planlar"));
}
//! [22]

//! [23]
void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Öne Ge&tir"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Öne Getir"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Al&ta Al"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Alta Al"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Sil"), this);
    deleteAction->setShortcut(tr("Sil"));
    deleteAction->setStatusTip(tr("Akış Nesnesi Siler"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));


    runAction = new QAction(QIcon(":/images/run.png"), tr("&Çalıştır"), this);
    runAction->setShortcut(tr("Çalıştır"));
    runAction->setStatusTip(tr("Akış Diyagramını Çalıştırır"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(run()));

    stopAction = new QAction(QIcon(":/images/stop.png"), tr("&Dur"), this);
    stopAction->setShortcut(tr("Dur"));
    stopAction->setStatusTip(tr("ÇAlışmayı Durdurur"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));


    exitAction = new QAction(tr("Ç&ıkış"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Uygulmadan Çıkılır"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Kalın"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italik"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Altı Çizgili"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("H&akkında"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

   }

//! [24]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Dosya"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Düzen"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    runMenu = menuBar()->addMenu(tr("&Çalıştır"));
    runMenu->addAction(runAction);
    runMenu->addAction(stopAction);


    aboutMenu = menuBar()->addMenu(tr("&Yardım"));
    aboutMenu->addAction(aboutAction);
   // aboutMenu->addAction(runAction);


}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
//! [25]
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
    editToolBar->addAction(runAction);
    editToolBar->addAction(stopAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(1);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
//! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text, Diagram::DiagramType type)
{

    // DiagramItem item(type, itemMenu);
     Diagram *item=new Diagram();

     QIcon icon(item->image(type));

     QToolButton *button = new QToolButton;
     button->setIcon(icon);
     button->setIconSize(QSize(50, 50));
     button->setFixedSize(75,50);
     button->setCheckable(true);
     button->setAutoRaise(true);
     buttonGroup->addButton(button, int(type));

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(button, 0, 0, Qt::AlignHCenter);
     layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
     layout->setContentsMargins(1,0, 0,0);
     layout->setSpacing(0);

     QWidget *widget = new QWidget;
     widget->setFixedSize(80,70);
     widget->setLayout(layout);

     return widget;
}
//! [29]

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]
