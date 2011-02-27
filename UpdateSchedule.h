#ifndef UPDATESCHEDULE_H
#define UPDATESCHEDULE_H

#include "Node.h"
#include "Types.h"

class UpdateSchedule
{
public:

  UpdateSchedule();

  virtual MessageVector& NextMessage() = 0;
  virtual void Initialize() = 0;

  void SetNodeImage(NodeImageType::Pointer);

protected:
  NodeImageType::Pointer NodeImage;

};

#endif