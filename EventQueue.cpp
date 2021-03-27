#include "EventQueue.h"

using namespace std;

void
EventQueue::queue_insert(shared_ptr<Event> &event_ptr)
{
  list<shared_ptr<Event>>::iterator i = ev_Queue.end();

  i--;
  while(i != ev_Queue.begin()) {
    if(event_ptr->eventTime > (*i)->eventTime)
    {
      i++;
      ev_Queue.insert(i, event_ptr);
      break;
    }
    i--;
  }

  if(i == ev_Queue.begin()) {
    if(event_ptr->eventTime > (*i)->eventTime)
    {
      i++;
      ev_Queue.insert(i, event_ptr);
    }
    else
      ev_Queue.push_front(event_ptr);
  }
}
