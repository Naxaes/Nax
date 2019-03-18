#pragma once

#include <vector>
#include <string>

extern const unsigned MAX_EVENTS;

struct Event
{
    enum Type { FILE_DROP, RESIZE, MOUSE_CLICK, MOUSE_MOVEMENT };
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

struct MouseClick : public Event
{
    enum Button { RIGHT, LEFT, MIDDLE };
    enum Event  { CLICKED, RELEASED };

    Button button;
    Event  event;
    MouseClick(Button button, Event event) : button(button), event(event) { type = MOUSE_CLICK; }
};

struct MouseMovement : public Event
{
    float dx;
    float dy;
    MouseMovement(float dx, float dy) : dx(dx), dy(dy) { type = MOUSE_MOVEMENT; }
};

struct EventQueue
{
    unsigned used = 0;
    Event** events = new Event*[MAX_EVENTS];
};


std::vector<Event*> GetAll(const EventQueue& queue);
void AddEvent(EventQueue& queue, Event* event);
void Clear(EventQueue& queue);