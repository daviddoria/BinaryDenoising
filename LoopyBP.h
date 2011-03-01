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

#ifndef LoopyBP_H
#define LoopyBP_H

#include "Types.h"
#include "Message.h"
#include "UpdateSchedule.h"

// Some useful references:
// http://www.stanford.edu/~montanar/TEACHING/Stat375/handouts/bp_book2.pdf
// http://people.cs.uchicago.edu/~pff/papers/bp-long.pdf (below eq 9 - good confirmation that phi and psi are exp(-cost) )

/*
 * This class implements the core LBP algorithm. The message update equation and belief read out equations are both implemented here.
 * This class also stores the MRF.
 */

template <typename TImage>
class LoopyBP
{
public:

  // Constructor
  LoopyBP();

  // Perform one iteration of the propagation
  bool Iterate();

  // Get the output image after performing the propagation and belief read out.
  IntImageType::Pointer GetResult();

  // Create a label set of {0,1}
  void CreateBinaryLabelSet();

  // Specify a custom label set.
  void SetLabelSet(std::vector<int>);

  // Specify the number of update passes to perform over the entire image.
  void SetNumberOfPasses(unsigned int passes);

  // Determine if the algorithm has converged or reached a stopping point.
  bool IsFinished();

  // Create the MRF and messages and initialize them.
  void CreateAndInitializeMessages(const float defaultMessageValue);

  // Initialize the schedule.
  void Initialize();

  //void SetScheduleToRandom();
  void SetScheduleToRandomUnique();
  //void SetScheduleToRasterOneNeighbor();

  // Determine which update rule to use
  enum UpdateTypeEnum { SUMPRODUCT, MAXPRODUCT, MINSUM, MAXSUM};
  UpdateTypeEnum UpdateType;
  void SetUpdateToSumProduct();
  void SetUpdateToMaxProduct();
  void SetUpdateToMinSum();
  void SetUpdateToMaxSum();

  // The update rule choices
  bool SumProductMessageUpdate(MessageVector& messageVector);
  bool MaxProductMessageUpdate(MessageVector& messageVector);
  bool MinSumMessageUpdate(MessageVector& messageVector);
  bool MaxSumMessageUpdate(MessageVector& messageVector);


protected:

  // The Schedule object determines the order that the messages are updated.
  UpdateSchedule* Schedule;

  // The cost functions. They must be implemented by a subclass.
  virtual float UnaryCost(const int label, const itk::Index<2> pixel) = 0;
  virtual float BinaryCost(const int label1, const int label2) = 0;

  // The belief readout equations. Belief() calls either LogBelief() or StandardBelief() based on which message update rule is being used.
  float Belief(Node* const node, const int label);
  float LogBelief(Node* const node, const int label);
  float StandardBelief(Node* const node, const int label);

  // Compute the min or max belief label.
  int FindLabelWithLowestBelief(Node* const node);
  int FindLabelWithHighestBelief(Node* const node);

  // This is the output image.
  IntImageType::Pointer Image;

  // This is the MRF.
  NodeImageType::Pointer NodeImage;

  // The set of labels to use in the labeling.
  std::vector<int> LabelSet;

  // Get a specific message between two nodes.
  Message& GetMessage(const Node* fromNode, const Node* toNode, const int label);

  // Get the vector of message between two nodes.
  MessageVector& GetMessages(Node* const fromNode, Node* const toNode);

  // Sum the values of a vector of messages.
  float SumOfMessages(const std::vector<Message*> messages);

  // Get all incoming messages to 'node' with the specified 'label'.
  std::vector<Message*> GetIncomingMessagesWithLabel(Node* const node, const int label);




};

#include "LoopyBP.txx"

#endif
