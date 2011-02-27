#ifndef RASTERONENEIGHBORUPDATESCHEDULE_H
#define RASTERONENEIGHBORUPDATESCHEDULE_H

/*
 * The current functionality is identical to RasterAllNeighborsUpdateSchedule.
 * The functionality SHOULD be that all of the left neighbors over the entire image are updated, followed by all top neighbors, etc etc.
 * That is, only one neighbor of each node is update during each raster scan of the image.
 *
 */

#include "UpdateSchedule.h"

#include "itkImageRegionConstIterator.h"

class RasterOneNeighborUpdateSchedule : public UpdateSchedule
{
public:
  RasterOneNeighborUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();

private:
  itk::ImageRegionConstIterator<NodeImageType> ImageIterator;
  unsigned int CurrentMessageVectorId;
};

#endif