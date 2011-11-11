#ifndef HELPERS_H
#define HELPERS_H

template <typename TImage>
void WriteImage(const typename TImage::Pointer image, const std::string& fileName);

template<typename TImage>
void DeepCopy(typename TImage::Pointer input, typename TImage::Pointer output);

template<typename TImage>
itk::Index<2> FindLeftMostNonZeroPixel(const typename TImage::Pointer input);

template<typename TImage>
itk::Index<2> FindRightMostNonZeroPixel(const typename TImage::Pointer input);

#include "Helpers.hxx"

#endif
