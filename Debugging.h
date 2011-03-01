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

#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "Types.h"

namespace Debugging
{
template<typename T>
unsigned int CountNeighborsWithSameValue(typename T::Pointer image, itk::Index<2> pixel);

template<typename T>
unsigned int CountNeighborsWithDifferentValue(typename T::Pointer image, itk::Index<2> pixel);
}

// Display all messages between two nodes
void OutputMessagesBetweenNodes(const itk::Index<2> FromNode, const itk::Index<2> ToNode);

  void OutputBeliefImage();
  void OutputMessageImage();
  void WriteBeliefImage(std::string filename);

#include "Debugging.txx"

#endif