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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include<QDebug>
#include<QStaticText>
#include<diagram.h>
#include<QLabel>
#include<QTimer>
#include<dotsignal.h>
#include <QObject>
#include<QVBoxLayout>
#include<QComboBox>
#include<QDialogButtonBox>
extern QString varMain0;
extern QString varMain1;
extern QString varMain2;

extern QString varAnswerMain0;
extern QString varAnswerMain1;
extern QString varAnswerMain2;

extern QString varOperatorMain0;
extern QString varOperatorMain1;
extern QString varOperatorMain2;

extern QString varConditionalMain0;
extern QString varConditionalMain1;
extern QString varConditionalMain2;

extern QString varLoopAnswerMain0;
extern QString varLoopAnswerMain1;
extern QString varLoopAnswerMain2;


QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;

//! [0]
class DiagramItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    enum { Type = UserType + 15 };
   /// enum DiagramType { Step, Conditional, StartEnd, Io };
    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);

 enum ActionStates {
    ResizeState = 0x01,
    RotationState = 0x02
};
 enum CornerFlags {
     Top = 0x01,
     Bottom = 0x02,
     Left = 0x04,
     Right = 0x08,
     TopLeft = Top|Left,
     TopRight = Top|Right,
     BottomLeft = Bottom|Left,
     BottomRight = Bottom|Right
 };
    enum CornerGrabbers {
        GrabberTop = 0,
        GrabberBottom,
        GrabberLeft,
        GrabberRight,
        GrabberTopLeft,
        GrabberTopRight,
        GrabberBottomLeft,
        GrabberBottomRight
    };
    DotSignal *cornerGrabber[8];

    DiagramItem(Diagram::DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = 0);

    void removeArrow(Arrow *arrow);
    void removeArrowPolar(Arrow *arrow,QString polar);

    void removeArrows();
    Diagram::DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    bool addArrowState(Arrow *arrow, QString polar, QString rota);
    void setText(QString text, QColor color);

    QPixmap image() const;
    int type() const override { return Type;}
    Diagram::DiagramType myDiagramType;
    QList<Arrow *> arrows;
    QColor myBackground;
    QStaticText label;
    Arrow *startArrow;
    Arrow *endArrow;
    Arrow *leftArrow;
    Arrow *rightArrow;
    QString startArrowRota="";
    QString endArrowRota="";
    QString leftArrowRota="";
    QString rightArrowRota="";
    QString var0="";
    QString var1="";
    QString var2="";
    QString varAnswer0="";
    QString varAnswer1="";
    QString varAnswer2="";
    QString varOperator0="";
    QString varOperator1="";
    QString varOperator2="";
    QString varConditional0="";
    QString varConditional1="";
    QString varConditional2="";

    //loop
    QString varStartValue0="";
    QString varEndValue0="";
    QString varStepValue0="";
    bool input0=false;
    bool input1=false;
    bool input2=false;

    int counter=0;
    bool renkdrm;

  public slots:
      void renk();
signals:
    void rectChanged(DiagramItem *rect);
    void previousPositionChanged();
    void clicked(DiagramItem *rect);
    void signalMove(QGraphicsItem *item, qreal dx, qreal dy);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    unsigned int m_cornerFlags;
    unsigned int m_actionFlags;

    bool drm;

    bool rotateState=false;
    bool m_leftMouseButtonPressed;
    QPointF m_previousPosition;

    QPolygonF myPolygon;
    QMenu *myContextMenu;

    void setPositionGrabbers();
    void setVisibilityGrabbers();
    void hideGrabbers();

    void resizeLeft( const QPointF &pt);
    void resizeRight( const QPointF &pt);
    void resizeBottom(const QPointF &pt);
    void resizeTop(const QPointF &pt);
    void rotateItem(const QPointF &pt);

};
//! [0]

#endif // DIAGRAMITEM_H
