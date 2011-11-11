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

#ifndef MOORETRACING_H
#define MOORETRACING_H

// ITK
#include "itkImage.h"

// STL
#include <vector>

typedef itk::Image<unsigned char, 2> UnsignedCharImageType;

std::vector< itk::Offset<2> > GetAllOffsets();

std::vector< itk::Offset<2> > GetOrderedOffsets(itk::Offset<2> firstOffset);

itk::Index<2> FindFirstPixel(const UnsignedCharImageType::Pointer image);

itk::Index<2> FindNextPixel(const UnsignedCharImageType::Pointer image, itk::Index<2> currentPixel, itk::Offset<2>& backtrack);

bool FindNeighborWithValue(const UnsignedCharImageType::Pointer image, itk::Index<2> queryPixel, unsigned char queryValue, itk::Offset<2>& neighbor);

// This function expects a black image with a single contour to trace
// std::vector< itk::Index<2> > MooreTrace(const UnsignedCharImageType::Pointer image);

std::vector< itk::Index<2> > MooreTrace(const UnsignedCharImageType::Pointer image, const itk::Index<2>& startingPixel, const itk::Index<2>& endingPixel);

unsigned int CountNonZeroPixels(const UnsignedCharImageType::Pointer image);

void WriteColoredPath(const std::vector< itk::Index<2> >& pixelPath, const itk::ImageRegion<2>& region, const std::string& fileName);

#endif
