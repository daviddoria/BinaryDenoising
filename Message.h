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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "itkIndex.h"

#include <vector>

/* This class represents a single message between a pair of nodes.
 * As an example, the message m_{2,5}(3) (i.e. the message from node 2 to node 5 about label 3)
 * is represented in this class with
 * FromNode = (pixel which has node id 2)
 * ToNode = (pixel which has node id 5)
 * Label = 3
 */

struct Message
{
  Message();

  int Label;
  float Value;
};

std::ostream& operator<<(std::ostream& output, const Message &m);

#endif