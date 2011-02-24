#include "RandomUpdateSchedule.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

RandomUpdateSchedule::RandomUpdateSchedule()
{
}

void RandomUpdateSchedule::Initialize()
{
  this->randomIterator = itk::ImageRandomConstIteratorWithIndex<MessageImageType> (this->OutgoingMessageImage,
                                                                                   this->OutgoingMessageImage->GetLargestPossibleRegion());
  this->randomIterator.SetNumberOfSamples(1);
}

MessageVector& RandomUpdateSchedule::NextMessage()
{
  this->randomIterator.GoToBegin();
  itk::Index<2> randomIndex = this->randomIterator.GetIndex();

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer generator = GeneratorType::New();
  generator->Initialize();
  unsigned int randomMessageVector = generator->GetIntegerVariate(this->OutgoingMessageImage->GetPixel(randomIndex).size() -1);

  MessageVector& messageVector = this->OutgoingMessageImage->GetPixel(randomIndex)[randomMessageVector];
  return messageVector;
}
