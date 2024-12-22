#include <iostream>

using namespace std;

class AudioFormat {
    private:
        int channels;
    
    public:

        AudioFormat() {

            this->channels = 0;


        }

        AudioFormat(int numChannels) {
            this->channels = numChannels;
        }
};
