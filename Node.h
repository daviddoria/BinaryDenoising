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

private:
  std::vector<MessageVector> OutgoingMessages;

};

#endif

