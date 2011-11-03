#include "ClickableScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include <iostream>

ClickableScene::ClickableScene(QWidget *parent) : QGraphicsScene()
{
  this->ValidLastClick = false;
  this->LastClick.setX(0);
  this->LastClick.setY(0);
}

void ClickableScene::mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
  if(this->ValidLastClick)
    {
    QLineF line(this->LastClick.x(), this->LastClick.y(), pMouseEvent->scenePos().x(), pMouseEvent->scenePos().y());
    this->addLine(line);
    emit AddedLine(line);
    }
  
  this->LastClick.setX(pMouseEvent->scenePos().x());
  this->LastClick.setY(pMouseEvent->scenePos().y());
  this->ValidLastClick = true;
}
