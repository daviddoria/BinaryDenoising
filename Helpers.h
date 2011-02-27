#ifndef HELPERS_H
#define HELPERS_H

#include "Types.h"

namespace Helpers
{

void AddNoise(IntImageType::Pointer image, float percentToFlip);

template<typename T>
void DeepCopy(typename T::Pointer inputImage, typename T::Pointer outputImage);

template<typename T>
void WriteScaledImage(typename T::Pointer image, std::string filename);

template<typename T>
void WriteCastedImage(typename T::Pointer image, std::string filename);


template<typename T>
void WriteBinaryImage(typename T::Pointer image, std::string filename);

template<typename T>
void ConvertToBinaryImage(typename T::Pointer image, IntImageType::Pointer binary);

template<typename T>
std::vector<itk::Index<2> > Get8Neighbors(typename T::Pointer image, itk::Index<2> pixel);

template<typename T>
std::vector<itk::Index<2> > Get4Neighbors(typename T::Pointer image, itk::Index<2> pixel);

template<typename T>
void OutputVector(std::vector<T> vec);

void NormalizeVector(std::vector<float> &vec);

}

#include "Helpers.txx"

#endif
