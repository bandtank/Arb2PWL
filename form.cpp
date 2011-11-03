#include <QtGui>
#include <QLineF>

#include <iostream>

#include "form.h"

Form::Form(QWidget *parent) : QMainWindow(parent)
{
  setupUi(this);
  
  QColor sceneBackgroundColor;
  sceneBackgroundColor.setRgb(153, 255, 0);
  
  this->Scene = new ClickableScene();
  this->Scene->setSceneRect(this->graphicsView->rect());
  
  // Set background color
//   QBrush brush;
//   brush.setStyle(Qt::SolidPattern);
//   brush.setColor(sceneBackgroundColor);
//   this->Scene->setBackgroundBrush(brush);
  
  this->graphicsView->setScene(this->Scene);
  
}
