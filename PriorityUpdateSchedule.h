#ifndef PRIORITYUPDATESCHEDULE_H
#define PRIORITYUPDATESCHEDULE_H

/*
 * This update keeps track of the "priority" (Komodakis Priority-BP) of a node (related to the beliefs of the labels)
 * and processes the node with the highest priority that hasn't yet been processed.
 */

#include <queue>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include "itkIndex.h"

#include "Message.h"

bool operator<(const itk::Index<2> &index1, const itk::Index<2> &index2)
{
  if((index1[0] < index2[0]))
  {
    return true;
  }

  if(index1[0] == index2[0])
    {
    if(index1[1] < index2[1])
      {
      return true;
      }
    }

  return false;
}

struct MessageVectorComparison
{
  bool operator()(const MessageVector& mv1, const MessageVector& mv2) const
  {
    if((mv1.FromNode < mv2.FromNode))
    {
      return true;
    }

    if(mv1.FromNode == mv2.FromNode)
      {
      if(mv1.ToNode < mv2.ToNode)
        {
        return true;
        }
      }

    return false;
  }
};

#include "UpdateSchedule.h"

#include "itkImageRandomConstIteratorWithIndex.h"

class PriorityUpdateSchedule : public UpdateSchedule
{
public:
  PriorityUpdateSchedule();
  MessageVector& NextMessage();
  void Initialize();
  void SetBeliefThreshold(float threshold);
  
private:

  float BeliefThreshold;
  
  float ComputePriority();
  
  std::priority_queue<MessageVector, std::vector<MessageVector>, MessageVectorComparison> Priorities;
};

#endif