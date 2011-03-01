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

#ifndef RANDOMUNIQUEUPDATESCHEDULE_H
#define RANDOMUNIQUEUPDATESCHEDULE_H

/*
 * This update schedule randomly selects a node and updates messages to all of its neighbors.
 * All nodes in the image are visited before any node is visited again.
 */

#include "UpdateSchedule.h"

#include "itkImageRandomNonRepeatingIteratorWithIndex.h"

class RandomUniqueUpdateSchedule : public UpdateSchedule
{
public:
  RandomUniqueUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();
  unsigned int GetCurrentPass();
  bool IsFinished();
  void SetNumberOfPasses(unsigned int passes);

private:
  itk::ImageRandomNonRepeatingIteratorWithIndex<NodeImageType> RandomIterator;
  unsigned int CurrentNeighbor;
  unsigned int CurrentPass;
};

#endif