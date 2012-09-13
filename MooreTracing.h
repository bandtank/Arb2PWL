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

#ifndef MOORETRACING_H
#define MOORETRACING_H

// ITK
#include "itkImage.h"

// STL
#include <vector>

/** Perform a raster scan until a non-zero pixel is reached. */
template <typename TImage>
itk::Index<2> FindFirstPixel(const TImage* const image);

/** The 'backtrack' input has two uses. First, it is used to know where to start the traversal.
  * Second, it returns the next backtrack position by reference. */
template <typename TImage>
itk::Index<2> FindNextPixel(const TImage* const image,
                            itk::Index<2> currentPixel, itk::Offset<2>& backtrack);

/** This function returns true if an 8-connected neighbor is found with the queryValue.
  * If it is, the location of this pixel is returned by reference in 'neighbor'.
  * Returns false otherwise. */
template <typename TImage>
bool FindNeighborWithValue(const TImage* const image,
                           itk::Index<2> queryPixel, unsigned char queryValue,
                           itk::Offset<2>& neighbor);

/** Trace the contour of a region.
  * This function expects a black image with a single contour to trace. */
template <typename TImage>
std::vector< itk::Index<2> > MooreTrace(const TImage* const image,
                                        const itk::Index<2>& startingPixel, const itk::Index<2>& endingPixel);

/** Count how many non-zero pixels are in 'image'. */
template <typename TImage>
unsigned int CountNonZeroPixels(const TImage* const image);

// Non-template functions
void WriteColoredPath(const std::vector< itk::Index<2> >& pixelPath,
                      const itk::ImageRegion<2>& region, const std::string& fileName);

/** Get a collection of the 8-neighbor offsets (of any pixel, since they are relative). */
std::vector<itk::Offset<2> > Get8NeighborOffsets();

/** Get a collection of the 8-neighbor offsets (of any pixel, since they are relative),
  * starting at 'firstOffset'. */
std::vector<itk::Offset<2> > GetOrdered8NeighborOffsets(itk::Offset<2> firstOffset);

#include "MooreTracing.hpp"

#endif
