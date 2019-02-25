#pragma once

#include <vector>
#include <string>

extern const unsigned MAX_EVENTS;

struct Event
{
    enum Type { FILE_DROP, RESIZE };
    Type type;
};

struct FileDrop : public Event
{
    std::string path;
    FileDrop(std::string path) : path(std::move(path)) { type = Event::FILE_DROP; }
};

struct Resize : public Event
{
    unsigned width, height;
    Resize(unsigned width, unsigned height) : width(width), height(height) { type = Event::RESIZE; }
};

struct EventQueue
{
    unsigned used = 0;
    Event** events = new Event*[MAX_EVENTS];
};


std::vector<Event*> GetAll(const EventQueue& queue);
void AddEvent(EventQueue& queue, Event* event);
void Clear(EventQueue& queue);