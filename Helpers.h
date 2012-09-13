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
