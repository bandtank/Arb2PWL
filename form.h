#ifndef FORM_H
#define FORM_H

#include "ui_form.h"

#include "ClickableScene.h"
#include "CustomGraphicsView.h"

class Form : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
      /** Constructor. */
      Form(QWidget *parent = 0);

    public slots:
      /** Handle adding a new line. */
      void slot_AddedLine(const QLineF& Line);

      /** Save the image. */
      void on_actionSave_activated();

      /** Select an image to open. */
      void on_actionOpenImageForTracing_activated();

      /** Fit the image to the view. */
      void slot_SizeImage();

      /** Quit the program. */
      void slot_actionExit_activated();

    protected:
      /** Do things when the widget is first displayed. */
      void showEvent ( QShowEvent * event );

      /** Do things when the widget is resized. */
      void resizeEvent ( QResizeEvent * event );

      /** The scene. */
      ClickableScene* Scene;

      /** The view. */
      CustomGraphicsView* View;

      /** A collection of the lines that have been drawn so far. */
      std::vector<QLineF> Lines;
      /** The image we want to trace. */
      QPixmap ImageToTrace;

      /** The item of the image we want to trace. */
      QGraphicsPixmapItem* ImageToTraceItem;

};

/** Output a point. */
std::ostream& operator<<(std::ostream& output, const QLineF &point);

/** Output a line. */
std::ostream& operator<<(std::ostream& output, const QLineF &line);

#endif
