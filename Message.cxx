#include "Message.h"


Message::Message()
{
  this->Label = 0;
  this->Value = 1.0;
}

std::ostream& operator<<(std::ostream& output, const Message &m)
{
  output << "Message: " << std::endl
         << "Label: " << m.Label << std::endl
         << "Value: " << m.Value << std::endl << std::endl;
  return output;
}


