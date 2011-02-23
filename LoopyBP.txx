#include "LoopyBP.h"

#include "Helpers.h"

#include "itkImageRegionIterator.h"

#include <vector>

template <typename T>
LoopyBP<T>::LoopyBP()
{
  this->Image = NULL;
  this->OutgoingMessageImage = NULL;
}

template <typename T>
void LoopyBP<T>::CreateBinaryLabelSet()
{
  this->LabelSet.clear();
  this->LabelSet.push_back(0);
  this->LabelSet.push_back(1);
}

template <typename T>
void LoopyBP<T>::CreateAndInitializeMessages(const float defaultMessageValue)
{
  // For each pixel, create |L| messages to each neighboring pixel
  if(!this->OutgoingMessageImage)
    {
    this->OutgoingMessageImage = MessageImageType::New();
    }
  this->OutgoingMessageImage->SetRegions(this->Image->GetLargestPossibleRegion());
  this->OutgoingMessageImage->Allocate();

  itk::ImageRegionIterator<MessageImageType> imageIterator(this->OutgoingMessageImage, this->OutgoingMessageImage->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    std::vector<itk::Index<2> > neighbors = Helpers::GetNeighbors<MessageImageType>(this->OutgoingMessageImage, imageIterator.GetIndex());
    std::vector<MessageVector> messageVectors;

    for(unsigned int i = 0; i < neighbors.size(); i++) // add a message to each neighbor
      {
      MessageVector messageVector;
      messageVector.FromNode = imageIterator.GetIndex();
      messageVector.ToNode = neighbors[i];

      //std::cout << "Created a message from " << messageVector.FromNode << " to " << messageVector.ToNode << std::endl;

      if(messageVector.FromNode == messageVector.ToNode)
        {
        std::cerr << "Cannot create a message from " << messageVector.FromNode << " to " << messageVector.ToNode << std::endl;
        exit(-1);
        }
      for(unsigned int l = 0; l < this->LabelSet.size(); l++) // add a message for each label
        {
        Message m;
        m.Label = l;
        m.Value = defaultMessageValue;
        messageVector.AddMessage(m);
        }
      messageVectors.push_back(messageVector);
      }
    imageIterator.Set(messageVectors);

    ++imageIterator;
    }

  this->Schedule.SetOutgoingMessageImage(this->OutgoingMessageImage);
  this->Schedule.Initialize();
}

template <typename T>
bool LoopyBP<T>::Iterate()
{
  // Determine which message to update (following the schedule)
  MessageVector& messageVector = this->Schedule.NextMessage(); // The message is returned by reference

  SumProductMessageUpdate(messageVector);

  return false; // Not yet converged
}

template <typename T>
bool LoopyBP<T>::SumProductMessageUpdate(MessageVector& messageVector)
{
  // This function returns true if nothing changed (converged) (Convergence checking is not yet implemented, so it always returns false at the moment)

  // Message update equation
  // m_{ij}(l) = \sum_{p \in L} \left[ B(L(p), L(l)) U(L(p)) \prod_{k=N(i)\backslash j} \left( m_{ki}(L(p)) \right) \right]

  for(unsigned int l = 0; l < messageVector.GetNumberOfMessages(); l++)
    {
    float sum = 0.0;
    for(unsigned int p = 0; p < this->LabelSet.size(); p++)
      {
      float unary = this->UnaryCost(this->LabelSet[p], messageVector.FromNode);
      float binary = this->BinaryCost(this->LabelSet[p], messageVector.GetMessage(l).Label);

      float product = 1.0;
      //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
      std::vector<Message*> messages = GetIncomingMessagesWithLabel(messageVector.FromNode, this->LabelSet[p]);
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
bool LoopyBP<T>::MaxProductMessageUpdate(MessageVector& m)
{
#if 0
  // This function returns true if nothing changed (converged) (Convergence checking is not yet implemented, so it always returns false at the moment)

  float max = 0.0;

  for(unsigned int p = 0; p < this->LabelSet.size(); p++)
    {
    float unary = this->UnaryCost(p, m.FromNode);
    float binary = this->BinaryCost(p, m.Label);

    float product = 1.0;
    //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
    std::vector<Message*> messages = GetMessagesWithLabel(m.FromNode, p);
    for(unsigned int k = 0; k < messages.size(); k++)
      {
      product *= messages[k]->Value;
      }
    float current = exp(-unary) * exp(-binary) * product;
    if(current > max)
      {
      max = current;
      }
    }

  //std::cout << "Original message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  m.Value = max;

  //std::cout << "New message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;

  NormalizeMessages(m.FromNode, m.ToNode);

  //std::cout << "Normalized message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  //std::cout << std::endl;

#endif
  return false; // Not yet converged
}


template <typename T>
bool LoopyBP<T>::MinSumMessageUpdate(MessageVector& m)
{
#if 0
  // This function returns true if nothing changed (converged) (Convergence checking is not yet implemented, so it always returns false at the moment)

  float min = itk::NumericTraits<float>::max();

  for(unsigned int p = 0; p < this->LabelSet.size(); p++)
    {
    float unary = this->UnaryCost(p, m.FromNode);
    float binary = this->BinaryCost(p, m.Label);

    float sum = 0.0;
    //std::cout << "Getting messages with label (" << m.FromNode << " , " << p << ")" << std::endl;
    std::vector<Message*> messages = GetMessagesWithLabel(m.FromNode, p);
    for(unsigned int k = 0; k < messages.size(); k++)
      {
      sum += messages[k]->Value;
      }
    float current = unary + binary + sum;
    if(current < min)
      {
      min = current;
      }
    }

  //std::cout << "Original message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  m.Value = min;

  //std::cout << "New message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;

  NormalizeMessages(m.FromNode, m.ToNode);

  //std::cout << "Normalized message value: " << GetMessage(m.FromNode, m.ToNode,m .Label).Value << std::endl;
  //std::cout << std::endl;
#endif
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
float LoopyBP<T>::Belief(const itk::Index<2> node, const int label)
{
  float unary = UnaryCost(label, node);
  //std::cout << "UnaryCost: " << unary << std::endl;

  float product = 1.0;

  std::vector<Message*> incomingMessages = GetIncomingMessagesWithLabel(node, label);

  for(unsigned int i = 0; i < incomingMessages.size(); i++)
    {
    product *= incomingMessages[i]->Value;
    }

  //std::cout << "product: " << product << std::endl;

  float belief = unary * product;
  return belief;
}

template <typename T>
IntImageType::Pointer LoopyBP<T>::GetResult()
{
  itk::ImageRegionIterator<IntImageType> imageIterator(this->Image, this->Image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    int label = FindLabelWithHighestBelief(imageIterator.GetIndex());
    imageIterator.Set(label);
    ++imageIterator;
    }

  return this->Image;
}

template <typename T>
int LoopyBP<T>::FindLabelWithHighestBelief(const itk::Index<2> node)
{
  float bestBelief = 0;
  int bestLabel = 0;

  for(unsigned int l = 0; l < this->LabelSet.size(); l++)
    {
    float belief = Belief(node, l);
    //std::cout << "Pixel: " << node << " Label: " << l << " belief: " << belief << std::endl;
    if(belief > bestBelief)
      {
      bestBelief = belief;
      bestLabel = l;
      }
    }
  //std::cout << "Best label is " << bestLabel << " with belief " << bestBelief << std::endl;

  return bestLabel;
}


template <typename T>
int LoopyBP<T>::FindLabelWithLowestBelief(const itk::Index<2> node)
{
  // Find the label with the lowest belief

  float bestBelief = itk::NumericTraits<float>::max();
  int bestLabel = 0;

  for(unsigned int l = 0; l < this->LabelSet.size(); l++)
    {
    float belief = Belief(node, l);
    //std::cout << "Pixel: " << node << " Label: " << l << " belief: " << belief << std::endl;
    if(belief < bestBelief)
      {
      bestBelief = belief;
      bestLabel = l;
      }
    }
  //std::cout << "Best label is " << bestLabel << " with belief " << bestBelief << std::endl;

  return bestLabel;
}

template <typename T>
Message& LoopyBP<T>::GetMessage(const itk::Index<2> fromNode, const itk::Index<2> toNode, const int label)
{
  std::vector<MessageVector>& messageVectors = this->OutgoingMessageImage->GetPixel(fromNode);
  for(unsigned int v = 0; v < messageVectors.size(); v++)
    {
    if(messageVectors[v].ToNode == toNode)
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
std::vector<Message*> LoopyBP<T>::GetMessages(const itk::Index<2> node)
{
  std::vector<Message>& outgoingMessages = this->OutgoingMessageImage->GetPixel(node);
  std::vector<Message*> messages;

  for(unsigned int i = 0; i < outgoingMessages.size(); i++)
    {
    messages.push_back(&(outgoingMessages[i]));
    }
  return messages;
}

template <typename T>
MessageVector& LoopyBP<T>::GetMessages(const itk::Index<2> fromNode, const itk::Index<2> toNode)
{
  unsigned int numberOfMessageVectors = this->OutgoingMessageImage->GetPixel(fromNode).size();

  for(unsigned int i = 0; i < numberOfMessageVectors; i++)
    {
    if(this->OutgoingMessageImage->GetPixel(fromNode)[i].ToNode == toNode)
      {
      return this->OutgoingMessageImage->GetPixel(fromNode)[i];
      }
    }
  std::cerr << "No MessageVector from " << fromNode << " to " << toNode << " exists!" << std::endl;
  exit(-1);
}


template <typename T>
std::vector<Message*> LoopyBP<T>::GetIncomingMessagesWithLabel(const itk::Index<2> node, const int label)
{
  std::vector<Message*> incomingMessages;
  std::vector<itk::Index<2> > neighbors = Helpers::GetNeighbors<MessageImageType>(this->OutgoingMessageImage, node);
  for(unsigned int i = 0; i < neighbors.size(); i++)
    {
    MessageVector& messageVector = GetMessages(neighbors[i], node); // Get incoming messages
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

template <typename T>
void LoopyBP<T>::OutputMessagesBetweenNodes(const itk::Index<2> fromNode, const itk::Index<2> toNode)
{
  MessageVector& messageVector = GetMessages(fromNode, toNode);
  std::cout << "Messages from " << fromNode << " to " << toNode << std::endl << messageVector << std::endl;
}