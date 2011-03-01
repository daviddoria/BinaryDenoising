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

#ifndef NODE_H
#define NODE_H

#include "MessageVector.h"

#include <vector>

class Node
{
public:
  Node();
  void SetOutgoingMessages(std::vector<MessageVector>);
  std::vector<MessageVector>& GetOutgoingMessageVectors();
  MessageVector& GetOutgoingMessageVector(unsigned int);
  unsigned int GetNumberOfNeighbors() const;

  void SetGridIndex(itk::Index<2>);
  itk::Index<2> GetGridIndex() const;

private:
  std::vector<MessageVector> OutgoingMessages;
  itk::Index<2> GridIndex;
};

#endif

