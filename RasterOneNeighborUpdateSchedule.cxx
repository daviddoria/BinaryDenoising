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
