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

#ifndef RANDOMUPDATESCHEDULE_H
#define RANDOMUPDATESCHEDULE_H

/*
 * This update schedule randomly selects a node and a neighbor to update the messages between.
 */

#include "UpdateSchedule.h"

#include "itkImageRandomConstIteratorWithIndex.h"

class RandomUpdateSchedule : public UpdateSchedule
{
public:
  RandomUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();
  bool IsFinished();
private:
  itk::ImageRandomConstIteratorWithIndex<NodeImageType> RandomIterator;

};

#endif