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
  typedef itk::ImageFileReader<IntImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  Helpers::WriteImage<IntImageType>(reader->GetOutput(), "noisy.png");

  BinaryDenoising binaryDenoising;
  binaryDenoising.SetObservations(reader->GetOutput());
  binaryDenoising.CreateBinaryLabelSet();

  binaryDenoising.CreateAndInitializeMessages(1.0);
  //binaryDenoising.CreateAndInitializeMessages(0.0);

  unsigned int numberOfPixels = reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels();
  unsigned int numberOfIterations = numberOfPixels * 10 * 8 * 2; // 8 neighbors on average, 2 messages per neighbor, 10 full iterations
  unsigned int fullPassCounter = 0;

  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    //std::cout << "Performing iteration " << i << "..." << std::endl;

    binaryDenoising.Iterate();

    if((i % (numberOfPixels * 8 * 2) == 0) && (i > 0))
      {
      std::cout << "Full pass " << fullPassCounter << std::endl;
      std::stringstream filename;
      filename << "result_" << std::setfill('0') << std::setw(5) << fullPassCounter << ".png";
      ++fullPassCounter;
      Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), filename.str());
      }

    }

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
