#ifndef AutoTrace_H
#define AutoTrace_H

#include <string>

#include "itkCovariantVector.h"
#include "itkImage.h"

/** Declare some types */
typedef itk::CovariantVector<unsigned char, 3> VectorRGB;
typedef itk::Image<VectorRGB, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;

/** This function takes an 'image' and traces a path (generates an ordered sequence of pixels) of pixels that are "connected" to 'seed'.*/
void Trace(const RGBImageType* const image, const itk::CovariantVector<unsigned char, 3>& color, const itk::Index<2>& seed);

#include "AutoTrace.hxx"

#endif
