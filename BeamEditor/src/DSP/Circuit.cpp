// Circuit.cpp

#include "Circuit.h"

Circuit::Circuit() {
    this->inChunk.fill(0.0);
    this->outChunk.fill(0.0);
}

Circuit::Circuit(const vector<Component> components) {
    this->components = components;
    this->inChunk.fill(0.0);
    this->outChunk.fill(0.0);
}

array<double, 100> Circuit::processChunk(const array<double, 100>& inChunk) {
    array<double, 100> intermediateChunk = inChunk;
    for (Component c : components) {
        intermediateChunk = c.processChunk(intermediateChunk);
    }
    return intermediateChunk;
}
