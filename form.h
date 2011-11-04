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
      void slot_AddedLine(const QLineF& Line);
      void on_actionSave_activated();
      void on_pushButton_clicked();

    protected:
      ClickableScene* Scene;

      std::vector<QLineF> Lines;

      std::string PointToString(const QLineF& Lines);
};

#endif
