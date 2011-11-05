// Qt
#include <QtGui>
#include <QLineF>

// STL
#include <fstream>
#include <iostream>
#include <sstream>

// ITK
#include "itkBinaryThresholdImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkImageFileReader.h"

// Custom
#include "form.h"
#include "MooreTracing.h"

Form::Form(QWidget *parent) : QMainWindow(parent)
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
    actionOpenImageForTracing->setIcon(saveIcon);
    this->toolBar->addAction(actionOpenImageForTracing);
  
    this->connect(this->Scene, SIGNAL(AddedLine(QLineF)), SLOT(slot_AddedLine(QLineF)));
    this->connect(this->View, SIGNAL(resized()), SLOT(slot_SizeImage()));
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

void Form::on_actionOpenImageForTracing_activated()
{
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this, "Open", ".", "All Files (*.*)");

  this->ImageToTrace.load(fileName);

  this->ImageToTrace = this->ImageToTrace.scaled(this->Scene->sceneRect().size().toSize());

  this->ImageToTraceItem = this->Scene->addPixmap(this->ImageToTrace);
  this->statusBar()->showMessage("Opened " + fileName + " to trace.");

  slot_SizeImage();
}


void Form::slot_SizeImage()
{
  if(this->ImageToTraceItem)
    {
    std::cout << "Sized image." << std::endl;
    this->View->fitInView (this->ImageToTraceItem);
    }
}

void Form::slot_actionExit_activated()
{
  exit(0);
}

typedef itk::CovariantVector<unsigned char, 3> VectorRGB;

void Form::Trace(const std::string& fileName)
{
  typedef itk::Image<VectorRGB, 2>  RGBImageType;

  typedef itk::Image<unsigned char, 2>  ScalarImageType;


  typedef itk::ImageFileReader<RGBImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);
  reader->Update();

  // Use the color of the selection to threshold the image (looking for similar colors)
  typedef itk::BinaryThresholdImageFilter <RGBImageType, ScalarImageType> BinaryThresholdImageFilterType;

  RGBImageType::PixelType lowerVector;
  lowerVector.Fill(50);

  RGBImageType::PixelType upperVector;
  upperVector.Fill(100);

  BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(reader->GetOutput());
  thresholdFilter->SetLowerThreshold(lowerVector);
  thresholdFilter->SetUpperThreshold(upperVector);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);
  thresholdFilter->Update();

  // Use the location of the selection to get pixels that fit the color criterion and that are connected to the selected point.
  typedef itk::ConnectedThresholdImageFilter<ScalarImageType, ScalarImageType> ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(100);
  connectedThreshold->SetUpper(200);
  connectedThreshold->SetReplaceValue(255);

  itk::Index<2> seed;
  seed[0] = 5;
  seed[1] = 5;

  connectedThreshold->SetSeed(seed);
  connectedThreshold->SetInput(thresholdFilter->GetOutput());
  connectedThreshold->Update();

  //QImage lineImage(this->ImageToTrace.size());
  
}


bool operator<(const VectorRGB &P1, const VectorRGB &P2)
{
  if(P1[0] < P2[0] && P1[1] < P2[1] && P1[2] < P2[2])
    {
    return true;
    }
  return false;
}

bool operator>(const VectorRGB &P1, const VectorRGB &P2)
{
  if(P1[0] > P2[0] && P1[1] > P2[1] && P1[2] > P2[2])
    {
    return true;
    }
  return false;
}

bool operator<=(const VectorRGB &P1, const VectorRGB &P2)
{
  return !operator>(P1, P2);
}

bool operator>=(const VectorRGB &P1, const VectorRGB &P2)
{
  return !operator<(P1, P2);
}
