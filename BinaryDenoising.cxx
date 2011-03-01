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

#include "BinaryDenoising.h"

BinaryDenoising::BinaryDenoising()
{
  // Initializations
  this->BinaryPenalty = 50;
  this->Observations = NULL;
}

void BinaryDenoising::SetBinaryPenalty(float penalty)
{
 this->BinaryPenalty = penalty;
}

void BinaryDenoising::SetObservations(IntImageType::Pointer observations)
{
  // Create a new image if one hasn't previously been created.
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
  // A simple cost function which prefers pixels to stay the same as their original observations.

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
  // The penalty for neighboring nodes being assigned specific labels.

  // If labels are the same, the cost is low (zero)
  float cost = this->BinaryPenalty * abs(label1 - label2);

  return cost;
}