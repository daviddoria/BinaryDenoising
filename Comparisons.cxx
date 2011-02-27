#include "Comparisons.h"

bool operator<(const itk::Index<2> &index1, const itk::Index<2> &index2)
{
  if((index1[0] < index2[0]))
  {
    return true;
  }

  if(index1[0] == index2[0])
    {
    if(index1[1] < index2[1])
      {
      return true;
      }
    }

  return false;
}

bool IndexComparison::operator()(const itk::Index<2>& index1, const itk::Index<2>& index2) const
{
  return index1 < index2;
}

bool MessageVectorComparison::operator()(const MessageVector& mv1, const MessageVector& mv2) const
{
  if((mv1.FromNode < mv2.FromNode))
  {
    return true;
  }

  if(mv1.FromNode == mv2.FromNode)
    {
    if(mv1.ToNode < mv2.ToNode)
      {
      return true;
      }
    }

  return false;
}
