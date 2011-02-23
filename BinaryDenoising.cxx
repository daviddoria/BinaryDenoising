#include "BinaryDenoising.h"

BinaryDenoising::BinaryDenoising()
{

  this->Observations = NULL;
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
    cost = 0.2;
    }
  else
    {
    cost = .8;
    }

  return cost;
  //return exp(-cost);

  //return 0.5;
}

float BinaryDenoising::BinaryCost(int label1, int label2)
{
  float cost = 0.0;

  if(label1 == label2)
    {
    cost = 0.;
    }
  else
    {
    //cost = 1;
    cost = 100.;
    }
  return cost;
  //return exp(-cost);
}