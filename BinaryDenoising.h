/*
Copyright (C) 2011 David Doria, daviddoria@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BINARYDENOISING_H
#define BINARYDENOISING_H

/*
 * This class is a demonstration of using LoopyBP. It takes a noisy binary image and attempts to reduce the noise.
 */

#include "LoopyBP.h"

class BinaryDenoising : public LoopyBP<IntImageType>
{
public:
  BinaryDenoising();

  // Specify the noisy image.
  void SetObservations(IntImageType::Pointer);

  // Specify how much to weight the binary cost function.
  void SetBinaryPenalty(float);

private:

  // Implement the pure virtual cost functions defined in LoopyBP.
  float UnaryCost(int label, itk::Index<2> pixel); // The penalty for choosing a label for a particular node based only on the original observation at that node.
  float BinaryCost(int label1, int label2); // The penalty for assigning neighboring nodes specific labels.

  // The weight of the binary cost function.
  float BinaryPenalty;

  // The noisy image.
  IntImageType::Pointer Observations;
};

#endif