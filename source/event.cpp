#include "event.h"

#include <string>
#include <vector>
#include <deque>
#include <cassert>


std::vector<Event*> GetAll(const EventQueue& queue)
{
    std::vector<Event*> events;
    events.reserve(queue.used);
    for (unsigned i = 0; i < queue.used; ++i)
        events.push_back(queue.events[i]);
    return events;
}

void AddEvent(EventQueue& queue, Event* event)
{
    assert(queue.used < MAX_EVENTS);
    queue.events[queue.used++] = event;
}

void Clear(EventQueue& queue)
{
    for (unsigned i = 0; i < queue.used; ++i)
        delete queue.events[i];
    queue.used = 0;
}



