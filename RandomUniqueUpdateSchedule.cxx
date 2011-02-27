#include "RandomUniqueUpdateSchedule.h"

RandomUniqueUpdateSchedule::RandomUniqueUpdateSchedule()
{
  this->CurrentNeighbor = 0;
}

void RandomUniqueUpdateSchedule::Initialize()
{
  this->randomIterator = itk::ImageRandomNonRepeatingConstIteratorWithIndex<MessageImageType> (this->OutgoingMessageImage,
                                                                                   this->OutgoingMessageImage->GetLargestPossibleRegion());
  std::cout << "Setting " << this->OutgoingMessageImage->GetLargestPossibleRegion().GetNumberOfPixels() << " samples." << std::endl;
  this->randomIterator.SetNumberOfSamples(this->OutgoingMessageImage->GetLargestPossibleRegion().GetNumberOfPixels());
  this->randomIterator.GoToBegin();
}

MessageVector& RandomUniqueUpdateSchedule::NextMessage()
{
  while(!this->randomIterator.IsAtEnd())
    {
    itk::Index<2> randomIndex = this->randomIterator.GetIndex();
    if(this->CurrentNeighbor < this->randomIterator.Get().size())
      {
      MessageVector& messageVector = this->OutgoingMessageImage->GetPixel(randomIndex)[this->CurrentNeighbor]; // this works
      // this->randomIterator->Get()[this->CurrentNeighbor] should do the same thing
      //MessageVector& messageVector = this->randomIterator.Get()[this->CurrentNeighbor];

      //std::cout << "Get: " << this->randomIterator.Get()[this->CurrentNeighbor] << std::endl;
      //std::cout << "GetPixel: " << this->OutgoingMessageImage->GetPixel(randomIndex)[this->CurrentNeighbor] << std::endl;

      std::cout << "Get: " << this->randomIterator.Get() << std::endl;
      std::cout << "GetPixel: " << this->OutgoingMessageImage->GetPixel(randomIndex) << std::endl;

      this->CurrentNeighbor++;
      return messageVector;
      }
    ++this->randomIterator;
    this->CurrentNeighbor = 0;
    }
  std::cout << "Visited all pixels!" << std::endl;
  this->randomIterator.GoToBegin();
  return NextMessage();
}
