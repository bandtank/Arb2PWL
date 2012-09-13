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

// Qt
#include <QtGui>
#include <QLineF>

// STL
#include <fstream>
#include <iostream>
#include <sstream>

// Custom
#include "Arb2PWLWidget.h"
//#include "MooreTracing.h"

Arb2PWLWidget::Arb2PWLWidget(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    this->Scene = new ClickableScene;

    // Manually size the scene. The scene should be fixed size for easier computations later.
    QRect rect(0,0,100,100); // (x, y, width, height)
    this->Scene->setSceneRect(rect);

    this->View = new CustomGraphicsView;
    this->gridLayout->addWidget(this->View);

    this->View->setScene(this->Scene);

    this->ImageToTraceItem = NULL;

    QIcon saveIcon = QIcon::fromTheme("document-save");
    actionSave->setIcon(saveIcon);
    this->toolBar->addAction(actionSave);

    QIcon openIcon = QIcon::fromTheme("document-open");
    actionOpenImageForTracing->setIcon(openIcon);
    this->toolBar->addAction(actionOpenImageForTracing);
  
    this->connect(this->Scene, SIGNAL(AddedLine(QLineF)), SLOT(slot_AddedLine(QLineF)));
    this->connect(this->View, SIGNAL(resized()), SLOT(slot_SizeImage()));
}

void Arb2PWLWidget::showEvent ( QShowEvent * event )
{
  this->View->fitInView(this->Scene->sceneRect(), Qt::KeepAspectRatio);
}

void Arb2PWLWidget::slot_AddedLine(const QLineF& Line)
{
    this->Lines.push_back(Line);
    std::cout << "There are now " << this->Lines.size() << " lines." << std::endl;
}

void Arb2PWLWidget::on_actionSave_activated()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", ".", "Text Files (*.txt)");

    std::cout << "Saving points to filename: " << fileName.toStdString() << std::endl;

    std::ofstream fout(fileName.toStdString().c_str());

    //Write the first point of each line
    //for(unsigned int i = 0; i < this->Points.size(); ++i)
    //{
        //fout << this->Points[i].p1() << std::endl;
    //}

    // Write the second point of the last line
    //fout << this->Points[this->Points.size() - 1].p2() << std::endl;
    fout.close();
}

void Arb2PWLWidget::on_actionOpenImageForTracing_activated()
{
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this, "Open", ".", "All Files (*.*)");

  this->ImageToTrace.load(fileName);

  this->ImageToTrace = this->ImageToTrace.scaled(this->Scene->sceneRect().size().toSize());

  this->ImageToTraceItem = this->Scene->addPixmap(this->ImageToTrace);
  this->statusBar()->showMessage("Opened " + fileName + " to trace.");

  slot_SizeImage();
}


void Arb2PWLWidget::slot_SizeImage()
{
  if(this->ImageToTraceItem)
    {
//    std::cout << "Sized image." << std::endl;
    this->View->fitInView(this->ImageToTraceItem);
    }
}

void Arb2PWLWidget::slot_actionExit_activated()
{
  QApplication::quit();
}

void Arb2PWLWidget::resizeEvent ( QResizeEvent * event )
{

}

std::ostream& operator<<(std::ostream& output, const QPointF &point)
{
  output << "(" << point.x() << ", " << point.y() << ")";

  return output;
}

std::ostream& operator<<(std::ostream& output, const QLineF &line)
{
  output << "P1: " << line.p1() << " P1: " << line.p2();

  return output;
}
