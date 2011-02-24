#include "RasterAllNeighborsUpdateSchedule.h"

RasterAllNeighborsUpdateSchedule::RasterAllNeighborsUpdateSchedule()
{
  this->currentMessageVectorId = 0;
}

void RasterAllNeighborsUpdateSchedule::Initialize()
{
  this->imageIterator = itk::ImageRegionConstIterator<MessageImageType> (this->OutgoingMessageImage,
                                                                         this->OutgoingMessageImage->GetLargestPossibleRegion());
}

MessageVector& RasterAllNeighborsUpdateSchedule::NextMessage()
{
  while(!this->imageIterator.IsAtEnd())
    {
    itk::Index<2> currentIndex = imageIterator.GetIndex();
    MessageVector& messageVector = this->OutgoingMessageImage->GetPixel(currentIndex)[currentMessageVectorId];

    currentMessageVectorId++;
    if(currentMessageVectorId >= this->OutgoingMessageImage->GetPixel(currentIndex).size())
      {
      currentMessageVectorId = 0;
      ++imageIterator;
      }

    return messageVector;
    }

  // When we get to here, we have traversed the whole image. Start over:
  //std::cout << "Restart raster scan." << std::endl;
  imageIterator.GoToBegin();
  return NextMessage();
}
