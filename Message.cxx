#include "Message.h"

MessageVector::MessageVector()
{
  itk::Index<2> zero;
  zero.Fill(0);

  this->FromNode = zero;
  this->ToNode = zero;
}

void MessageVector::AddMessage(Message m)
{
  this->Messages.push_back(m);
}

std::vector<Message>& MessageVector::GetMessages()
{
  return this->Messages;
}

Message& MessageVector::GetMessage(const unsigned int i)
{
  return this->Messages[i];
}

unsigned int MessageVector::GetNumberOfMessages() const
{
  return this->Messages.size();
}

Message& MessageVector::GetMessageWithLabel(const int label)
{
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


Message::Message()
{
  this->Label = 0;
  this->Value = 1.0;
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

std::ostream& operator<<(std::ostream& output, const MessageVector &messageVector)
{
  output << "MessageVector: " << std::endl
         << "FromNode: " << messageVector.FromNode << std::endl
         << "ToNode: " << messageVector.ToNode << std::endl;
  for(unsigned int i = 0; i < messageVector.GetNumberOfMessages(); i++)
    {
    messageVector.OutputMessage(i);
    }

  return output;
}

std::ostream& operator<<(std::ostream& output, const Message &m)
{
  output << "Message: " << std::endl
         << "Label: " << m.Label << std::endl
         << "Value: " << m.Value << std::endl << std::endl;
  return output;
}