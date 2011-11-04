#ifndef FORM_H
#define FORM_H

#include "ui_form.h"

#include "ClickableScene.h"

class Form : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        Form(QWidget *parent = 0);

    public slots:
      void slot_AddedPoint(const QRectF& Point);
      void on_actionSave_activated();

    protected:
      ClickableScene* Scene;

      std::vector<QRectF> Points;

      std::string PointToString(const QRectF& Point);
};

#endif
