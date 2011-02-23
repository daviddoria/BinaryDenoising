#include "Helpers.h"

namespace Debugging
{

template<typename T>
unsigned int CountNeighborsWithSameValue(typename T::Pointer image, itk::Index<2> pixel)
{
  std::vector<itk::Index<2> > neighbors = Helpers::GetNeighbors<T>(image, pixel);

  unsigned int count = 0;

  for(unsigned int i = 0; i < neighbors.size(); i++)
    {
    if(image->GetPixel(pixel) == image->GetPixel(neighbors[i]))
      {
      count++;
      }
    }
  return count;
}

template<typename T>
unsigned int CountNeighborsWithDifferentValue(typename T::Pointer image, itk::Index<2> pixel)
{
  std::vector<itk::Index<2> > neighbors = Helpers::GetNeighbors<T>(image, pixel);

  unsigned int count = 0;

  for(unsigned int i = 0; i < neighbors.size(); i++)
    {
    if(image->GetPixel(pixel) != image->GetPixel(neighbors[i]))
      {
      count++;
      }
    }
  return count;
}

} // end namespace