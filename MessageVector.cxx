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

#include "MessageVector.h"

MessageVector::MessageVector()
{
  // Initializations
  this->OriginNode = NULL;
  this->DestinationNode = NULL;
}

bool MessageVector::IsValid()
{
  // The message is invalid if the DestinationNode is not valid.
  return this->DestinationNode;
}


void MessageVector::AddMessage(Message m)
{
  // Add a message to the vector.
  this->Messages.push_back(m);
}

std::vector<Message>& MessageVector::GetMessages()
{
  // Return a reference to the vector of messages
  return this->Messages;
}

Message& MessageVector::GetMessage(const unsigned int i)
{
  // Return a reference to the ith message in the vector
  return this->Messages[i];
}

unsigned int MessageVector::GetNumberOfMessages() const
{
  // Return the number of messages in the vector.
  return this->Messages.size();
}

Message& MessageVector::GetMessageWithLabel(const int label)
{
  // Search the vector for a message with a particular label.
  // Display an error and quit if one is not found.

  for(unsigned int i = 0; i < this->Messages.size(); i++)
    {
    if(this->Messages[i].Label == label)
      {
      return this->Messages[i];
      }
    }
  std::cerr << "No message found with label " << label << std::endl;
  exit(-1);
}

void MessageVector::Normalize()
{
  // Divide each message's value by the sum of all the message values (so the resulting values sum to 1).

  float sum = 0.0;
  for(unsigned int i = 0; i < this->Messages.size(); i++)
    {
    sum += this->Messages[i].Value;
    }

  for(unsigned int i = 0; i < this->Messages.size(); i++)
    {
    this->Messages[i].Value /= sum;
    }
}


void MessageVector::OutputMessage(const unsigned int i) const
{
  std::cout << this->Messages[i];
}

void MessageVector::OutputAllMessageValues() const
{
  std::cout << "{ ";
  for(unsigned int i = 0; i < this->Messages.size(); i++)
    {
    std::cout << this->Messages[i].Value << " ";
    }
  std::cout << " }" << std::endl;
}

std::vector<float> MessageVector::GetAllMessageValues() const
{
  // Extract the value of all messages and return them in a vector.

  std::vector<float> values;
  for(unsigned int i = 0; i < this->Messages.size(); i++)
    {
    values.push_back(this->Messages[i].Value);
    }
  return values;
}

std::ostream& operator<<(std::ostream& output, const MessageVector &messageVector)
{
  // Output operator.

  output << "MessageVector: " << std::endl
         << "Origin node: " << messageVector.OriginNode << std::endl
         << "Destination node: " << messageVector.DestinationNode << std::endl;
  for(unsigned int i = 0; i < messageVector.GetNumberOfMessages(); i++)
    {
    messageVector.OutputMessage(i);
    }

  return output;
}
