#include "BinaryDenoising.h"

BinaryDenoising::BinaryDenoising()
{
  this->BinaryPenalty = 50;
  this->Observations = NULL;
}

void BinaryDenoising::SetBinaryPenalty(float penalty)
{
 this->BinaryPenalty = penalty;
}

void BinaryDenoising::SetObservations(IntImageType::Pointer observations)
{
  // Setup observations
  if(!this->Observations)
    {
    this->Observations = IntImageType::New();
    }
  this->Observations->Graft(observations);

  // Initialize the output image to be equal to the observations
  if(!this->Image)
    {
    this->Image = IntImageType::New();
    }

  Helpers::DeepCopy<IntImageType>(this->Observations, this->Image);

}


float BinaryDenoising::UnaryCost(int label, itk::Index<2> pixel)
{
  float cost = 0.0;
  if(label == this->Observations->GetPixel(pixel)) 
    {
    cost = 0.2;// if labels are the same, the cost is low
    }
  else
    {
    cost = .8;// if labels are the different, the cost is high
    }

  return cost;
}

float BinaryDenoising::BinaryCost(int label1, int label2)
{
  // if labels are the same, the cost is low (zero)
  float cost = this->BinaryPenalty * abs(label1 - label2);
  
  return cost;
}