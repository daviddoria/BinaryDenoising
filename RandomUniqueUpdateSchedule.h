#ifndef RANDOMUNIQUEUPDATESCHEDULE_H
#define RANDOMUNIQUEUPDATESCHEDULE_H

/*
 * This update schedule randomly selects a node and updates messages to all of its neighbors.
 * All nodes in the image are visited before any node is visited again.
 */

#include "UpdateSchedule.h"

#include "itkImageRandomNonRepeatingIteratorWithIndex.h"

class RandomUniqueUpdateSchedule : public UpdateSchedule
{
public:
  RandomUniqueUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();
  unsigned int GetCurrentPass();
  bool IsFinished();
  void SetNumberOfPasses(unsigned int passes);

private:
  itk::ImageRandomNonRepeatingIteratorWithIndex<NodeImageType> RandomIterator;
  unsigned int CurrentNeighbor;
  unsigned int CurrentPass;
};

#endif