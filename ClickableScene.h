#ifndef ClickableScene_H
#define ClickableScene_H

#include <QGraphicsScene>
#include <QMouseEvent>

class ClickableScene : public QGraphicsScene
{
  Q_OBJECT
public:
  ClickableScene(QWidget *parent = 0);
  
  void mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent);

signals:
  void AddedLine(const QLineF& line);
  
protected:
  QPointF LastClick;
  bool ValidLastClick;
};

#endif
