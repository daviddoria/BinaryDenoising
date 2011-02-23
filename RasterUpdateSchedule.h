#ifndef RASTERUPDATESCHEDULE_H
#define RASTERUPDATESCHEDULE_H

#include "UpdateSchedule.h"

#include "itkImageRegionConstIterator.h"

class RasterUpdateSchedule : public UpdateSchedule
{
public:
  RasterUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();

private:
  itk::ImageRegionConstIterator<MessageImageType> imageIterator;
  unsigned int currentMessageVectorId;
};

#endif