
#include<array>
using namespace std;

class Component {
        
    public:
        Component() {}

        virtual array<double, 100> processChunk(array<double, 100> inChunk) {
            return inChunk;
        }

};
