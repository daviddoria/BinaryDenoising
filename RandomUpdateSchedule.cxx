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

#include "RandomUpdateSchedule.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

RandomUpdateSchedule::RandomUpdateSchedule()
{
}

bool RandomUpdateSchedule::IsFinished()
{
  return false;
}

void RandomUpdateSchedule::Initialize()
{
  this->RandomIterator = itk::ImageRandomConstIteratorWithIndex<NodeImageType> (this->NodeImage,
                                                                                   this->NodeImage->GetLargestPossibleRegion());
  this->RandomIterator.SetNumberOfSamples(1);
}

MessageVector& RandomUpdateSchedule::NextMessage()
{
  this->RandomIterator.GoToBegin();
  itk::Index<2> randomIndex = this->RandomIterator.GetIndex();

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer generator = GeneratorType::New();
  generator->Initialize();
  unsigned int randomMessageVector = generator->GetIntegerVariate(this->NodeImage->GetPixel(randomIndex).GetNumberOfNeighbors() -1);

  MessageVector& messageVector = this->NodeImage->GetPixel(randomIndex).GetOutgoingMessageVector(randomMessageVector);
  return messageVector;
}
