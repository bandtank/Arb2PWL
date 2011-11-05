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
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pMouseEvent);

signals:
    void AddedLine(const QLineF& Line);
  
protected:
    bool MouseIsDown;
    bool Drawing;
    QPointF LastClick;
    QPointF NextValidX;
};

#endif
