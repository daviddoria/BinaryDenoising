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

#ifndef MESSAGEVECTOR_H
#define MESSAGEVECTOR_H

#include "itkIndex.h"

#include "Message.h"

class Node;

#include <vector>

// This class represents all of the outgoing messages between a node and one of its neighbors.

class MessageVector
{
public:
  MessageVector();

  // Divide each message's value by the sum of all the message values (so the resulting values sum to 1).
  void Normalize();

  // The origin node (where the message started).
  Node* OriginNode;

  // The destination node (where the message is going).
  Node* DestinationNode;

  // The vector of messages between two nodes (one message for each label).
  std::vector<Message> Messages;

  // Get a reference to the messages vector
  std::vector<Message>& GetMessages();

  // Get a reference to a particular message
  Message& GetMessage(const unsigned int i);

  // Get a reference to the message with a particular label
  Message& GetMessageWithLabel(const int label);

  // Get the number of messages in the vector of messages between the two nodes (this is the same as the number of labels).
  unsigned int GetNumberOfMessages() const;

  // Get all of the message values.
  std::vector<float> GetAllMessageValues() const;

  // Add a message to the message vector.
  void AddMessage(Message);

  // Returns the validity of the ToNode (That is, "Does this message go somewhere valid?")
  bool IsValid();

  /////// For debugging //////
  void OutputMessage(const unsigned int i) const;
  void OutputAllMessageValues() const;
private:

};

// Enable the message vector to be written to std::out.
std::ostream& operator<<(std::ostream& output, const MessageVector &m);

#endif