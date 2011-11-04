#include <QtGui>
#include <QLineF>

#include <fstream>
#include <iostream>
#include <sstream>

#include "form.h"

Form::Form(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    QColor sceneBackgroundColor;
    sceneBackgroundColor.setRgb(153, 255, 0);

    this->Scene = new ClickableScene();
    this->Scene->setSceneRect(this->graphicsView->rect());

    //   Set background color
    //   QBrush brush;
    //   brush.setStyle(Qt::SolidPattern);
    //   brush.setColor(sceneBackgroundColor);
    //   this->Scene->setBackgroundBrush(brush);

    this->graphicsView->setScene(this->Scene);

    this->connect(this->Scene, SIGNAL(AddedLine(QLineF)), SLOT(slot_AddedLine(QLineF)));
}

void Form::slot_AddedLine(const QLineF& Line)
{
    this->Lines.push_back(Line);
    std::cout << "There are now " << this->Lines.size() << " lines." << std::endl;
}

void Form::on_actionSave_activated()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", ".", "Text Files (*.txt)");

    std::cout << "Saving points to filename: " << fileName.toStdString() << std::endl;

    std::ofstream fout(fileName.toStdString().c_str());

    //Write the first point of each line
    //for(unsigned int i = 0; i < this->Points.size(); ++i)
    //{
        //fout << PointToString(this->Points[i].p1()) << std::endl;
    //}

    // Write the second point of the last line
    //fout << PointToString(this->Points[this->Points.size() - 1].p2()) << std::endl;
    fout.close();
}

std::string Form::PointToString(const QLineF& Line)
{
    std::stringstream pointStringStream;
    //pointStringStream << "(" << Point.x() << ", " << Point.y() << ")";
    return pointStringStream.str();
}
