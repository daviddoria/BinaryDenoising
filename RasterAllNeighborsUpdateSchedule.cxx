#include "RasterAllNeighborsUpdateSchedule.h"

RasterAllNeighborsUpdateSchedule::RasterAllNeighborsUpdateSchedule()
{
  this->CurrentMessageVectorId = 0;
}

void RasterAllNeighborsUpdateSchedule::Initialize()
{
  this->ImageIterator = itk::ImageRegionConstIterator<NodeImageType> (this->NodeImage,
                                                                         this->NodeImage->GetLargestPossibleRegion());
}

MessageVector& RasterAllNeighborsUpdateSchedule::NextMessage()
{
  while(!this->ImageIterator.IsAtEnd())
    {
    itk::Index<2> currentIndex = this->ImageIterator.GetIndex();
    MessageVector& messageVector = this->NodeImage->GetPixel(currentIndex).GetOutgoingMessageVector(this->CurrentMessageVectorId);

    this->CurrentMessageVectorId++;
    if(this->CurrentMessageVectorId >= this->NodeImage->GetPixel(currentIndex).GetNumberOfNeighbors())
      {
      this->CurrentMessageVectorId = 0;
      ++(this->ImageIterator);
      }

    return messageVector;
    }

  // When we get to here, we have traversed the whole image. Start over:
  //std::cout << "Restart raster scan." << std::endl;
  this->ImageIterator.GoToBegin();
  return NextMessage();
}
