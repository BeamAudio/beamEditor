
// Component.h

#ifndef COMPONENT_H
#define COMPONENT_H

#include <array>

using namespace std;

class Component {
public:
    Component();
    virtual array<double, 100> processChunk(const array<double, 100>& inChunk);
};

#endif // COMPONENT_H
