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

#include "UpdateSchedule.h"

UpdateSchedule::UpdateSchedule()
{
  this->NodeImage = NULL;
  this->Finished = false;
}

void UpdateSchedule::SetNodeImage(NodeImageType::Pointer image)
{
  this->NodeImage = image; // This must not be copied, but rather the pointer assigned. That is because UpdateSchedule subclasses return pixels of the OutgoingMessageImage by reference, and we want these to be identical to the pixels in the OutgoingMessageImage of the LoopyBP subclass's OutgoingMessageImage
}


void UpdateSchedule::SetNumberOfPasses(unsigned int passes)
{
  this->NumberOfPasses = passes;
}