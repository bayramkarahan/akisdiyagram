/*****************************************************************************
 *   Copyright (C) 2020 by Bayram KARAHAN                                    *
 *   <bayramk@gmail.com>                                                     *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/

#include "verectangle.h"
#include <QPainter>
#include <QDebug>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
//#include <QGraphicsRectItem>
#include <math.h>
#include "dotsignal.h"
#include<QPen>
#include<QStyle>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

VERectangle::VERectangle(QGraphicsScene *_scn) :
    scn(_scn),
    m_cornerFlags(0),
    m_actionFlags(ResizeState),
    drm(false),
    dclick(false)
{
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    for(int i = 0; i < 8; i++){
        cornerGrabber[i] = new DotSignal(this);
    }
    setPositionGrabbers();
}

VERectangle::~VERectangle()
{
    for(int i = 0; i < 8; i++){
        delete cornerGrabber[i];
    }
}

QPointF VERectangle::previousPosition() const
{
    return m_previousPosition;
}

void VERectangle::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}

void VERectangle::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x,y,w,h));
}

void VERectangle::setRect(const QRectF &rect)
{
    QGraphicsRectItem::setRect(rect);
    if(brush().gradient() != 0){
        const QGradient * grad = brush().gradient();
        if(grad->type() == QGradient::LinearGradient){
            auto tmpRect = this->rect();
            const QLinearGradient *lGradient = static_cast<const QLinearGradient *>(grad);
            QLinearGradient g = *const_cast<QLinearGradient*>(lGradient);
            g.setStart(tmpRect.left() + tmpRect.width()/2,tmpRect.top());
            g.setFinalStop(tmpRect.left() + tmpRect.width()/2,tmpRect.bottom());
            setBrush(g);
        }
    }
}

void VERectangle::fareState(const bool _drm)
{
drm=_drm;
/************************************************/

m_actionFlags =RotationState;
setVisibilityGrabbers();

if(drm){
    dclick=true;
    setPositionGrabbers();
    setVisibilityGrabbers();
  // if(sekilTr==DiagramIte::DiagramType::Pdf) hideGrabbers();
    //qDebug() <<"rectangle nesnesi seçildi--";
}
else{
    dclick=false;
m_cornerFlags = 0;
hideGrabbers();
setCursor(Qt::ArrowCursor);
  //  qDebug() <<"rectangle nesnesi seçim kaldırıldı";
}
/*************************************************/
update();
}

void VERectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
 //   qDebug() <<"move yapıldı";
  //  qDebug() <<"üzerine geldi"<<drm;
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
         //   resizeTop(pt);
           // resizeRight(pt);
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
        switch (m_cornerFlags) {
        case TopLeft:{
            resizeLeft(pt);
            resizeTop(pt);
            break;
        }
        case TopRight:{
            rotateItem(pt);

            break;
        }
        case BottomLeft: {
          //  Scene * _scene = dynamic_cast<Scene *>(parent);
///if(sekilTr!=DiagramItem::DiagramType::Pdf) scn->removeItem(this);


          //  qDebug() <<"siliniyorrr";

           break;
        }
        case BottomRight: {
            resizeBottom(pt);
            resizeRight(pt);
            //rotateItem(pt);
            break;
        }
        default:
           if (m_leftMouseButtonPressed) {
               //qDebug() <<"gidiyor";
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - m_previousPosition.x();
                auto dy = event->scenePos().y() - m_previousPosition.y();
                moveBy(dx,dy);
                setPreviousPosition(event->scenePos());
                emit signalMove(this, dx, dy);
            }

            break;
        }
    }
   }
   scn->update();
  //  QGraphicsItem::mouseMoveEvent(event);
}

void VERectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());
        emit clicked(this);
       // qDebug() <<"rectangle nesnesine tıklama yapıldı";


    }
  //  QGraphicsItem::mousePressEvent(event);
//QGraphicsScene::mousePressEvent(event);
}

void VERectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void VERectangle::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() <<"çift tıklama1 "<<rotateState;


  /*  m_actionFlags = (m_actionFlags == ResizeState)?RotationState:ResizeState;
    setVisibilityGrabbers();
    dclick=(!dclick)?true:false;
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
    qDebug() <<"çift tıklama "<<dclick;
    */
 //   QGraphicsItem::mouseDoubleClickEvent(event);
}

void VERectangle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  /*  qDebug() <<"üzerine geldi"<<drm;
   if(drm)
   {
    setPositionGrabbers();
    setVisibilityGrabbers();
    QGraphicsItem::hoverEnterEvent(event);
   }*/
}

void VERectangle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

  //   qDebug() <<"üzerine ayrıldı"<<drm;
  /*
     m_cornerFlags = 0;
    hideGrabbers();
    setCursor(Qt::CrossCursor);
    drm=false;

    QGraphicsItem::hoverLeaveEvent( event );
*/
}

void VERectangle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(drm&&dclick)
    {
    //qDebug() <<"üzerindesin";
    QPointF pt = event->pos();              // The current position of the mouse
    qreal drx = pt.x() - rect().right();    // Distance between the mouse and the right
    qreal dlx = pt.x() - rect().left();     // Distance between the mouse and the left

    qreal dby = pt.y() - rect().top();      // Distance between the mouse and the top
    qreal dty = pt.y() - rect().bottom();   // Distance between the mouse and the bottom

    // If the mouse position is within a radius of 7
    // to a certain side( top, left, bottom or right)
    // we set the Flag in the Corner Flags Register

    m_cornerFlags = 0;
    int alan=25;
    if( (dby < alan && dby > -alan)||(dby < -alan && dby > alan) ) m_cornerFlags |= Top;       // Top side
    if( (dty < alan && dty > -alan)|| (dty < -alan && dty > alan)) m_cornerFlags |= Bottom;    // Bottom side
    if( (drx < alan && drx > -alan )||(drx < -alan && drx > alan )) m_cornerFlags |= Right;     // Right side
    if( (dlx < alan && dlx > -alan)||  (dlx < -alan && dlx > alan)) m_cornerFlags |= Left;      // Left side
    QPixmap p(":/icons/arrow-up-down.png");
    QPixmap px(":/icons/rotate-right.png");
    QPixmap pd(":/icons/erase.png");

    QPixmap pResult;
    QTransform trans = transform();
    if(m_actionFlags == ResizeState){


        switch (m_cornerFlags) {
        case Top:
        case Bottom:
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;
        case Left:
        case Right:
            trans.rotate(90);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;
        case TopRight:

            trans.rotate(0);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;

        case BottomLeft:
            trans.rotate(45);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;
        case TopLeft:

        case BottomRight:

            trans.rotate(135);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    } else {
        switch (m_cornerFlags) {
        case TopLeft:
        {
            trans.rotate(135);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;

        }
        case TopRight: {
                   // trans.rotate(135);
                    pResult = px.transformed(trans);
                    setCursor(px.scaled(40,40,Qt::KeepAspectRatio));
                    break;
                }
        case BottomLeft:
        {
                  //  trans.rotate(135);
                    pResult = pd.transformed(trans);
                    setCursor(pResult.scaled(30,30,Qt::KeepAspectRatio));
                    break;
                }

        case BottomRight: {
            trans.rotate(135);
            pResult = p.transformed(trans);
            setCursor(pResult.scaled(24,24,Qt::KeepAspectRatio));
            break;
        }
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    }
    }
    QGraphicsItem::hoverMoveEvent( event );
}

QVariant VERectangle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemSelectedChange:
        m_actionFlags = ResizeState;
        break;
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void VERectangle::resizeLeft(const QPointF &pt)
{
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
}

void VERectangle::resizeRight(const QPointF &pt)
{
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
}

void VERectangle::resizeBottom(const QPointF &pt)
{
    QRectF tmpRect = rect();
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
}

void VERectangle::resizeTop(const QPointF &pt)
{
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
}

void VERectangle::rotateItem(const QPointF &pt)
{
    QRectF tmpRect = rect();
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
}

void VERectangle::setPositionGrabbers()
{
    QRectF tmpRect = rect();
    cornerGrabber[GrabberTop]->setPos(tmpRect.left() + tmpRect.width()/2, tmpRect.top());
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTop]->setZValue(-100);
    cornerGrabber[GrabberTop]->setDotFlags(GrabberTop);

    cornerGrabber[GrabberBottom]->setPos(tmpRect.left() + tmpRect.width()/2, tmpRect.bottom());
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottom]->setDotFlags(GrabberBottom);

    cornerGrabber[GrabberLeft]->setPos(tmpRect.left(), tmpRect.top() + tmpRect.height()/2);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberLeft]->setDotFlags(GrabberLeft);


    cornerGrabber[GrabberRight]->setPos(tmpRect.right(), tmpRect.top() + tmpRect.height()/2);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberRight]->setDotFlags(GrabberRight);


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

void VERectangle::setVisibilityGrabbers()
{
    cornerGrabber[GrabberTopLeft]->setVisible(true);
    cornerGrabber[GrabberTopRight]->setVisible(true);
    cornerGrabber[GrabberBottomLeft]->setVisible(true);
    cornerGrabber[GrabberBottomRight]->setVisible(true);

    if(m_actionFlags == ResizeState){
        cornerGrabber[GrabberTop]->setVisible(true);
        cornerGrabber[GrabberBottom]->setVisible(true);
        cornerGrabber[GrabberLeft]->setVisible(true);
        cornerGrabber[GrabberRight]->setVisible(true);
    } else {
        cornerGrabber[GrabberTop]->setVisible(false);
        cornerGrabber[GrabberBottom]->setVisible(false);
        cornerGrabber[GrabberLeft]->setVisible(false);
        cornerGrabber[GrabberRight]->setVisible(false);
    }
}

void VERectangle::hideGrabbers()
{
    for(int i = 0; i < 8; i++){
        cornerGrabber[i]->setVisible(false);
    }
}


void VERectangle::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(drm&&!dclick)
    {
/*QStyle::State_Selected;
 painter->setPen(QPen(QColor(0,0,0,255),0,Qt::DashLine));
 painter->setBrush(Qt::NoBrush);
 painter->drawPath(shape());
       // QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
*/
 hideGrabbers();
    }
    /*
     The drop shadow effect will be created by drawing a filled, rounded corner rectangle with a gradient fill.
     Then on top of this will be drawn  filled, rounded corner rectangle, filled with a solid color, and offset such that the gradient filled
     box is only visible below for a few pixels on two edges.

     The total box size is _width by _height. So the top box will start at (0,0) and go to (_width-shadowThickness, _height-shadowThickness),
     while the under box will be offset, and start at (shadowThickness+0, shadowThickness+0) and go to  (_width, _height).
       */

    //int shadowThickness = 3;

   // QLinearGradient gradient;
    //gradient.setStart(_drawingOrigenX,_drawingOrigenY);
 //   gradient.setFinalStop( _drawingWidth ,_drawingOrigenY);
   // QColor grey1(150,150,150,0);// starting color of the gradient - can play with the starting color and ,point since its not visible anyway

    // grey2 is ending color of the gradient - this is what will show up as the shadow. the last parameter is the alpha blend, its set
    // to 125 allowing a mix of th color and and the background, making more realistic shadow effect.
   // QColor grey2(225,225,225,0);


   // gradient.setColorAt((qreal)0, grey1 );
  //  gradient.setColorAt((qreal)1, grey2 );

   // QBrush brush(gradient);

    painter->setBrush(this->brush());

    // for the desired effect, no border will be drawn, and because a brush was set, the drawRoundRect will fill the box with the gradient brush.
    //_outterborderPen.setStyle(Qt::SolidPen);
   painter->setPen(this->pen());
  int  _drawingOrigenX=boundingRect().topLeft().x();
   int _drawingOrigenY=boundingRect().topLeft().y();
    int _drawingWidth=boundingRect().topLeft().x()+boundingRect().width();
    int _drawingHeight=boundingRect().topLeft().y()+boundingRect().height();

    QPointF topLeft (_drawingOrigenX,_drawingOrigenY);
    QPointF bottomRight ( _drawingWidth , _drawingHeight);

    QRectF rect (topLeft, bottomRight);

    //painter->drawRoundRect(rect,0,0); // corner radius of 25 pixels

    // draw the top box, the visible one
  //  QBrush brush2(QColor(243,255,216,255),Qt::SolidPattern);

    //painter->setBrush( brush2);

 //   QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
  //  QPointF bottomRight2 ( _drawingWidth, _drawingHeight);

 //   QRectF rect2 (topLeft2, bottomRight2);

  //  painter->drawRoundRect(rect2,0,0);

   /*
    //Üçgen
    QPolygon result(3);
    result.setPoint(0, rect.center().x(), rect.top());
    result.setPoint(1, rect.right(), rect.bottom());
    result.setPoint(2, rect.left(), rect.bottom());
*/
   /*
    //Yamuk
    QPolygon result(4);
    result.setPoint(0, rect.left(), rect.top());
    result.setPoint(1, rect.right()/2, rect.top());
    result.setPoint(2, rect.right(), rect.bottom());
    result.setPoint(3, rect.left(), rect.bottom());
    */
   /*
    //Dörtgen
    QPolygon result(4);
    result.setPoint(0, rect.left(), rect.top());
    result.setPoint(1, rect.right(), rect.top());
    result.setPoint(2, rect.right(), rect.bottom());
    result.setPoint(3, rect.left(), rect.bottom());
*/
   /*
    //Pararel Kenar
    QPolygon result(4);
    result.setPoint(0, rect.center().x()/2, rect.top());
    result.setPoint(1, rect.right(), rect.top());
    result.setPoint(2, rect.center().x()+rect.center().x()/2, rect.bottom());
    result.setPoint(3, rect.left(), rect.bottom());
*/
 /*
    //Dik Pararel Kenar
    QPolygon result(4);
    result.setPoint(0, rect.center().x(), rect.top());
    result.setPoint(1, rect.right(), rect.top()+rect.height()/2);
    result.setPoint(2, rect.center().x(), rect.bottom());
    result.setPoint(3, rect.left(), rect.top()+rect.height()/2);
*/

    //Dik Pararel Kenar
   /* QPolygon result(4);
    result.setPoint(0, rect.center().x(), rect.top());
    result.setPoint(1, rect.right(), rect.top()+rect.height()/2);
    result.setPoint(2, rect.center().x(), rect.bottom());
    result.setPoint(3, rect.left(), rect.top()+rect.height()/2);
*/

    painter->setRenderHint(QPainter::Antialiasing);
  //  painter->drawPolygon(triangle(result));
   // painter->drawPolygon(result);


    Diagram *ditem=new Diagram();
        painter->drawPolygon(ditem->sekilStore(sekilTr,rect));
     painter->setRenderHint(QPainter::Antialiasing, false);

}
