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