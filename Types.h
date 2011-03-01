#ifndef TYPES_H
#define TYPES_H

#include "itkImage.h"

#include "Node.h"

//typedef itk::Image< bool, 2> BoolImageType;
typedef itk::Image< int, 2> IntImageType;
typedef itk::Image< float, 2> FloatImageType;
typedef itk::Image< unsigned char, 2> UnsignedCharImageType;

// Each pixel of a NodeImageType image is a node of an MRF.
typedef itk::Image<Node*, 2> NodeImageType;

#endif