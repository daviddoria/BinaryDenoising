#include "PriorityUpdateSchedule.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

PriorityUpdateSchedule::PriorityUpdateSchedule()
{
}

void PriorityUpdateSchedule::Initialize()
{

}

float PriorityUpdateSchedule::ComputePriority()
{
  // not implemented - please see the separate project "Priority Belief Propagation"
}

void PriorityUpdateSchedule::SetBeliefThreshold(float threshold)
{
  this->BeliefThreshold = threshold;
}

MessageVector& PriorityUpdateSchedule::NextMessage()
{
  /*
  this->randomIterator.GoToBegin();
  itk::Index<2> randomIndex = this->randomIterator.GetIndex();

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer generator = GeneratorType::New();
  generator->Initialize();
  unsigned int randomMessageVector = generator->GetIntegerVariate(this->OutgoingMessageImage->GetPixel(randomIndex).size() -1);

  MessageVector& messageVector = this->OutgoingMessageImage->GetPixel(randomIndex)[randomMessageVector];
  return messageVector;
  */
}
