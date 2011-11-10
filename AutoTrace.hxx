#include "itkImageFileWriter.h"

template <typename TImage>
void WriteImage(const typename TImage::Pointer image, const std::string& fileName)
{
  typedef  itk::ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(fileName);
  writer->SetInput(image);
  writer->Update();
}

template<typename TImage>
void DeepCopy(typename TImage::Pointer input, typename TImage::Pointer output)
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
