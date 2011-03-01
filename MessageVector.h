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
  void Normalize();

  Node* FromNode;
  Node* ToNode;

  std::vector<Message> Messages; // The vector of messages

  std::vector<Message>& GetMessages();
  Message& GetMessage(const unsigned int i);
  Message& GetMessageWithLabel(const int label);
  unsigned int GetNumberOfMessages() const;

  void OutputMessage(const unsigned int i) const;
  void OutputAllMessageValues() const;
  std::vector<float> GetAllMessageValues() const;

  void AddMessage(Message);

  bool IsValid();

private:

};

std::ostream& operator<<(std::ostream& output, const MessageVector &m);

#endif