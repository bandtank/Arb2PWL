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

#ifndef MOORETRACING_HPP
#define MOORETRACING_HPP

#include "MooreTracing.h"

#include "itkImageRegionConstIterator.h"

template <typename TImage>
bool FindNeighborWithValue(const TImage* const image,
                           itk::Index<2> queryPixel, unsigned char queryValue,
                           itk::Offset<2>& neighbor)
{
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

template <typename TImage>
itk::Index<2> FindNextPixel(const TImage* const image, itk::Index<2> currentPixel, itk::Offset<2>& backtrack)
{
  itk::Offset<2> startingOffset = backtrack;

  std::vector< itk::Offset<2> > orderedOffsets = GetOrdered8NeighborOffsets(startingOffset);
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
  std::stringstream ss;
  ss << "Pixel " << currentPixel << " has no next pixel - this means there was a pixel that is not connected to anything!";
  throw std::runtime_error(ss.str());

  // This is just so there are no warnings about no return value, it should never be run. (Maybe we don't need it now that we throw?)
  itk::Index<2> zeroPixel;
  zeroPixel.Fill(0);
  return zeroPixel;
}

template <typename TImage>
itk::Index<2> FindFirstPixel(const TImage* const image)
{
  itk::ImageRegionConstIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());

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

template <typename TImage>
std::vector< itk::Index<2> > MooreTrace(const TImage* const image, const itk::Index<2>& startingPixel,
                                        const itk::Index<2>& endingPixel)
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

template <typename TImage>
unsigned int CountNonZeroPixels(const TImage* const image)
{
  itk::ImageRegionConstIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());

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

#endif
