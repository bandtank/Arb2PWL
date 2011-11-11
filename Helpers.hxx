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

template<typename TImage>
itk::Index<2> FindLeftMostNonZeroPixel(const typename TImage::Pointer input)
{
  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  
  itk::Index<2> leftMostPixel;
  leftMostPixel[0] = input->GetLargestPossibleRegion().GetSize()[0];
  leftMostPixel[1] = input->GetLargestPossibleRegion().GetSize()[1];
  while(!inputIterator.IsAtEnd())
    {
    if(inputIterator.Get())
      {
      if(inputIterator.GetIndex()[0] < leftMostPixel[0])
	{
	leftMostPixel = inputIterator.GetIndex();
	}
      }
    ++inputIterator;
    }
    
  return leftMostPixel;
}

template<typename TImage>
itk::Index<2> FindRightMostNonZeroPixel(const typename TImage::Pointer input)
{
  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  
  itk::Index<2> rightMostPixel;
  rightMostPixel.Fill(0);
  while(!inputIterator.IsAtEnd())
    {
    if(inputIterator.Get())
      {
      if(inputIterator.GetIndex()[0] > rightMostPixel[0])
	{
	rightMostPixel = inputIterator.GetIndex();
	}
      }
    ++inputIterator;
    }
    
  return rightMostPixel;  
}
