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

#ifndef HELPERS_H
#define HELPERS_H

/** Write an 'image' to a file. */
template <typename TImage>
void WriteImage(const TImage* const image, const std::string& fileName);

/** Copy 'input' to 'output'. */
template<typename TImage>
void DeepCopy(const TImage* const input, TImage* const output);

/** Find the left most non-zero pixel in 'image'. */
template<typename TImage>
itk::Index<2> FindLeftMostNonZeroPixel(const TImage* const image);

/** Find the right most non-zero pixel in 'image'. */
template<typename TImage>
itk::Index<2> FindRightMostNonZeroPixel(const TImage* const image);

#include "Helpers.hpp"

#endif
