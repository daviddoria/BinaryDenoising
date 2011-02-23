#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "Types.h"

namespace Debugging
{
template<typename T>
unsigned int CountNeighborsWithSameValue(typename T::Pointer image, itk::Index<2> pixel);

template<typename T>
unsigned int CountNeighborsWithDifferentValue(typename T::Pointer image, itk::Index<2> pixel);
}

#include "Debugging.txx"

#endif