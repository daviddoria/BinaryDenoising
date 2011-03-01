#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "Types.h"
#include "BinaryDenoising.h"
#include "Helpers.h"

void CreateTestImage(IntImageType::Pointer);

int main(int argc, char *argv[])
{
  IntImageType::Pointer image = IntImageType::New();
  CreateTestImage(image);

  BinaryDenoising binaryDenoising;
  binaryDenoising.SetObservations(image);
  binaryDenoising.CreateBinaryLabelSet();
  binaryDenoising.SetUpdateToSumProduct();
  //binaryDenoising.SetUpdateToMaxProduct();
  //binaryDenoising.SetUpdateToMaxSum();
  //binaryDenoising.SetUpdateToMinSum();
  binaryDenoising.CreateAndInitializeMessages(1.0);

  /*
  MessageVector& messageVector = binaryDenoising.Schedule.NextMessage();

  std::cout << "maxproduct: " << binaryDenoising.SumProductMessageUpdate(messageVector) << std::endl;

  std::cout << "Updated messages: " << std::endl;
  messageVector.OutputAllMessageValues();
  */
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