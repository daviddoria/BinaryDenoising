#include "RandomUniqueUpdateSchedule.h"

RandomUniqueUpdateSchedule::RandomUniqueUpdateSchedule()
{
  this->CurrentNeighbor = 0;
}

void RandomUniqueUpdateSchedule::Initialize()
{
  this->RandomIterator = itk::ImageRandomNonRepeatingConstIteratorWithIndex<NodeImageType> (this->NodeImage,
                                                                                   this->NodeImage->GetLargestPossibleRegion());
  std::cout << "Setting " << this->NodeImage->GetLargestPossibleRegion().GetNumberOfPixels() << " samples." << std::endl;
  this->RandomIterator.SetNumberOfSamples(this->NodeImage->GetLargestPossibleRegion().GetNumberOfPixels());
  this->RandomIterator.GoToBegin();
}

MessageVector& RandomUniqueUpdateSchedule::NextMessage()
{
  while(!this->RandomIterator.IsAtEnd())
    {
    itk::Index<2> randomIndex = this->RandomIterator.GetIndex();
    if(this->CurrentNeighbor < this->RandomIterator.Get().GetNumberOfNeighbors())
      {
      //MessageVector& messageVector = this->NodeImage->GetPixel(randomIndex).GetOutgoingMessageVector(this->CurrentNeighbor); // this works
      // this->randomIterator->Get()[this->CurrentNeighbor] should do the same thing
      MessageVector& messageVector = this->RandomIterator.Get().GetOutgoingMessageVector(this->CurrentNeighbor); // this causes a crash

      //std::cout << "Get: " << this->randomIterator.Get()[this->CurrentNeighbor] << std::endl;
      //std::cout << "GetPixel: " << this->OutgoingMessageImage->GetPixel(randomIndex)[this->CurrentNeighbor] << std::endl;

      //std::cout << "Get: " << this->RandomIterator.Get() << std::endl;
      //std::cout << "GetPixel: " << this->NodeImage->GetPixel(randomIndex) << std::endl;

      this->CurrentNeighbor++;
      return messageVector;
      }
    ++(this->RandomIterator);
    this->CurrentNeighbor = 0;
    }
  std::cout << "Visited all pixels!" << std::endl;
  this->RandomIterator.GoToBegin();
  return NextMessage();
}
