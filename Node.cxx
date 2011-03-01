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
  //std::cout << "There are " << this->OutgoingMessages.size() << " neighbors." << std::endl;
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

void Node::SetGridIndex(itk::Index<2> index)
{
  this->GridIndex = index;
}

itk::Index<2> Node::GetGridIndex() const
{
  return this->GridIndex;
}