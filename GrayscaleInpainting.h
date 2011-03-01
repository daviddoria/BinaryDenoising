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