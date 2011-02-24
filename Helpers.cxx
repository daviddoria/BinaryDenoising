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