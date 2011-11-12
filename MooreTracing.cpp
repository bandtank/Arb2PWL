/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
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

#include "MooreTracing.h"

// ITK
#include "itkOffset.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkScalarToRGBColormapImageFilter.h"

// Custom
#include "Helpers.h"

std::vector< itk::Offset<2> > GetAllOffsets()
{
  // Set up the 8-neighbor indices in counter-clockwise order.
  // (In ITK, Offset<2> is indexed with (row, column) )
  std::vector< itk::Offset<2> > offsets;
  
  itk::Offset<2> offset;
  offset[0] = -1; offset[1] = -1;
  offsets.push_back(offset);
  offset[0] = -1; offset[1] = 0;
  offsets.push_back(offset);
  offset[0] = -1; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 0; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = 0;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = -1;
  offsets.push_back(offset);
  offset[0] = 0; offset[1] = -1;
  offsets.push_back(offset);

  return offsets;
}

std::vector< itk::Offset<2> > GetOrderedOffsets(itk::Offset<2> firstOffset)
{
  std::vector< itk::Offset<2> > orderedOffsets;
  
  std::vector< itk::Offset<2> > allOffsets = GetAllOffsets();
  
  // Find the starting point
  unsigned int startingOffset = 0;
  for(unsigned int i = 0; i < allOffsets.size(); ++i)
    {
    if(allOffsets[i] == firstOffset)
      {
      startingOffset = i;
      break;
      }
    }
    
  // Add the reamining offsets to the end of the list
  for(unsigned int i = startingOffset + 1; i < allOffsets.size(); ++i)
    {
    orderedOffsets.push_back(allOffsets[i]);
    }
    
  // Add the reamining offsets from the beginning of the list
  for(unsigned int i = 0; i < startingOffset; ++i)
    {
    orderedOffsets.push_back(allOffsets[i]);
    }
  
  return orderedOffsets;
}

bool FindNeighborWithValue(const UnsignedCharImageType::Pointer image, itk::Index<2> queryPixel, unsigned char queryValue, itk::Offset<2>& neighbor)
{
  // This function returns true if an 8-connected neighbor is found with the queryValue. If it is, the location of this pixel is returned by reference in 'neighbor'.
  // This function returns false otherwise.
  for(int i = -1; i <= 1; ++i)
    {
    for(int j = -1; j <= 1; ++j)
      {
      itk::Offset<2> offset;
      offset[0] = i;
      offset[1] = j;
      if(!image->GetLargestPossibleRegion().IsInside(queryPixel + offset))
	{
	continue;
	}
      if(image->GetPixel(queryPixel + offset) == queryValue)
	{
	neighbor = offset;
	return true;
	}
      }
    }
    
  return false;
}

std::vector< itk::Index<2> > MooreTrace(const UnsignedCharImageType::Pointer image, const itk::Index<2>& startingPixel, const itk::Index<2>& endingPixel)
{
  // Need to find a nonzero pixel around startingPixel. If there is not one, throw an error and quit.
  itk::Offset<2> backtrack;
  bool validStart = FindNeighborWithValue(image, startingPixel, 0, backtrack);
  if(!validStart)
    {
    std::cerr << "Cannot trace from " << startingPixel << " - it is not a boundary pixel!" << std::endl;
    exit(-1);
    }

  itk::Index<2> currentPixel = startingPixel;
  
  std::vector< itk::Index<2> > path;
  
  do
    {
    path.push_back(currentPixel);
    currentPixel = FindNextPixel(image, currentPixel, backtrack);
    //std::cout << numberOfPixelsTraced << " pixels have been traced." << std::endl;
    //std::cout << "Current pixel: " << currentPixel << " with backtrack: " << backtrack << std::endl;
    } while(currentPixel != endingPixel);
  
  return path;
}
/*
std::vector< itk::Index<2> > MooreTrace(UnsignedCharImageType::Pointer image)
{
  
  itk::Index<2> firstPixel = FindFirstPixel(image);
  return MooreTrace(image, firstPixel);
}*/

itk::Index<2> FindNextPixel(UnsignedCharImageType::Pointer image, itk::Index<2> currentPixel, itk::Offset<2>& backtrack)
{
  // The 'backtrack' input has two uses. First, it is used to know where to start the traversal. Second, it returns the next backtrack position by reference.
  
  itk::Offset<2> startingOffset = backtrack;
  
  std::vector< itk::Offset<2> > orderedOffsets = GetOrderedOffsets(startingOffset);
  for(unsigned int i = 0; i < orderedOffsets.size(); ++i)
    {
    if(image->GetPixel(currentPixel + orderedOffsets[i]))
      {
      if(i != 0)
	{
	backtrack = (currentPixel + orderedOffsets[i-1]) - (currentPixel + orderedOffsets[i]);
	}
      else
	{
	backtrack = (currentPixel + startingOffset) - (currentPixel + orderedOffsets[i]);
	}

      return currentPixel + orderedOffsets[i];
      }
    }
  std::cerr << "No next pixel - this means there was a pixel that is not connected to anything!" << std::endl;
  exit(-1);
  
  // This is just so there are no warnings about no return value, it should never be run
  itk::Index<2> zeroPixel;
  zeroPixel.Fill(0);
  return zeroPixel;
}

itk::Index<2> FindFirstPixel(const UnsignedCharImageType::Pointer image)
{
  // Perform a raster scan until a non-zero pixel is reached
  itk::ImageRegionConstIterator<UnsignedCharImageType> imageIterator(image, image->GetLargestPossibleRegion());

  itk::Index<2> firstPixelIndex;
  
  while(!imageIterator.IsAtEnd())
    {
    // Get the value of the current pixel
    if(imageIterator.Get())
      {
      firstPixelIndex = imageIterator.GetIndex();
      break;
      }

    ++imageIterator;
    }

  return firstPixelIndex;
}

unsigned int CountNonZeroPixels(const UnsignedCharImageType::Pointer image)
{
  itk::ImageRegionConstIterator<UnsignedCharImageType> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int numberOfNonZeroPixels = 0;
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get())
      {
      numberOfNonZeroPixels++;
      }

    ++imageIterator;
    }
  return numberOfNonZeroPixels;
}

void WriteColoredPath(const std::vector< itk::Index<2> >& pixelPath, const itk::ImageRegion<2>& region, const std::string& fileName)
{
  typedef itk::Image<int, 2> IntImageType;
  IntImageType::Pointer coloredPathImage = IntImageType::New();
  coloredPathImage->SetRegions(region);
  coloredPathImage->Allocate();
  coloredPathImage->FillBuffer(0);
  
  for(unsigned int i = 0; i < pixelPath.size(); ++i)
    {
    coloredPathImage->SetPixel(pixelPath[i], i);
    }
    
  WriteImage<IntImageType>(coloredPathImage, "IntPath.mha");
//   typedef itk::RescaleIntensityImageFilter< UnsignedCharImageType, UnsignedCharImageType > RescaleFilterType;
//   RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
//   rescaleFilter->SetInput(output);
//   rescaleFilter->SetOutputMinimum(0);
//   rescaleFilter->SetOutputMaximum(255);
//   rescaleFilter->Update();
  
  typedef itk::RGBPixel<unsigned char>    RGBPixelType;
  typedef itk::Image<RGBPixelType, 2>  RGBImageType;
  typedef itk::ScalarToRGBColormapImageFilter<IntImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(coloredPathImage);
  rgbfilter->SetColormap( RGBFilterType::Hot );
  rgbfilter->Update();
 
  typedef  itk::ImageFileWriter< RGBImageType > OutputWriterType;
  OutputWriterType::Pointer outputWriter = OutputWriterType::New();
  outputWriter->SetFileName(fileName);
  outputWriter->SetInput(rgbfilter->GetOutput());
  outputWriter->Update();
}
