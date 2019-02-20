#include "event.h"

#include <string>
#include <vector>
#include <deque>

#include "debug.h"


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
    Assert(queue.used == 0, "Queue overflown! Has %i events of a maximum %i.", queue.used, MAX_EVENTS);
    queue.events[queue.used++] = event;
}

void Clear(EventQueue& queue)
{
    for (unsigned i = 0; i < queue.used; ++i)
        delete queue.events[i];
    queue.used = 0;
}



