#ifndef Helpers_HPP
#define Helpers_HPP

#include "Helpers.h"

#include "itkImageRegionConstIteratorWithIndex.h"

template <typename TImage>
void WriteImage(const TImage* const image, const std::string& fileName)
{
  typedef  itk::ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(fileName);
  writer->SetInput(image);
  writer->Update();
}

template<typename TImage>
void DeepCopy(const TImage* const input, TImage* const output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}

template<typename TImage>
itk::Index<2> FindLeftMostNonZeroPixel(const TImage* const image)
{
  itk::ImageRegionConstIteratorWithIndex<TImage> imageIterator(image, image->GetLargestPossibleRegion());
  
  // Initialize
  itk::Index<2> leftMostPixel;
  leftMostPixel[0] = image->GetLargestPossibleRegion().GetSize()[0];
  leftMostPixel[1] = image->GetLargestPossibleRegion().GetSize()[1];
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get())
      {
      if(imageIterator.GetIndex()[0] < leftMostPixel[0])
	{
        leftMostPixel = imageIterator.GetIndex();
	}
      }
    ++imageIterator;
    }
    
  return leftMostPixel;
}

template<typename TImage>
itk::Index<2> FindRightMostNonZeroPixel(const TImage* const image)
{
  itk::ImageRegionConstIteratorWithIndex<TImage> imageIterator(image, image->GetLargestPossibleRegion());
  
  // Initialize
  itk::Index<2> rightMostPixel;
  rightMostPixel.Fill(0);
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get())
      {
      if(imageIterator.GetIndex()[0] > rightMostPixel[0])
	{
        rightMostPixel = imageIterator.GetIndex();
	}
      }
    ++imageIterator;
    }
    
  return rightMostPixel;  
}

#endif
