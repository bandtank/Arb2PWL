#include "AutoTrace.h"

#include "itkImageFileReader.h"

int main(int argc, char *argv[])
{
  RGBImageType::PixelType color;
  color[0] = 122;
  color[1] = 122;
  color[2] = 122;

  itk::Index<2> seed;
  seed[0] = 74;
  seed[1] = 104;

  typedef itk::ImageFileReader<RGBImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("data/line.png");
  reader->Update();
  
  Trace(reader->GetOutput(), color, seed);

  return 0;
}
