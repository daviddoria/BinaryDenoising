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

#include "RandomUniqueUpdateSchedule.h"

RandomUniqueUpdateSchedule::RandomUniqueUpdateSchedule()
{
  this->CurrentNeighbor = 0;
  this->CurrentPass = 0;
}

void RandomUniqueUpdateSchedule::Initialize()
{
  this->RandomIterator = itk::ImageRandomNonRepeatingIteratorWithIndex<NodeImageType> (this->NodeImage,
                                                                                   this->NodeImage->GetLargestPossibleRegion());
  this->RandomIterator.SetNumberOfSamples(this->NodeImage->GetLargestPossibleRegion().GetNumberOfPixels());
  this->RandomIterator.GoToBegin();
}

unsigned int RandomUniqueUpdateSchedule::GetCurrentPass()
{
  return this->CurrentPass;
}

bool RandomUniqueUpdateSchedule::IsFinished()
{
  if(this->CurrentPass == this->NumberOfPasses)
    {
    return true;
    }
  return false;
}

MessageVector& RandomUniqueUpdateSchedule::NextMessage()
{
  while(!this->RandomIterator.IsAtEnd())
    {
    itk::Index<2> randomIndex = this->RandomIterator.GetIndex();
    if(this->CurrentNeighbor < this->RandomIterator.Get()->GetNumberOfNeighbors())
      {
      //MessageVector& messageVector = this->NodeImage->GetPixel(randomIndex).GetOutgoingMessageVector(this->CurrentNeighbor); // this works
      MessageVector& messageVector = this->RandomIterator.Value()->GetOutgoingMessageVector(this->CurrentNeighbor);

      this->CurrentNeighbor++;
      return messageVector;
      }
    ++(this->RandomIterator);
    this->CurrentNeighbor = 0;
    }
  this->CurrentPass++;
  this->RandomIterator.GoToBegin();
  return NextMessage();
}
