#ifndef MESSAGEVECTORCOMPARISON_H
#define MESSAGEVECTORCOMPARISON_H

#include "itkIndex.h"

#include "MessageVector.h"
#include "Node.h"

bool operator<(const itk::Index<2> &index1, const itk::Index<2> &index2);

struct MessageVectorComparison
{
  bool operator()(const MessageVector& mv1, const MessageVector& mv2) const;
};

struct IndexComparison
{
  bool operator()(const itk::Index<2>& index1, const itk::Index<2>& index2) const;
};

struct NodeComparison
{
  bool operator()(const Node& node1, const Node& node2) const;
};

struct NodePointerComparison
{
  bool operator()(const Node* node1, const Node* node2) const;
};

#endif