#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "Types.h"
#include "GrayscaleInpainting.h"
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

  Helpers::WriteCastedImage<IntImageType>(reader->GetOutput(), "noisy.png");

  std::vector<int> labelSet(256);
  for(unsigned int i = 0; i <= 256; i++)
    {
    labelSet[i] = i;
    }
    
  GrayscaleInpainting grayscaleInpainting;
  grayscaleInpainting.SetLabelSet(labelSet);
  grayscaleInpainting.SetObservations(reader->GetOutput());
  grayscaleInpainting.SetScheduleToRandom(); // must set the schedule before calling CreateAndInitializeMessages!
  //binaryDenoising.SetScheduleToRasterOneNeighbor();
  grayscaleInpainting.CreateAndInitializeMessages(1.0);

  unsigned int numberOfPixels = reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels();
  unsigned int iterationsPerPass = numberOfPixels * 4 * 2; // 4 neighbors per pixel (approximately) and 2 messages per neighbor

  unsigned int numberOfPasses = 1;
  unsigned int fullPassCounter = 0;
  
  unsigned int numberOfIterations = numberOfPasses * iterationsPerPass;
  //unsigned int numberOfIterations = 10;

  std::cout << "Performing " << numberOfIterations << " total iterations." << std::endl;
  
  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    std::cout << "Performing iteration " << i << "..." << std::endl;

    grayscaleInpainting.Iterate();


    if((i % iterationsPerPass == 0) && (i > 0))
      {
      std::cout << "Full pass " << fullPassCounter << std::endl;
      std::stringstream filename;
      filename << "result_" << std::setfill('0') << std::setw(5) << fullPassCounter << ".png";
      ++fullPassCounter;
      Helpers::WriteCastedImage<IntImageType>(grayscaleInpainting.GetResult(), filename.str());
      }

    }

  Helpers::WriteCastedImage<IntImageType>(grayscaleInpainting.GetResult(), "result.png");
  Helpers::WriteCastedImage<IntImageType>(grayscaleInpainting.GetImage(), "OriginalImage.png");
  Helpers::WriteCastedImage<IntImageType>(grayscaleInpainting.GetObservations(), "Observations.png");

  return EXIT_SUCCESS;
}
