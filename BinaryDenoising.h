#ifndef BINARYDENOISING_H
#define BINARYDENOISING_H

#include "LoopyBP.h"

class BinaryDenoising : public LoopyBP<IntImageType>
{
public:
  BinaryDenoising();

  void SetObservations(IntImageType::Pointer);
  void SetBinaryPenalty(float);

private:
  float UnaryCost(int label, itk::Index<2> pixel);
  float BinaryCost(int label1, int label2);

  float BinaryPenalty;

  IntImageType::Pointer Observations;
};

#endif