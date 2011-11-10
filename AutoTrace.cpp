#include "AutoTrace.h"

// ITK
#include "itkAndImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkImage.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

void Trace(const RGBImageType::Pointer image, const itk::CovariantVector<unsigned char, 3>& color, const itk::Index<2>& seed)
{
  // Create a color epsilon lower than the user supplied color
  int epsilon = 10;
  RGBImageType::PixelType lowerColor;
  RGBImageType::PixelType upperColor;
  for(unsigned int component = 0; component < 3; ++component)
    {
    lowerColor[component] = std::max(static_cast<int>(color[component]) - epsilon, 0); // Truncate negative values
    upperColor[component] = std::min(static_cast<int>(color[component]) + epsilon, 255); // Truncate values above 255
    }
  std::cout << "Lower color: " << lowerColor << std::endl;
  std::cout << "Upper color: " << upperColor << std::endl;
  
  // Threshold all 3 channels and AND the result
  std::vector<ScalarImageType::Pointer> thresholdedChannels(3);
  for(unsigned int channel = 0; channel < 3; ++channel)
  {
    typedef itk::VectorIndexSelectionCastImageFilter<RGBImageType, ScalarImageType> IndexSelectionType;
    IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetIndex(channel);
    indexSelectionFilter->SetInput(image);
    indexSelectionFilter->Update();

    // Use the color of the selection to threshold the image (looking for similar colors)
    typedef itk::BinaryThresholdImageFilter <ScalarImageType, ScalarImageType> BinaryThresholdImageFilterType;
    // Color pixels inside the threshold white. Color pixels outside the threshold black.
    BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
    thresholdFilter->SetInput(indexSelectionFilter->GetOutput());
    thresholdFilter->SetLowerThreshold(lowerColor[channel]);
    thresholdFilter->SetUpperThreshold(upperColor[channel]);
    thresholdFilter->SetInsideValue(200); // White
    thresholdFilter->SetOutsideValue(0); // Black
    thresholdFilter->Update();

    thresholdedChannels[channel] = ScalarImageType::New();
    DeepCopy<ScalarImageType>(thresholdFilter->GetOutput(), thresholdedChannels[channel]);
    }

  // Keep only region where all channels passed the threshold test
  typedef itk::AndImageFilter <ScalarImageType> AndImageFilterType;

  AndImageFilterType::Pointer andFilter1 = AndImageFilterType::New();
  andFilter1->SetInput(0, thresholdedChannels[0]);
  andFilter1->SetInput(1, thresholdedChannels[1]);
  andFilter1->Update();

  AndImageFilterType::Pointer andFilter2 = AndImageFilterType::New();
  andFilter2->SetInput(0, andFilter1->GetOutput());
  andFilter2->SetInput(1, thresholdedChannels[2]);
  andFilter2->Update();

  WriteImage<ScalarImageType>(andFilter2->GetOutput(), "Thresholded.png");
  
  // Use the location of the selection to get pixels that fit the color criterion and that are connected to the selected point.
  typedef itk::ConnectedThresholdImageFilter<ScalarImageType, ScalarImageType> ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(180);
  connectedThreshold->SetUpper(220);
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetSeed(seed);
  connectedThreshold->SetInput(andFilter2->GetOutput());
  connectedThreshold->Update();

  WriteImage<ScalarImageType>(connectedThreshold->GetOutput(), "connectedThreshold.png");
}
