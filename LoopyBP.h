#ifndef MRF_H
#define MRF_H

#include "Types.h"
#include "Message.h"
#include "UpdateSchedule.h"

// http://www.stanford.edu/~montanar/TEACHING/Stat375/handouts/bp_book2.pdf
// http://people.cs.uchicago.edu/~pff/papers/bp-long.pdf (below eq 9 - good confirmation that phi and psi are exp(-cost) )

template <typename TImage>
class LoopyBP
{
public:

  LoopyBP();

  bool Iterate();

  IntImageType::Pointer GetResult();

  void CreateBinaryLabelSet(); // Create a label set of {0,1}

  void SetLabelSet(std::vector<int>);

  void SetNumberOfPasses(unsigned int passes);
  bool IsFinished();
  
  void CreateAndInitializeMessages(const float defaultMessageValue);

  void Initialize();

  void OutputMessagesBetweenNodes(const itk::Index<2> FromNode, const itk::Index<2> ToNode);

  // These should be protected, but are public for testing
  UpdateSchedule* Schedule;
  //void SetScheduleToRandom();
  void SetScheduleToRandomUnique();
  //void SetScheduleToRasterOneNeighbor();

  enum UpdateTypeEnum { SUMPRODUCT, MAXPRODUCT, MINSUM, MAXSUM};
  UpdateTypeEnum UpdateType;
  void SetUpdateToSumProduct();
  void SetUpdateToMaxProduct();
  void SetUpdateToMinSum();
  void SetUpdateToMaxSum();

  bool SumProductMessageUpdate(MessageVector& messageVector);
  bool MaxProductMessageUpdate(MessageVector& messageVector);
  bool MinSumMessageUpdate(MessageVector& messageVector); // since this is the log domain, must initialize messages to 0 instead of 1
  bool MaxSumMessageUpdate(MessageVector& messageVector);

  void OutputBeliefImage();
  void OutputMessageImage();
  void WriteBeliefImage(std::string filename);

  IntImageType::Pointer GetImage();

protected:

  virtual float UnaryCost(const int label, const itk::Index<2> pixel) = 0;
  virtual float BinaryCost(const int label1, const int label2) = 0;

  float Belief(Node* const node, const int label);
  float LogBelief(Node* const node, const int label);
  float StandardBelief(Node* const node, const int label);
  
  int FindLabelWithLowestBelief(Node* const node);
  int FindLabelWithHighestBelief(Node* const node);

  IntImageType::Pointer Image;

  NodeImageType::Pointer NodeImage;

  std::vector<int> LabelSet;

  // Get a specific message
  Message& GetMessage(const Node* fromNode, const Node* toNode, const int label);
  
  MessageVector& GetMessages(Node* const fromNode, Node* const toNode);
  
  float SumOfMessages(const std::vector<Message*> messages);

  std::vector<Message*> GetIncomingMessagesWithLabel(Node* const node, const int label);



};

#include "LoopyBP.txx"

#endif
