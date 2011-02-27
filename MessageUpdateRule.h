#ifndef MESSAGEUPDATERULE_h
#define MESSAGEUPDATERULE_h

class MessageUpdateRule
{
  virtual void Update(MessageVector& messageVector) = 0;
};

#endif