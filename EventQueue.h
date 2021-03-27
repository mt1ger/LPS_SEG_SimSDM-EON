#ifndef _EVENTQUEUE_H
#define _EVENTQUEUE_H

#include "Event.h"
#include <list>
#include <memory>
#include <vector>

class EventQueue {
public:
  EventQueue() {};
  ~EventQueue() {};

  // list<Event *> ev_Queue;
  list<shared_ptr<Event>> ev_Queue;

  void
  queue_insert(shared_ptr<Event> &event_ptr);
};

#endif
