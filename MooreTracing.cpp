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

#include "MooreTracing.h"

// ITK
#include "itkOffset.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkScalarToRGBColormapImageFilter.h"

// Custom
#include "Helpers.h"

std::vector< itk::Offset<2> > Get8NeighborOffsets()
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

std::vector< itk::Offset<2> > GetOrdered8NeighborOffsets(itk::Offset<2> firstOffset)
{
  std::vector< itk::Offset<2> > orderedOffsets;
  
  std::vector< itk::Offset<2> > allOffsets = Get8NeighborOffsets();
  
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

void WriteColoredPath(const std::vector< itk::Index<2> >& pixelPath,
                      const itk::ImageRegion<2>& region, const std::string& fileName)
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
    
  WriteImage(coloredPathImage.GetPointer(), "IntPath.mha");
//   typedef itk::RescaleIntensityImageFilter< UnsignedCharImageType, UnsignedCharImageType > RescaleFilterType;
//   RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
//   rescaleFilter->SetInput(output);
//   rescaleFilter->SetOutputMinimum(0);
//   rescaleFilter->SetOutputMaximum(255);
//   rescaleFilter->Update();
  
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
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
