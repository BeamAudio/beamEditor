// Circuit.h

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <array>
#include <vector>
#include "Component.h"

using namespace std;

// The circuit class is responsible for taking the audio input from the mixer and processing the output

// The mixer controls the circuits inside it, so it will be the only element writing to the components

class Circuit {
private:
    vector<Component> components;

public:
    Circuit();
    Circuit(const vector<Component> components);
    array<double, 100> processChunk(const array<double, 100>& inChunk);
};

#endif // CIRCUIT_H
