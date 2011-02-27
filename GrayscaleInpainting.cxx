#include "GrayscaleInpainting.h"

GrayscaleInpainting::GrayscaleInpainting()
{
  this->Observations = NULL;
}

IntImageType::Pointer GrayscaleInpainting::GetObservations()
{
  return this->Observations;
}

void GrayscaleInpainting::SetObservations(IntImageType::Pointer observations)
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


float GrayscaleInpainting::UnaryCost(int label, itk::Index<2> pixel)
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
}

float GrayscaleInpainting::BinaryCost(int label1, int label2)
{
  float cost = abs(label1-label2);

  return cost;
}