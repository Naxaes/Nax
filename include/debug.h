#pragma once

#define Assert(statement, ...) AssertImplementation(statement, #statement, __FILE__, __LINE__, __VA_ARGS__)

void AssertImplementation(
    bool status,
    const char* statement,
    const char* file,
    unsigned line,
    const char* message = "",
    ...
);


