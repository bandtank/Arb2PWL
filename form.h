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
  void slot_AddedLine(const QLineF& line);
  void on_actionSave_activated();
  
protected:
  ClickableScene* Scene;
  
  std::vector<QLineF> Lines;
  
  std::string PointToString(const QPointF& point);
};

#endif
