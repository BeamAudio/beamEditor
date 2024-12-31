// Circuit.cpp

#include "Circuit.h"

Circuit::Circuit() {
    
}

Circuit::Circuit(const vector<Component> components) {
    this->components = components;
    
}

vector<double> Circuit::processChunk(const vector<double>& inChunk) {
    vector<double> intermediateChunk = inChunk;
    for (Component c : components) {
        intermediateChunk = c.processChunk(intermediateChunk);
    }
    return intermediateChunk;
}
