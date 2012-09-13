/*=========================================================================
 *
 *  Copyright Anthony Andriano, David Doria 2012
 *  anthonyandriano@gmail.com daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef Arb2PWLWidget_H
#define Arb2PWLWidget_H

#include "ui_Arb2PWLWidget.h"

#include "ClickableScene.h"
#include "CustomGraphicsView.h"

class Arb2PWLWidget : public QMainWindow, private Ui::Arb2PWLWidget
{
    Q_OBJECT

    public:
      /** Constructor. */
      Arb2PWLWidget(QWidget *parent = 0);

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
