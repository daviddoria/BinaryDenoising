#ifndef HELPERS_H
#define HELPERS_H

#include "Types.h"

namespace Helpers
{

void AddNoise(IntImageType::Pointer image, float percentToFlip);

template<typename T>
void DeepCopy(typename T::Pointer inputImage, typename T::Pointer outputImage);

template<typename T>
void WriteImage(typename T::Pointer image, std::string filename);

template<typename T>
std::vector<itk::Index<2> > GetNeighbors(typename T::Pointer image, itk::Index<2> pixel);

}

#include "Helpers.txx"

#endif
