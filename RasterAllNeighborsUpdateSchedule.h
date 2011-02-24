#ifndef RASTERALLNEIGHBORSUPDATESCHEDULE_H
#define RASTERALLNEIGHBORSUPDATESCHEDULE_H

/*
 * This update schedule updates the messages going to all neighbors of a node, traversing the nodes in a raster fasion.
 * Only one raster scan throught the image is necessary to update all messages.
 */

#include "UpdateSchedule.h"

#include "itkImageRegionConstIterator.h"

class RasterAllNeighborsUpdateSchedule : public UpdateSchedule
{
public:
  RasterAllNeighborsUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();

private:
  itk::ImageRegionConstIterator<MessageImageType> imageIterator;
  unsigned int currentMessageVectorId;
};

#endif