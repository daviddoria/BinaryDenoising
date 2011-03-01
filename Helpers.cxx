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

#include "Helpers.h"

#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkRandomImageSource.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageFileWriter.h"

namespace Helpers
{

void NormalizeVector(std::vector<float> &vec)
{
  float sum = 0.;
  for(unsigned int i = 0; i < vec.size(); i++)
    {
    sum += vec[i];
    }
  for(unsigned int i = 0; i < vec.size(); i++)
    {
    vec[i] /= sum;
    }
}

void AddNoise(IntImageType::Pointer image, float percentToFlip)
{
  // eg. percentToFlip is .9 if 90% of pixels should be flipped
  typedef itk::Image< float, 2> FloatImageType;
  itk::RandomImageSource<FloatImageType>::Pointer randomImageSource =
    itk::RandomImageSource<FloatImageType>::New();
  randomImageSource->SetNumberOfThreads(1); // to produce non-random results
  randomImageSource->SetSize(image->GetLargestPossibleRegion().GetSize());
  randomImageSource->SetSpacing(image->GetSpacing());
  randomImageSource->SetOrigin(image->GetOrigin());
  randomImageSource->SetMin(0);
  randomImageSource->SetMax(1);
  randomImageSource->Update();

  itk::ImageRegionConstIterator<FloatImageType> noiseImageIterator(randomImageSource->GetOutput(),
                                                                   randomImageSource->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<IntImageType> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int numberFlipped = 0;
  while(!imageIterator.IsAtEnd())
    {
    //std::cout << "Noise image: " << noiseImageIterator.Get() << std::endl;
    if(noiseImageIterator.Get() < percentToFlip)
      {
      imageIterator.Set(!imageIterator.Get());
      ++numberFlipped;
      }
    ++imageIterator;
    ++noiseImageIterator;
    }
  std::cout << "Corrupted " << numberFlipped << " pixels. (" << 100.*static_cast<float>(numberFlipped)/image->GetLargestPossibleRegion().GetNumberOfPixels() << " percent)" << std::endl;

}

}// end namespace