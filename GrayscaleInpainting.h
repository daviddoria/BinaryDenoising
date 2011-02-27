#ifndef GRAYSCALEINPAINTING_H
#define GRAYSCALEINPAINTING_H

#include "LoopyBP.h"

class GrayscaleInpainting : public LoopyBP<IntImageType>
{
public:
  GrayscaleInpainting();

  void SetObservations(IntImageType::Pointer);

  IntImageType::Pointer GetObservations();

private:
  float UnaryCost(int label, itk::Index<2> pixel);
  float BinaryCost(int label1, int label2);


  IntImageType::Pointer Observations;
};

#endif