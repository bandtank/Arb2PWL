#include "AutoTrace.h"

// Custom
#include "MooreTracing.h"
#include "Helpers.h"

// ITK
#include "itkAndImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryThinningImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

void Trace(const RGBImageType* const image, const itk::CovariantVector<unsigned char, 3>& color, const itk::Index<2>& seed)
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
  std::cout << "Lower color: ";
  for(unsigned int i = 0; i < 3; ++i)
    {
    std::cout << static_cast<int>(lowerColor[i]) << " ";
    }
  std::cout << std::endl;
  
  std::cout << "Upper color: ";
  for(unsigned int i = 0; i < 3; ++i)
    {
    std::cout << static_cast<int>(upperColor[i]) << " ";
    }
  std::cout << std::endl;
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
    thresholdFilter->SetInsideValue(255); // White
    thresholdFilter->SetOutsideValue(0); // Black
    thresholdFilter->Update();

    thresholdedChannels[channel] = ScalarImageType::New();
    DeepCopy(thresholdFilter->GetOutput(), thresholdedChannels[channel].GetPointer());
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

  ScalarImageType::Pointer thresholdedImage = ScalarImageType::New();
  DeepCopy(andFilter2->GetOutput(), thresholdedImage.GetPointer());
  WriteImage(thresholdedImage.GetPointer(), "Thresholded.png");
  
  // Dilate the image.
  typedef itk::BinaryBallStructuringElement<ScalarImageType::PixelType, 2> StructuringElementType;
  StructuringElementType structuringElement;
  unsigned int radius = 5; // How big of holes we want to connect.
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();
 
  typedef itk::BinaryDilateImageFilter <ScalarImageType, ScalarImageType, StructuringElementType> BinaryDilateImageFilterType;
  BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
  dilateFilter->SetInput(thresholdedImage);
  dilateFilter->SetKernel(structuringElement);
  dilateFilter->SetDilateValue(255);
  dilateFilter->Update();
  
  ScalarImageType::Pointer dilatedImage = ScalarImageType::New();
  DeepCopy(dilateFilter->GetOutput(), dilatedImage.GetPointer());
  WriteImage(dilatedImage.GetPointer(), "Dilated.png");
  
  // Thin/skeletonize the image
  typedef itk::BinaryThinningImageFilter <ScalarImageType, ScalarImageType> BinaryThinningImageFilterType;
  BinaryThinningImageFilterType::Pointer binaryThinningImageFilter = BinaryThinningImageFilterType::New();
  binaryThinningImageFilter->SetInput(dilatedImage);
  binaryThinningImageFilter->Update();

  // Rescale the output of the thinning filter so that it can be seen (the output is 0 and 1, we want 0 and 255)
  typedef itk::RescaleIntensityImageFilter< ScalarImageType, ScalarImageType > RescaleType;
  RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput( binaryThinningImageFilter->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->Update();
  
  WriteImage(rescaler->GetOutput(), "Thinned.png");
  
  // Get pixels that are connected to the selected point.
  typedef itk::ConnectedThresholdImageFilter<ScalarImageType, ScalarImageType> ConnectedFilterType;
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->SetLower(255);
  connectedThreshold->SetUpper(255);
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetSeed(seed);
  connectedThreshold->SetInput(rescaler->GetOutput());
  connectedThreshold->SetConnectivity(ConnectedFilterType::FullConnectivity); // consider 8 neighbors when deciding connectivity
  connectedThreshold->Update();

  WriteImage(connectedThreshold->GetOutput(), "connectedThreshold.png");

  // Find the extreme pixel
  itk::Index<2> leftPixel = FindLeftMostNonZeroPixel<ScalarImageType>(connectedThreshold->GetOutput());
  itk::Index<2> rightPixel = FindRightMostNonZeroPixel<ScalarImageType>(connectedThreshold->GetOutput());
  std::vector< itk::Index<2> > pixelPath = MooreTrace(connectedThreshold->GetOutput(), leftPixel, rightPixel);
  
  WriteColoredPath(pixelPath, image->GetLargestPossibleRegion(), "ColoredPath.png");
}
