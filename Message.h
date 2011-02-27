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

struct MessageVector
{
  MessageVector();
  void Normalize();

  itk::Index<2> FromNode;
  itk::Index<2> ToNode;

  std::vector<Message> Messages; // The vector of messages

  std::vector<Message>& GetMessages();
  Message& GetMessage(const unsigned int i);
  Message& GetMessageWithLabel(const int label);
  unsigned int GetNumberOfMessages() const;

  void OutputMessage(const unsigned int i) const;
  void OutputAllMessageValues() const;
  std::vector<float> GetAllMessageValues() const;

  void AddMessage(Message);
};



std::ostream& operator<<(std::ostream& output, const Message &m);
std::ostream& operator<<(std::ostream& output, const MessageVector &m);

#endif