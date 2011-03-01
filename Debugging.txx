/*
Copyright (C) 2011 David Doria, daviddoria@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Helpers.h"

namespace Debugging
{


template <typename T>
void LoopyBP<T>::OutputBeliefImage()
{
  itk::Size<2> imageSize = this->OutgoingMessageImage->GetLargestPossibleRegion().GetSize();

  for(unsigned int i = 0; i < imageSize[0]; i++)
    {
    for(unsigned int j = 0; j < imageSize[1]; j++)
      {
      std::vector<float> beliefs(this->LabelSet.size());
      itk::Index<2> index;
      index[0] = i;
      index[1] = j;
      for(unsigned int l = 0; l < this->LabelSet.size(); l++)
        {
        float belief = Belief(index, l);
        beliefs[l] = belief;
        }
      Helpers::NormalizeVector(beliefs);
      Helpers::OutputVector<float>(beliefs);
      }// end for j
      std::cout << std::endl;
    }// end for i
  std::cout << std::endl;
}


template <typename T>
void LoopyBP<T>::OutputMessageImage()
{

  itk::Size<2> imageSize = this->OutgoingMessageImage->GetLargestPossibleRegion().GetSize();

  for(unsigned int i = 0; i < imageSize[0]; i++)
    {
    for(unsigned int j = 0; j < imageSize[1]; j++)
      {
      itk::Index<2> index;
      index[0] = i;
      index[1] = j;
      std::vector<MessageVector> messageVectors = this->OutgoingMessageImage->GetPixel(index);
      for(unsigned int neighbor = 0; neighbor < messageVectors.size(); neighbor++)
        {
        std::vector<float> values = messageVectors[neighbor].GetAllMessageValues();
        Helpers::OutputVector<float>(values);
        }
      std::cout << " ";
      }// end for j
      std::cout << std::endl;
    }// end for i
  std::cout << std::endl;
}


template <typename T>
void LoopyBP<T>::WriteBeliefImage(std::string filename)
{
  FloatImageType::Pointer beliefImage = FloatImageType::New();
  beliefImage->SetRegions(this->NodeImage->GetLargestPossibleRegion());
  beliefImage->Allocate();

  itk::Size<2> imageSize = beliefImage->GetLargestPossibleRegion().GetSize();

  for(unsigned int i = 0; i < imageSize[0]; i++)
    {
    for(unsigned int j = 0; j < imageSize[1]; j++)
      {
      std::vector<float> beliefs(this->LabelSet.size());
      itk::Index<2> index;
      index[0] = i;
      index[1] = j;
      for(unsigned int l = 0; l < this->LabelSet.size(); l++)
        {
        float belief = Belief(index, l);
        beliefs[l] = belief;
        }
      Helpers::NormalizeVector(beliefs);
      beliefImage->SetPixel(index, beliefs[1]); // the probability of "white pixel"
      }// end for j
    }// end for i

  typedef itk::IntensityWindowingImageFilter <FloatImageType, IntImageType> IntensityWindowingImageFilterType;

  IntensityWindowingImageFilterType::Pointer filter = IntensityWindowingImageFilterType::New();
  filter->SetInput(beliefImage);
  filter->SetWindowMinimum(0);
  filter->SetWindowMaximum(1);
  filter->SetOutputMinimum(0);
  filter->SetOutputMaximum(255);
  filter->Update();

  Helpers::WriteScaledImage<IntImageType>(filter->GetOutput(), filename);
}

template <typename T>
void LoopyBP<T>::OutputMessagesBetweenNodes(const itk::Index<2> fromNode, const itk::Index<2> toNode)
{
  MessageVector& messageVector = GetMessages(fromNode, toNode);
  std::cout << "Messages from " << fromNode << " to " << toNode << std::endl << messageVector << std::endl;
}

template<typename T>
unsigned int CountNeighborsWithSameValue(typename T::Pointer image, itk::Index<2> pixel)
{
  std::vector<itk::Index<2> > neighbors = Helpers::Get4Neighbors<T>(image, pixel);

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
  std::vector<itk::Index<2> > neighbors = Helpers::Get4Neighbors<T>(image, pixel);

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