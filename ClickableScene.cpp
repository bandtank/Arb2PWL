#include "ClickableScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <iostream>

ClickableScene::ClickableScene(QWidget *parent) : QGraphicsScene()
{
  this->MouseIsDown = false;
}

void ClickableScene::mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    this->MouseIsDown = true;

    //Create a dot at this point
    QRectF Dot(pMouseEvent->scenePos().x(),pMouseEvent->scenePos().y(),0,0);
    addRect(Dot);

    //Create the first point in a line
    this->LastClick.setX(pMouseEvent->scenePos().x());
    this->LastClick.setY(pMouseEvent->scenePos().y());
}

void ClickableScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    this->MouseIsDown = false;
}

void ClickableScene::mouseMoveEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    if(this->MouseIsDown == true)
    {
        qreal x;
        if(pMouseEvent->scenePos().x() < this->LastClick.x())
            x = this->LastClick.x();
        else
            x = pMouseEvent->scenePos().x();


        qreal y = pMouseEvent->scenePos().y();

        QLineF Line(this->LastClick.x(),this->LastClick.y(),x,y);
        addLine(Line);

        this->LastClick.setX(x);
        this->LastClick.setY(y);
    }
}
