#ifndef MRF_H
#define MRF_H

#include "Types.h"
#include "Message.h"
#include "UpdateSchedule.h"

template <typename TImage>
class LoopyBP
{
public:

  LoopyBP();

  bool Iterate();

  IntImageType::Pointer GetResult();

  void CreateBinaryLabelSet(); // Create a label set of {0,1}

  void SetLabelSet(std::vector<int>);

  void CreateAndInitializeMessages(const float defaultMessageValue);

  void Initialize();

  void OutputMessagesBetweenNodes(const itk::Index<2> FromNode, const itk::Index<2> ToNode);

  // These should be protected, but are public for testing
  UpdateSchedule* Schedule;
  void SetScheduleToRandom();
  void SetScheduleToRandomUnique();
  void SetScheduleToRasterOneNeighbor();

  enum UpdateTypeEnum { SUMPRODUCT, MAXPRODUCT, MINSUM};
  UpdateTypeEnum UpdateType;
  void SetUpdateToSumProduct();
  void SetUpdateToMaxProduct();
  void SetUpdateToMinSum();

  bool SumProductMessageUpdate(MessageVector& messageVector);
  bool MaxProductMessageUpdate(MessageVector& messageVector);
  bool MinSumMessageUpdate(MessageVector& messageVector); // since this is the log domain, must initialize messages to 0 instead of 1

  void OutputBeliefImage();
  void OutputMessageImage();
  void WriteBeliefImage(std::string filename);

  IntImageType::Pointer GetImage();

protected:

  virtual float UnaryCost(const int label, const itk::Index<2> pixel) = 0;
  virtual float BinaryCost(const int label1, const int label2) = 0;

  float Belief(const itk::Index<2> node, const int label);
  int FindLabelWithLowestBelief(const itk::Index<2> node);
  int FindLabelWithHighestBelief(const itk::Index<2> node);

  IntImageType::Pointer Image;

  // OutgoingMessageImage stores a vector of Messages at each pixel which describes all of the messages originating at each pixel
  MessageImageType::Pointer OutgoingMessageImage;

  std::vector<int> LabelSet;

  // Get a specific message
  Message& GetMessage(const itk::Index<2> fromNode, const itk::Index<2> toNode, const int label);
  std::vector<Message*> GetMessages(const itk::Index<2> node);
  MessageVector& GetMessages(const itk::Index<2> fromNode, const itk::Index<2> toNode);
  float SumOfMessages(const std::vector<Message*> messages);

  std::vector<Message*> GetIncomingMessagesWithLabel(const itk::Index<2> node, const int label);



};

#include "LoopyBP.txx"

#endif
