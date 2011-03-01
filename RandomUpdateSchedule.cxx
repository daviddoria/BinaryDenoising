#include "RandomUpdateSchedule.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

RandomUpdateSchedule::RandomUpdateSchedule()
{
}

bool RandomUpdateSchedule::IsFinished()
{
  return false;
}

void RandomUpdateSchedule::Initialize()
{
  this->RandomIterator = itk::ImageRandomConstIteratorWithIndex<NodeImageType> (this->NodeImage,
                                                                                   this->NodeImage->GetLargestPossibleRegion());
  this->RandomIterator.SetNumberOfSamples(1);
}

MessageVector& RandomUpdateSchedule::NextMessage()
{
  this->RandomIterator.GoToBegin();
  itk::Index<2> randomIndex = this->RandomIterator.GetIndex();

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer generator = GeneratorType::New();
  generator->Initialize();
  unsigned int randomMessageVector = generator->GetIntegerVariate(this->NodeImage->GetPixel(randomIndex).GetNumberOfNeighbors() -1);

  MessageVector& messageVector = this->NodeImage->GetPixel(randomIndex).GetOutgoingMessageVector(randomMessageVector);
  return messageVector;
}
