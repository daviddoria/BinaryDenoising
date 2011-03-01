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

  std::cout << "Initializations:" << std::endl;

  binaryDenoising.OutputBeliefImage();
  //binaryDenoising.OutputMessageImage();

  unsigned int numberOfIterations = 100;
  for(unsigned int i = 0; i < numberOfIterations; i++) // since we are using the raster schedule, one iteration is simply one message
    {
    binaryDenoising.Iterate();

    std::cout << "Iteration " << i << std::endl;
    binaryDenoising.OutputBeliefImage();
    //binaryDenoising.OutputMessageImage();

    /*
    std::stringstream filename;
    filename << "result_" << std::setfill('0') << std::setw(5) << i << ".png";
    Helpers::WriteImage<IntImageType>(binaryDenoising.GetResult(), filename.str());
    */
    }

  Helpers::WriteBinaryImage<IntImageType>(binaryDenoising.GetResult(), "result.png");

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