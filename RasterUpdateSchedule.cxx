#include "RasterUpdateSchedule.h"

RasterUpdateSchedule::RasterUpdateSchedule()
{
  this->currentMessageVectorId = 0;
}

void RasterUpdateSchedule::Initialize()
{
  this->imageIterator = itk::ImageRegionConstIterator<MessageImageType> (this->OutgoingMessageImage,
                                                                         this->OutgoingMessageImage->GetLargestPossibleRegion());
}

MessageVector& RasterUpdateSchedule::NextMessage()
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
