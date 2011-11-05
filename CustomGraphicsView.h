#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class CustomGraphicsView: public QGraphicsView
{
  Q_OBJECT

  void resizeEvent ( QResizeEvent * event )
  {
    emit resized();
  }

signals:
  void resized();
};

#endif
