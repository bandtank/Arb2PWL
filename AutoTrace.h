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

#ifndef AutoTrace_H
#define AutoTrace_H

// STL
#include <string>

// ITK
#include "itkCovariantVector.h"
#include "itkImage.h"

/** Declare some types */
typedef itk::CovariantVector<unsigned char, 3> VectorRGB;
typedef itk::Image<VectorRGB, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;

/** This function takes an 'image' and traces a path (generates an ordered sequence of pixels) of pixels that are "connected" to 'seed'.*/
void Trace(const RGBImageType* const image, const itk::CovariantVector<unsigned char, 3>& color,
           const itk::Index<2>& seed);

#include "AutoTrace.hpp"

#endif
