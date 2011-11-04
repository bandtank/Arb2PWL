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
    void AddedPoint(const QRectF& Point);
  
protected:
    bool MouseIsDown;
    QRectF x;
};

#endif
