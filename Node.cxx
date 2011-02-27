#include "Node.h"

Node::Node()
{

}

void Node::SetOutgoingMessages(std::vector<MessageVector> messages)
{
  this->OutgoingMessages = messages;
}

unsigned int Node::GetNumberOfNeighbors() const
{
  return this->OutgoingMessages.size();
}

std::vector<MessageVector>& Node::GetOutgoingMessageVectors()
{
  return this->OutgoingMessages;
}

MessageVector& Node::GetOutgoingMessageVector(unsigned int id)
{
  return this->OutgoingMessages[id];
}