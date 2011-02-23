
template <typename T>
std::vector<Message*> LoopyBP<T>::GetMessagesWithLabel(itk::Index<2> fromNode, int label)
{
  std::vector<Message*> messagesWithLabel;
  std::vector<Message>& allMessages = this->OutgoingMessageImage->GetPixel(fromNode);
  for(unsigned int i = 0; i < allMessages.size(); i++)
    {
    if(allMessages[i].Label == label)
      {
      messagesWithLabel.push_back(&(allMessages[i]));
      }
    }

  if(messagesWithLabel.size() == 0)
    {
    std::cerr << "No message exists with FromNode = " << fromNode << " Label = " << label << std::endl;
    exit(-1);
    }

  return messagesWithLabel;
}