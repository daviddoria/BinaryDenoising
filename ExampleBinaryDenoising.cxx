#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"



#include "Types.h"
#include "MRF.h"
#include "Helpers.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string filename = argv[1];
  
  typedef itk::ImageFileReader<BoolImageType> ReaderType;
 
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->Update();
  
  BoolImageType::Pointer observations = reader->GetOutput();
  observations->DisconnectPipeline();
  Helpers::AddNoise(observations, .1);
  Helpers::WriteImage(observations, "noisy.png");
      
  MRF mrf;
  mrf.SetObservations(observations);
  
  for(unsigned int i = 0; i < 10; i++)
    {
    mrf.Iterate();
    }
    
  Helpers::WriteImage(mrf.GetResult(), "result.png");  
  
  return EXIT_SUCCESS;
}
