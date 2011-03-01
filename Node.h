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

/*
 * This class represents a node of the MRF.
 * It stores all outgoing messages to valid neighbors.
 */

class Node
{
public:

  Node();

  // Set all message vectors originating at this node.
  void SetOutgoingMessages(std::vector<MessageVector>);

  // Get all message vectors originating at this node.
  std::vector<MessageVector>& GetOutgoingMessageVectors();

  // Get the message vector to the ith neighbor.
  MessageVector& GetOutgoingMessageVector(unsigned int);

  // Determine how many neighbors the node has. All neighbors may not be valid (check the DestinationNode of the message to the neighbor).
  unsigned int GetNumberOfNeighbors() const;

  // Set the location of the node in the image.
  void SetGridIndex(itk::Index<2>);

  // Get the location of the node in the image.
  itk::Index<2> GetGridIndex() const;

private:

  std::vector<MessageVector> OutgoingMessages;

  itk::Index<2> GridIndex;
};

#endif

