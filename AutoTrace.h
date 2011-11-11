#ifndef AutoTrace_H
#define AutoTrace_H

#include <string>

#include "itkCovariantVector.h"
#include "itkImage.h"

typedef itk::CovariantVector<unsigned char, 3> VectorRGB;
typedef itk::Image<VectorRGB, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;

void Trace(const RGBImageType::Pointer image, const itk::CovariantVector<unsigned char, 3>& color, const itk::Index<2>& seed);

#include "AutoTrace.hxx"

#endif
