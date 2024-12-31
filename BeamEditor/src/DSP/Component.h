
// Component.h

#ifndef COMPONENT_H
#define COMPONENT_H



using namespace std;

class Component {
public:
    Component();
    virtual vector<double> processChunk(const vector<double>& inChunk);
};

#endif // COMPONENT_H
