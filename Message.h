#ifndef MESSAGE_H
#define MESSAGE_H

#include "itkIndex.h"

#include <vector>

// This class represents a single message between a pair of nodes.
// As an example, the message m_{2,5}(3) (i.e. the message from node 2 to node 5 about label 3)
// is represented in this class with
// FromNode = (pixel which has node id 2)
// ToNode = (pixel which has node id 5)
// Label = 3

struct Message
{
  Message();

  int Label;
  float Value;
};

std::ostream& operator<<(std::ostream& output, const Message &m);

#endif