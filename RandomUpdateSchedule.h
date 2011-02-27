#ifndef RANDOMUPDATESCHEDULE_H
#define RANDOMUPDATESCHEDULE_H

/*
 * This update schedule randomly selects a node and a neighbor to update the messages between.
 */

#include "UpdateSchedule.h"

#include "itkImageRandomConstIteratorWithIndex.h"

class RandomUpdateSchedule : public UpdateSchedule
{
public:
  RandomUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();

private:
  itk::ImageRandomConstIteratorWithIndex<NodeImageType> RandomIterator;

};

#endif