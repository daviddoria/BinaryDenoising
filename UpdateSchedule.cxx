#include "UpdateSchedule.h"

UpdateSchedule::UpdateSchedule()
{
  this->NodeImage = NULL;
}

void UpdateSchedule::SetNodeImage(NodeImageType::Pointer image)
{
  this->NodeImage = image; // This must not be copied, but rather the pointer assigned. That is because UpdateSchedule subclasses return pixels of the OutgoingMessageImage by reference, and we want these to be identical to the pixels in the OutgoingMessageImage of the LoopyBP subclass's OutgoingMessageImage
}