#pragma once

#include <vector>
#include <string>

constexpr unsigned MAX_EVENTS = 128;

struct Event
{
    enum Type { FILE_DROP };
    Type type;
};

struct FileDrop : public Event
{
    std::string path;
    FileDrop(std::string path) : path(std::move(path)) { type = Event::FILE_DROP; }
};

struct EventQueue
{
    unsigned used = 0;
    Event** events = new Event*[MAX_EVENTS];
};


std::vector<Event*> GetAll(const EventQueue& queue);
void AddEvent(EventQueue& queue, Event* event);
void Clear(EventQueue& queue);