#include "itkNeighborhoodIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"

namespace Helpers
{

template<typename T>
std::vector<itk::Index<2> > GetNeighbors(typename T::Pointer image, itk::Index<2> pixel)
{
  std::vector<itk::Index<2> > neighbors;

  itk::Size<2> oneRadius;
  oneRadius.Fill(1);

  itk::Size<2> singlePixelSize;
  singlePixelSize.Fill(1);

  itk::ImageRegion<2> singlePixelRegion(pixel, singlePixelSize);

  itk::NeighborhoodIterator<T> iterator(oneRadius, image, singlePixelRegion);

  while(!iterator.IsAtEnd())
    {
    for(unsigned int i = 0; i < 9; i++)
      {
      itk::Index<2> index = iterator.GetIndex(i);

      bool IsInBounds;
      iterator.GetPixel(i, IsInBounds);
      if(IsInBounds)
	{
	if(index != pixel) // don't include the center (current) pixel
	  {
	  neighbors.push_back(iterator.GetIndex(i));
	  //std::cout << "added " << iterator.GetIndex(i) << " as a neighbor of " << pixel << std::endl;
	  }
	}
      }
    ++iterator;
    }

  return neighbors;
}

template<typename T>
void WriteImage(typename T::Pointer image, std::string filename)
{
  typedef itk::Image< unsigned char, 2> UnsignedCharImageType;

  typedef itk::RescaleIntensityImageFilter< T, UnsignedCharImageType > RescaleFilterType;
  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  typedef  itk::ImageFileWriter< UnsignedCharImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
}

template<typename T>
void DeepCopy(typename T::Pointer inputImage, typename T::Pointer outputImage)
{
  outputImage->SetRegions(inputImage->GetLargestPossibleRegion());
  outputImage->Allocate();

  itk::ImageRegionConstIterator<T> inputImageIterator(inputImage, inputImage->GetLargestPossibleRegion());
  itk::ImageRegionIterator<T> outputImageIterator(outputImage, outputImage->GetLargestPossibleRegion());

  while(!inputImageIterator.IsAtEnd())
    {
    outputImageIterator.Set(inputImageIterator.Get());
    ++inputImageIterator;
    ++outputImageIterator;
    }

}

} // end namespace