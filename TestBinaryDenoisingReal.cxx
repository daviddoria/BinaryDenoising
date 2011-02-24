#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "Types.h"
#include "BinaryDenoising.h"
#include "Helpers.h"

#include <iomanip>
#include <sstream>

int main(int argc, char *argv[])
{
  std::string filename = argv[1];

  typedef itk::ImageFileReader<IntImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->Update();

  IntImageType::Pointer binaryImage = IntImageType::New();
  Helpers::ConvertToBinaryImage<IntImageType>(reader->GetOutput(), binaryImage);

  Helpers::WriteBinaryImage<IntImageType>(binaryImage, "noisy.png");

  BinaryDenoising binaryDenoising;
  binaryDenoising.SetObservations(binaryImage);
  binaryDenoising.CreateBinaryLabelSet();

  binaryDenoising.CreateAndInitializeMessages(1.0);
  //binaryDenoising.CreateAndInitializeMessages(0.0);

  std::cout << "Initializations:" << std::endl;

  //binaryDenoising.OutputBeliefImage();
  //binaryDenoising.OutputMessageImage();

  unsigned int numberOfPixels = binaryImage->GetLargestPossibleRegion().GetNumberOfPixels();
  unsigned int numberOfIterations = 50000;
  //unsigned int numberOfIterations = numberOfPixels * 10 * 8 * 2; // 8 neighbors on average, 2 messages per neighbor, 10 full iterations
  unsigned int fullPassCounter = 0;

  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    //std::cout << "Performing iteration " << i << "..." << std::endl;

    binaryDenoising.Iterate();
/*
    std::stringstream filename;
    filename << "belief_" << std::setfill('0') << std::setw(5) << i << ".png";
    ++fullPassCounter;
    binaryDenoising.WriteBeliefImage(filename.str());
*/

    if((i % (numberOfPixels * 4 * 2) == 0) && (i > 0))
      {
      std::cout << "Full pass " << fullPassCounter << std::endl;
      std::stringstream filename;
      filename << "result_" << std::setfill('0') << std::setw(5) << fullPassCounter << ".png";
      ++fullPassCounter;
      Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), filename.str());
      }

    }

  Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), "result.png");

  /*
  unsigned int numberOfIterations = 1e3;
  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    //binaryDenoising.SumProduct();
    //binaryDenoising.MaxProduct();
    binaryDenoising.Iterate();
    }

  Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), "result.png");
  */

  return EXIT_SUCCESS;
}
