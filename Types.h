#ifndef TYPES_H
#define TYPES_H

#include "itkImage.h"

#include "Message.h"

//typedef itk::Image< bool, 2> BoolImageType;
typedef itk::Image< int, 2> IntImageType;
typedef itk::Image< float, 2> FloatImageType;
typedef itk::Image< unsigned char, 2> UnsignedCharImageType;

// The MessageImageType is a vector of MessageVector's. Each MessageVector is the set of outgoing messages to a particular node
typedef itk::Image<std::vector<MessageVector>, 2> MessageImageType;

#endif