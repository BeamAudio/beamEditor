#include<array>
#include<vector>
#include<Component.h>

using namespace std;

//The circuit class is responsible for taking the audio input from the mixer and processing the output 

//The mixer controls the circuits inside it, so it will be the only element writing to the components


class Circuit {
    private: 

        array<double,100> inChunk;
        array<double,100> outChunk;


        vector<Component> components;

    public:
        Circuit() {
            this->inChunk.fill(0.0);
            this->outChunk.fill(0.0);
        }

        Circuit(const vector<Component> components) {
            this->components = components;
            this->inChunk.fill(0.0);
            this->outChunk.fill(0.0);
        }

        void setInChunk(array<double, 100> inChunk) {
            this->inChunk = inChunk;
        }

        array<double, 100> getOutChunk() {
            return this->outChunk;
        }

        array<double, 100> processChunk() {

            array<double, 100> intermediateChunk;
            intermediateChunk = inChunk;
                for(Component c : components) {
                    intermediateChunk = c.processChunk(intermediateChunk);
                }
            return intermediateChunk;

        }

        
        
};
