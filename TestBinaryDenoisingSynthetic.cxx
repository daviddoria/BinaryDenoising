#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "Types.h"
#include "BinaryDenoising.h"
#include "Helpers.h"

#include <iomanip>
#include <sstream>

void CreateTestImage(IntImageType::Pointer);

int main(int argc, char *argv[])
{
  IntImageType::Pointer image = IntImageType::New();
  CreateTestImage(image);

  Helpers::WriteImage<IntImageType>(image, "noisy.png");

  BinaryDenoising binaryDenoising;
  binaryDenoising.SetObservations(image);
  binaryDenoising.CreateBinaryLabelSet();

  binaryDenoising.CreateAndInitializeMessages(1.0);
  //binaryDenoising.CreateAndInitializeMessages(0.0);

  unsigned int numberOfIterations = 100;
  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    binaryDenoising.Iterate();

    std::stringstream filename;
    filename << "result_" << std::setfill('0') << std::setw(5) << i << ".png";
    Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), filename.str());

    }

  return EXIT_SUCCESS;
}

void CreateTestImage(IntImageType::Pointer image)
{
  itk::Size<2> size;
  size.Fill(3);

  itk::Index<2> start;
  start.Fill(0);

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<IntImageType> imageIterator(image, image->GetLargestPossibleRegion());

  itk::Index<2> center;
  center.Fill(1);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex() != center)
      {
      imageIterator.Set(1);
      }
    else
      {
      imageIterator.Set(0);
      }
    ++imageIterator;
    }

}