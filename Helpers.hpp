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
