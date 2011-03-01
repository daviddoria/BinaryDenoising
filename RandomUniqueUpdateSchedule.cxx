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
