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

#ifndef UPDATESCHEDULE_H
#define UPDATESCHEDULE_H

#include "Node.h"
#include "Types.h"

class UpdateSchedule
{
public:

  UpdateSchedule();

  // Return a reference to the next message vector to process.
  virtual MessageVector& NextMessage() = 0;

  // Once all of the properties have been set, initialize the schedule.
  virtual void Initialize() = 0;

  // Give the schedule access to the MRF.
  void SetNodeImage(NodeImageType::Pointer);

  // Determine of the algorithm has converged or reached a stopping condition.
  virtual bool IsFinished() = 0;

  // Specify how many passes over the entire image should be performed.
  void SetNumberOfPasses(unsigned int passes);

protected:

  // The schedule must have a pointer to the node image.
  NodeImageType::Pointer NodeImage;

  unsigned int NumberOfPasses;

};

#endif