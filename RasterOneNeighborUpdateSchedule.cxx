#include "RasterOneNeighborUpdateSchedule.h"

RasterOneNeighborUpdateSchedule::RasterOneNeighborUpdateSchedule()
{
  this->currentMessageVectorId = 0;
}

void RasterOneNeighborUpdateSchedule::Initialize()
{
  this->imageIterator = itk::ImageRegionConstIterator<MessageImageType> (this->OutgoingMessageImage,
                                                                         this->OutgoingMessageImage->GetLargestPossibleRegion());
}

MessageVector& RasterOneNeighborUpdateSchedule::NextMessage()
{
  while(!this->imageIterator.IsAtEnd())
    {
    itk::Index<2> currentIndex = imageIterator.GetIndex();
    MessageVector& messageVector = this->OutgoingMessageImage->GetPixel(currentIndex)[currentMessageVectorId];

    if(this->currentMessageVectorId >= this->OutgoingMessageImage->GetPixel(currentIndex).size())
      {
      ++imageIterator;
      continue;
      }
    //std::cout << "Processing message " << imageIterator.GetIndex() << " [" << currentMessageVectorId << "]" << std::endl;
    ++imageIterator;
    return messageVector;
    }

  // When we get to here, we have traversed the whole image. Start over:
  //std::cout << "Restart raster scan." << std::endl;
  this->currentMessageVectorId++;
  unsigned int maxNeighbors = 4;
  if(this->currentMessageVectorId == maxNeighbors)
    {
    this->currentMessageVectorId = 0;
    }

  imageIterator.GoToBegin();
  return NextMessage();
}
