#include "itkNeighborhoodIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"

#include <iomanip>

namespace Helpers
{

template<typename T>
void ConvertToBinaryImage(typename T::Pointer image, IntImageType::Pointer binary)
{
  binary->SetRegions(image->GetLargestPossibleRegion());
  binary->Allocate();

  itk::ImageRegionConstIterator<T> imageIterator(image, image->GetLargestPossibleRegion());
  itk::ImageRegionIterator<IntImageType> binaryIterator(binary, binary->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get())
      {
      binaryIterator.Set(1);
      }
    else
      {
      binaryIterator.Set(0);
      }

    ++imageIterator;
    ++binaryIterator;
    }
}


template<typename T>
void OutputVector(std::vector<T> vec)
{
  std::cout << "{ ";
  for(unsigned int i = 0; i < vec.size(); i++)
    {
    std::cout << std::fixed << std::setprecision(2) << vec[i];
    if(i < vec.size() -1)
      {
      std::cout << ", ";
      }
    }
  std::cout << " }";
}

template<typename T>
std::vector<itk::Index<2> > Get8Neighbors(typename T::Pointer image, itk::Index<2> pixel)
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
std::vector<itk::Index<2> > Get4Neighbors(typename T::Pointer image, itk::Index<2> pixel)
{
  std::vector<itk::Index<2> > neighbors;


  itk::Offset<2> top;
  top[0] = 0;
  top[1] = 1;

  itk::Offset<2> bottom;
  bottom[0] = 0;
  bottom[1] = -1;

  itk::Offset<2> left;
  left[0] = -1;
  left[1] = 0;

  itk::Offset<2> right;
  right[0] = 1;
  right[1] = 0;

  std::vector<itk::Offset<2> > offsets;
  offsets.push_back(top);
  offsets.push_back(bottom);
  offsets.push_back(left);
  offsets.push_back(right);

  for(unsigned int i = 0; i < offsets.size(); i++)
    {
    itk::Index<2> index = pixel + offsets[i];
    if(image->GetLargestPossibleRegion().IsInside(index))
      {
      neighbors.push_back(index);
      }
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
void WriteBinaryImage(typename T::Pointer image, std::string filename)
{
  typedef itk::Image< unsigned char, 2> UnsignedCharImageType;
  UnsignedCharImageType::Pointer output = UnsignedCharImageType::New();
  output->SetRegions(image->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<T> inputIterator(image, image->GetLargestPossibleRegion());
  itk::ImageRegionIterator<UnsignedCharImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    if(inputIterator.Get())
      {
      outputIterator.Set(255);
      }
    else
      {
      outputIterator.Set(0);
      }

    ++inputIterator;
    ++outputIterator;
    }

  typedef  itk::ImageFileWriter< UnsignedCharImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(output);
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