#ifndef AutoTrace_H
#define AutoTrace_H

#include <string>

#include "itkCovariantVector.h"
#include "itkImage.h"

typedef itk::CovariantVector<unsigned char, 3> VectorRGB;
typedef itk::Image<VectorRGB, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;

void Trace(const RGBImageType::Pointer image, const itk::CovariantVector<unsigned char, 3>& color, const itk::Index<2>& seed);

template <typename TImage>
void WriteImage(const typename TImage::Pointer image, const std::string& fileName);

template<typename TImage>
void DeepCopy(typename TImage::Pointer input, typename TImage::Pointer output);

#include "AutoTrace.hxx"

#endif
