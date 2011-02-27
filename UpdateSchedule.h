#ifndef UPDATESCHEDULE_H
#define UPDATESCHEDULE_H

#include "Message.h"
#include "Types.h"

class UpdateSchedule
{
public:

  UpdateSchedule();

  virtual MessageVector& NextMessage() = 0;
  virtual void Initialize() = 0;

  void SetOutgoingMessageImage(MessageImageType::Pointer);

protected:
  MessageImageType::Pointer OutgoingMessageImage;

};

#endif