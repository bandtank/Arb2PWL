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

    x.setRect(pMouseEvent->scenePos().x(),pMouseEvent->scenePos().y(),1,1);
    addRect(x);
}

void ClickableScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    this->MouseIsDown = false;
}

void ClickableScene::mouseMoveEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    if(this->MouseIsDown == true)
    {
        x.setRect(pMouseEvent->scenePos().x(),pMouseEvent->scenePos().y(),1,1);
        addRect(x);
    }
}
