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