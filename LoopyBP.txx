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

#include "LoopyBP.h"

#include "Helpers.h"

#include "itkImageRegionIterator.h"
#include "itkIntensityWindowingImageFilter.h"

#include "RandomUniqueUpdateSchedule.h"
#include "RasterOneNeighborUpdateSchedule.h"

#include <vector>

template <typename T>
LoopyBP<T>::LoopyBP()
{
  this->Image = NULL;
  this->NodeImage = NULL;
  this->Schedule = NULL;
  this->UpdateType = SUMPRODUCT;
}

template <typename T>
bool LoopyBP<T>::IsFinished()
{
  return this->Schedule->IsFinished();
}

template <typename T>
void LoopyBP<T>::SetUpdateToSumProduct()
{
  this->UpdateType = SUMPRODUCT;
}

template <typename T>
void LoopyBP<T>::SetUpdateToMaxProduct()
{
  this->UpdateType = MAXPRODUCT;
}

template <typename T>
void LoopyBP<T>::SetUpdateToMinSum()
{
  this->UpdateType = MINSUM;
}

template <typename T>
void LoopyBP<T>::SetUpdateToMaxSum()
{
  this->UpdateType = MAXSUM;
}

/*
template <typename T>
void LoopyBP<T>::SetScheduleToRandom()
{
  this->Schedule = new RandomUpdateSchedule;
}
*/

template <typename T>
void LoopyBP<T>::SetScheduleToRandomUnique()
{
  this->Schedule = new RandomUniqueUpdateSchedule;
}
/*
template <typename T>
void LoopyBP<T>::SetScheduleToRasterOneNeighbor()
{
  this->Schedule = new RasterOneNeighborUpdateSchedule;
}
*/

template <typename T>
void LoopyBP<T>::Initialize()
{
  this->Schedule->SetNodeImage(this->NodeImage);
  this->Schedule->Initialize();
}


template <typename T>
void LoopyBP<T>::CreateBinaryLabelSet()
{
  std::cout << "Creating binary label set..." << std::endl;
  this->LabelSet.clear();
  this->LabelSet.push_back(0);
  this->LabelSet.push_back(1);
}

template <typename T>
void LoopyBP<T>::SetLabelSet(std::vector<int> labelSet)
{
  this->LabelSet = labelSet;
}

template <typename T>
void LoopyBP<T>::CreateAndInitializeMessages(const float defaultMessageValue)
{
  // For each pixel, create |L| messages to each neighboring pixel
  if(!this->NodeImage)
    {
    this->NodeImage = NodeImageType::New();
    }
  this->NodeImage->SetRegions(this->Image->GetLargestPossibleRegion());
  this->NodeImage->Allocate();
  this->NodeImage->FillBuffer(NULL);

  itk::ImageRegionIterator<NodeImageType> nodeIterator(this->NodeImage, this->NodeImage->GetLargestPossibleRegion());
  nodeIterator.GoToBegin();

  // Create all of the nodes
  while(!nodeIterator.IsAtEnd())
    {
    Node* node = new Node;
    node->SetGridIndex(nodeIterator.GetIndex());
    nodeIterator.Set(node);
    ++nodeIterator;
    }

  // Setup messages between nodes
  nodeIterator.GoToBegin();
  while(!nodeIterator.IsAtEnd())
    {
    Node* node = nodeIterator.Value();

    std::vector<Node*> neighbors = Helpers::Get4Neighbors<NodeImageType>(this->NodeImage, nodeIterator.GetIndex());
    std::vector<itk::Index<2> > neighborIndices = Helpers::Get4NeighborIndices<NodeImageType>(this->NodeImage, nodeIterator.GetIndex());
    std::vector<MessageVector> messageVectors;

    for(unsigned int i = 0; i < neighbors.size(); i++) // add a message to each neighbor
      {

      MessageVector messageVector;
      messageVector.OriginNode = node;

      if(neighbors[i] == NULL)
        {
        messageVector.DestinationNode = NULL;
        }
      else
        {
        messageVector.DestinationNode = neighbors[i];

        //std::cout << "Created a message from " << messageVector.FromNode << " to " << messageVector.ToNode << std::endl;

        for(unsigned int l = 0; l < this->LabelSet.size(); l++) // add a message for each label
          {
          Message m;
          m.Label = l;
          m.Value = defaultMessageValue;
          messageVector.AddMessage(m);
          }
        }
      messageVectors.push_back(messageVector);
      }

    node->SetOutgoingMessages(messageVectors);

    ++nodeIterator;
    }

}

template <typename T>
void LoopyBP<T>::SetNumberOfPasses(unsigned int passes)
{
  this->Schedule->SetNumberOfPasses(passes);
}

template <typename T>
bool LoopyBP<T>::Iterate()
{
  // Determine which message to update (following the schedule)
  MessageVector& messageVector = this->Schedule->NextMessage(); // The message is returned by reference

  if(this->UpdateType == SUMPRODUCT)
    {
    //std::cout << "Using SumProductMessageUpdate" << std::endl;
    return SumProductMessageUpdate(messageVector);
    }
  else if(this->UpdateType == MAXPRODUCT)
    {
    return MaxProductMessageUpdate(messageVector);
    }
  else if(this->UpdateType == MINSUM)
    {
    return MinSumMessageUpdate(messageVector);
    }
  else if(this->UpdateType == MAXSUM)
    {
    return MaxSumMessageUpdate(messageVector);
    }

  return false; // Not yet converged
}

template <typename T>
bool LoopyBP<T>::SumProductMessageUpdate(MessageVector& messageVector)
{
  // This function returns true if nothing changed (converged) (Convergence checking is not yet implemented, so it always returns false at the moment)

  if(messageVector.DestinationNode == NULL)
    {
    return false; // nothing to do
    }

  for(unsigned int l = 0; l < messageVector.GetNumberOfMessages(); l++)
    {
    float sum = 0.0;
    for(unsigned int p = 0; p < this->LabelSet.size(); p++)
      {
      float unary = this->UnaryCost(this->LabelSet[p], messageVector.OriginNode->GetGridIndex());
      float binary = this->BinaryCost(this->LabelSet[p], messageVector.GetMessage(l).Label);

      float product = 1.0;
      //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
      std::vector<Message*> messages = GetIncomingMessagesWithLabel(messageVector.OriginNode, this->LabelSet[p]);
      for(unsigned int k = 0; k < messages.size(); k++)
        {
        product *= messages[k]->Value;
        }
      sum += exp(-unary) * exp(-binary) * product;
      }
    //std::cout << "Setting message to " << sum << std::endl;
    messageVector.GetMessage(l).Value = sum;
    }

  //std::cout << "Messages between node " << messageVector.FromNode << " and " << messageVector.ToNode << " before normalization: " << std::endl;
  //OutputMessagesBetweenNodes(messageVector.FromNode, messageVector.ToNode);

  messageVector.Normalize();

  //std::cout << "Normalized message value: " << std::endl;
  //OutputMessagesBetweenNodes(messageVector.FromNode, messageVector.ToNode);

  //std::cout << std::endl;

  return false; // Not yet converged
}


template <typename T>
bool LoopyBP<T>::MaxProductMessageUpdate(MessageVector& messageVector)
{
  for(unsigned int l = 0; l < messageVector.GetNumberOfMessages(); l++)
    {
    float maxValue = itk::NumericTraits<float>::min();

    for(unsigned int p = 0; p < this->LabelSet.size(); p++)
      {
      float unary = this->UnaryCost(this->LabelSet[p], messageVector.OriginNode->GetGridIndex());
      float binary = this->BinaryCost(this->LabelSet[p], messageVector.GetMessage(l).Label);

      float product = 1.0;
      //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
      std::vector<Message*> messages = GetIncomingMessagesWithLabel(messageVector.OriginNode, this->LabelSet[p]);
      for(unsigned int k = 0; k < messages.size(); k++)
        {
        product *= messages[k]->Value;
        }
      float current = exp(-unary) * exp(-binary) * product;

      if(current > maxValue)
        {
        maxValue = current;
        }
      }
      messageVector.GetMessage(l).Value = maxValue;
    }

  messageVector.Normalize();

  //std::cout << "Normalized message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  //std::cout << std::endl;

  return false; // Not yet converged
}


template <typename T>
bool LoopyBP<T>::MaxSumMessageUpdate(MessageVector& messageVector)
{
  //std::cout << "MaxSumMessageUpdate()" << std::endl;

  for(unsigned int l = 0; l < messageVector.GetNumberOfMessages(); l++)
    {
    float maxValue = itk::NumericTraits<float>::min(); // initialize to a very small value so any value we compute will be larger

    for(unsigned int p = 0; p < this->LabelSet.size(); p++)
      {
      float unary = this->UnaryCost(this->LabelSet[p], messageVector.OriginNode->GetGridIndex());
      float binary = this->BinaryCost(this->LabelSet[p], messageVector.GetMessage(l).Label);

      float sum = 0.0;
      //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
      std::vector<Message*> messages = GetIncomingMessagesWithLabel(messageVector.OriginNode, this->LabelSet[p]);
      for(unsigned int k = 0; k < messages.size(); k++)
        {
        float val = messages[k]->Value;
        sum += log(val);
        }
      float current = - unary - binary + sum;
      current = exp(current);

      if(current > maxValue)
        {
        maxValue = current;
        }
      }
      messageVector.GetMessage(l).Value = maxValue;
    }

  //std::cout << "New message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;

  messageVector.Normalize();

  //std::cout << "Normalized message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  //std::cout << std::endl;

  return false; // Not yet converged
}


template <typename T>
bool LoopyBP<T>::MinSumMessageUpdate(MessageVector& messageVector)
{
  //std::cout << "MinSumMessageUpdate()" << std::endl;

  for(unsigned int l = 0; l < messageVector.GetNumberOfMessages(); l++)
    {
    float minValue = itk::NumericTraits<float>::max();

    for(unsigned int p = 0; p < this->LabelSet.size(); p++)
      {
      float unary = this->UnaryCost(this->LabelSet[p], messageVector.OriginNode->GetGridIndex());
      float binary = this->BinaryCost(this->LabelSet[p], messageVector.GetMessage(l).Label);

      float sum = 0.0;
      //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
      std::vector<Message*> messages = GetIncomingMessagesWithLabel(messageVector.OriginNode, this->LabelSet[p]);
      for(unsigned int k = 0; k < messages.size(); k++)
        {
        float val = messages[k]->Value;
        sum += val;
        }
      float current = unary + binary - sum;

      if(current < minValue)
        {
        minValue = current;
        }
      }
    messageVector.GetMessage(l).Value = minValue;
    }

  //std::cout << "New message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;

  messageVector.Normalize();

  //std::cout << "Normalized message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  //std::cout << std::endl;

  return false; // Not yet converged
}




template <typename T>
float LoopyBP<T>::SumOfMessages(std::vector<Message*> messages)
{
  float sum = 0.0;
  for(unsigned int i = 0; i < messages.size(); i++)
    {
    sum += messages[i]->Value;
    }
  return sum;
}

template <typename T>
float LoopyBP<T>::Belief(Node* const node, const int label)
{
  if(this->UpdateType == MAXSUM)
    {
    return StandardBelief(node, label);
    }
  else if(this->UpdateType == MINSUM)
    {
    return LogBelief(node, label);
    }
  else if(this->UpdateType == SUMPRODUCT || this->UpdateType == MAXPRODUCT || this->UpdateType == MAXSUM)
    {
    return StandardBelief(node, label);
    }
  else
    {
    std::cerr << "This update type is not supported!" << std::endl;
    exit(-1);
    return 0.0;
    }
}

template <typename T>
float LoopyBP<T>::StandardBelief(Node* const node, const int label)
{
  float unary = UnaryCost(label, node->GetGridIndex());
  //std::cout << "UnaryCost: " << unary << std::endl;

  float product = 1.0;

  std::vector<Message*> incomingMessages = GetIncomingMessagesWithLabel(node, label);

  for(unsigned int i = 0; i < incomingMessages.size(); i++)
    {
    product *= incomingMessages[i]->Value;
    }

  //std::cout << "product: " << product << std::endl;

  float belief = exp(-unary) * product;
  return belief;
}


template <typename T>
float LoopyBP<T>::LogBelief(Node* const node, const int label)
{
  // This should only be used if the MinSumMesssageUpdate was used (because the message are already in the log domain)
  float unary = UnaryCost(label, node->GetGridIndex());

  float sum = 0.0;

  std::vector<Message*> incomingMessages = GetIncomingMessagesWithLabel(node, label);

  for(unsigned int i = 0; i < incomingMessages.size(); i++)
    {
    sum += log(incomingMessages[i]->Value); // works better
    }

  //std::cout << "product: " << product << std::endl;

  float belief = unary - sum;
  return belief;
}


template <typename T>
IntImageType::Pointer LoopyBP<T>::GetResult()
{
  itk::ImageRegionIterator<IntImageType> outputIterator(this->Image, this->Image->GetLargestPossibleRegion());
  itk::ImageRegionIterator<NodeImageType> nodeIterator(this->NodeImage, this->NodeImage->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    int label = 0;
    if(this->UpdateType == MAXPRODUCT || this->UpdateType == SUMPRODUCT || this->UpdateType == MAXSUM)
      {
      label = FindLabelWithHighestBelief(nodeIterator.Get());
      }
    else if(this->UpdateType == MINSUM)
      {
      //std::cout << "Computing MinSum Result..." << std::endl;
      label = FindLabelWithLowestBelief(nodeIterator.Get());
      }
    else
      {
      std::cerr << "Update type not supported!" << std::endl;
      exit(-1);
      }
    //std::cout << "Highest belief for " << imageIterator.GetIndex() << " is " << label << std::endl;
    outputIterator.Set(label);

    ++outputIterator;
    ++nodeIterator;
    }

  return this->Image;
}




template <typename T>
int LoopyBP<T>::FindLabelWithHighestBelief(Node* const node)
{
  // Find the label with the highest belief
  float bestBelief = itk::NumericTraits<float>::min();
  int bestLabel = 0;

  for(unsigned int l = 0; l < this->LabelSet.size(); l++)
    {
    float belief = Belief(node, l);

    if(belief > bestBelief)
      {
      bestBelief = belief;
      bestLabel = l;
      }
    }

  return bestLabel;
}


template <typename T>
int LoopyBP<T>::FindLabelWithLowestBelief(Node* const node)
{
  // Find the label with the lowest belief

  float bestBelief = itk::NumericTraits<float>::max();
  int bestLabel = 0;

  for(unsigned int l = 0; l < this->LabelSet.size(); l++)
    {
    float belief = Belief(node, l);

    if(belief < bestBelief)
      {
      bestBelief = belief;
      bestLabel = l;
      }
    }

  return bestLabel;
}


template <typename T>
Message& LoopyBP<T>::GetMessage(const Node* fromNode, const Node* toNode, const int label)
{
  std::vector<MessageVector>& messageVectors = this->OutgoingMessageImage->GetPixel(fromNode);
  for(unsigned int v = 0; v < messageVectors.size(); v++)
    {
    if(messageVectors[v].DestinationNode == toNode)
      {
      for(unsigned int m = 0; m < messageVectors[v].GetNumberOfMessages(); m++)
        {
        if(messageVectors[v].GetMessage(m).Label == label)
          {
          return messageVectors[v].GetMessage(m);
          }
        }
      }
    }
  std::cerr << "No message exists with FromNode = " << fromNode << " ToNode = " << toNode << " Label = " << label << "(" << std::endl;
  exit(-1);
}

template <typename T>
MessageVector& LoopyBP<T>::GetMessages(Node* const fromNode, Node* const toNode)
{
  //std::cout << "Enter GetMessages()" << std::endl;

  if(fromNode == NULL || toNode == NULL)
    {
    std::cerr << "Cannot get messages to or from NULL nodes! From: " << fromNode << " to: " << toNode << std::endl;
    exit(-1);
    }

  unsigned int numberOfMessageVectors = fromNode->GetNumberOfNeighbors();

  for(unsigned int i = 0; i < numberOfMessageVectors; i++)
    {
    if(fromNode->GetOutgoingMessageVector(i).DestinationNode == toNode)
      {
      return fromNode->GetOutgoingMessageVector(i);
      }
    }
  std::cerr << "No MessageVector from " << fromNode << " to " << toNode << " exists!" << std::endl;
  exit(-1);
}


template <typename T>
std::vector<Message*> LoopyBP<T>::GetIncomingMessagesWithLabel(Node* const node, const int label)
{
  //std::cout << "Enter GetIncomingMessagesWithLabel()" << std::endl;

  if(node == NULL)
    {
    std::cerr << "Cannot get messages for NULL node!" << std::endl;
    exit(-1);
    }

  std::vector<Message*> incomingMessages;
  std::vector<Node* > neighbors = Helpers::Get4Neighbors<NodeImageType>(this->NodeImage, node->GetGridIndex());
  for(unsigned int i = 0; i < neighbors.size(); i++)
    {
    if(neighbors[i] == NULL) // can't get incoming messages from a non-existent neighbor!
      {
      continue;
      }
    MessageVector& messageVector = GetMessages(neighbors[i], node); // Get incoming messages (call is GetMessage(from, to) )
    //std::cout << messageVector << std::endl;
    if(messageVector.DestinationNode == NULL)
      {
      continue;
      }
    for(unsigned int m = 0; m < messageVector.GetNumberOfMessages(); m++)
      {
      if(messageVector.GetMessage(m).Label == label)
        {
        incomingMessages.push_back(&(messageVector.GetMessage(m)));
        }
      }
    }
  return incomingMessages;
}
