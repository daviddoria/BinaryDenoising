/*
Copyright (C) 2011 David Doria, daviddoria@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
  bool IsFinished();

private:
  itk::ImageRegionConstIterator<NodeImageType> ImageIterator;
  unsigned int CurrentMessageVectorId;
};

#endif