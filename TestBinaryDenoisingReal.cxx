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

  binaryDenoising.SetScheduleToRandomUnique();

  binaryDenoising.SetUpdateToSumProduct();
  //binaryDenoising.SetUpdateToMaxProduct();
  //binaryDenoising.SetUpdateToMinSum();
  //binaryDenoising.SetUpdateToMaxSum();

  binaryDenoising.SetBinaryPenalty(1.);
  binaryDenoising.CreateAndInitializeMessages(1.0);
  binaryDenoising.SetNumberOfPasses(2);
  binaryDenoising.Initialize();

  while(!binaryDenoising.IsFinished())
    {

    binaryDenoising.Iterate();
/*
    std::stringstream filename;
    filename << "belief_" << std::setfill('0') << std::setw(5) << i << ".png";
    ++fullPassCounter;
    binaryDenoising.WriteBeliefImage(filename.str());
*/

/*
    if((i % (numberOfPixels * 4 * 2) == 0) && (i > 0))
      {
      std::cout << "Full pass " << fullPassCounter << std::endl;
      std::stringstream filename;
      filename << "result_" << std::setfill('0') << std::setw(5) << fullPassCounter << ".png";
      ++fullPassCounter;
      Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), filename.str());
      }
*/
    }

  Helpers::WriteBinaryImage<IntImageType>(binaryDenoising.GetResult(), "result.png");

  return EXIT_SUCCESS;
}
