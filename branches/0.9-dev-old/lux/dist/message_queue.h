#ifndef LUX_DIST_MESSAGEQUEUE_H
#define LUX_DIST_MESSAGEQUEUE_H

#include <deque>

namespace Lux {
namespace Dist {

  template <class Message>
  class MessageQueue {
  public:
    MessageQueue(void)
    {}

    ~MessageQueue()
    {}

    bool enqueue_message(Message &message)
    {
      queue_.push_back(message);
    }

    Message dequeue_message(void)
    {
      Message m = queue_[0];
      queue_.pop_front();
      return m;
    }

    bool is_queue_empty(void)
    {
      return queue_.empty();
    }

  private:
    std::deque<Message> queue_;
  };
}
}

#endif
