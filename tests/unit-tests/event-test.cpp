#include "event.h"

#include <gtest/gtest.h>


// GetAll
// AddEvent
// Clear


TEST(AddEvent, Valid)
{
    EventQueue queue{};
    EXPECT_EQ(queue.used, 0);
    AddEvent(queue, new Event);
    EXPECT_EQ(queue.used, 1);
}
//
// // TODO(ted): The overflow is currently handled with an assert. Don't know how to handle it at the moment.
// TEST(AddEvent, Overflow)
// {
//     EventQueue queue{};
//     for (unsigned i = 0; i < MAX_EVENTS; ++i)
//         AddEvent(queue, new Event);
//     EXPECT_EQ(queue.used, 1);
// }


TEST(GetAll, Empty)
{
    EventQueue queue{};
    auto events = GetAll(queue);
    EXPECT_EQ(events.size(), 0);
}

TEST(GetAll, NotEmpty)
{
    EventQueue queue{};
    AddEvent(queue, new Event);
    AddEvent(queue, new Event);
    AddEvent(queue, new FileDrop("Hello"));
    AddEvent(queue, new Event);
    AddEvent(queue, new FileDrop("Test"));

    auto events = GetAll(queue);
    EXPECT_EQ(events.size(), 5);
    EXPECT_EQ(reinterpret_cast<FileDrop*>(events[2])->path, "Hello");
    EXPECT_EQ(reinterpret_cast<FileDrop*>(events[4])->path, "Test");
}


TEST(Clear, Works)
{
    EventQueue queue{};
    AddEvent(queue, new FileDrop("Hello"));
    AddEvent(queue, new Event);
    AddEvent(queue, new Event);
    AddEvent(queue, new Event);
    AddEvent(queue, new FileDrop("Test"));

    Clear(queue);
    EXPECT_EQ(queue.used, 0);
}