#ifndef FORM_H
#define FORM_H

#include "ui_form.h"

#include "ClickableScene.h"
#include "CustomGraphicsView.h"

class Form : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        Form(QWidget *parent = 0);

    public slots:
      void slot_AddedLine(const QLineF& Line);
      void on_actionSave_activated();
      void on_actionOpenImageForTracing_activated();
      void slot_SizeImage();
      void slot_actionExit_activated();

    protected:
      ClickableScene* Scene;
      CustomGraphicsView* View;

      std::vector<QLineF> Lines;

      std::string PointToString(const QLineF& Lines);

      QPixmap ImageToTrace;
      QGraphicsPixmapItem* ImageToTraceItem;

      void Trace(const std::string& fileName);
};

#endif
