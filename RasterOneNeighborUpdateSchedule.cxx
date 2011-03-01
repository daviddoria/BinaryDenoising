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

#include "RasterOneNeighborUpdateSchedule.h"

RasterOneNeighborUpdateSchedule::RasterOneNeighborUpdateSchedule()
{
  this->CurrentMessageVectorId = 0;
}

void RasterOneNeighborUpdateSchedule::Initialize()
{
  this->ImageIterator = itk::ImageRegionConstIterator<NodeImageType> (this->NodeImage,
                                                                         this->NodeImage->GetLargestPossibleRegion());
}

bool RasterOneNeighborUpdateSchedule::IsFinished()
{
  return false;
}

MessageVector& RasterOneNeighborUpdateSchedule::NextMessage()
{
  while(!this->ImageIterator.IsAtEnd())
    {
    itk::Index<2> currentIndex = this->ImageIterator.GetIndex();
    MessageVector& messageVector = this->NodeImage->GetPixel(currentIndex).GetOutgoingMessageVector(this->CurrentMessageVectorId);

    if(this->CurrentMessageVectorId >= this->NodeImage->GetPixel(currentIndex).GetNumberOfNeighbors())
      {
      ++(this->ImageIterator);
      continue;
      }
    //std::cout << "Processing message " << imageIterator.GetIndex() << " [" << currentMessageVectorId << "]" << std::endl;
    ++(this->ImageIterator);
    return messageVector;
    }

  // When we get to here, we have traversed the whole image. Start over:
  //std::cout << "Restart raster scan." << std::endl;
  this->CurrentMessageVectorId++;
  unsigned int maxNeighbors = 4;
  if(this->CurrentMessageVectorId == maxNeighbors)
    {
    this->CurrentMessageVectorId = 0;
    }

  this->ImageIterator.GoToBegin();
  return NextMessage();
}
