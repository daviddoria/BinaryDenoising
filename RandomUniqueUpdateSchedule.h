#ifndef RANDOMUNIQUEUPDATESCHEDULE_H
#define RANDOMUNIQUEUPDATESCHEDULE_H

/*
 * This update schedule randomly selects a node and updates messages to all of its neighbors.
 * All nodes in the image are visited before any node is visited again.
 */

#include "UpdateSchedule.h"

#include "itkImageRandomNonRepeatingConstIteratorWithIndex.h"

class RandomUniqueUpdateSchedule : public UpdateSchedule
{
public:
  RandomUniqueUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();

private:
  itk::ImageRandomNonRepeatingConstIteratorWithIndex<MessageImageType> randomIterator;
  unsigned int CurrentNeighbor;
};

#endif