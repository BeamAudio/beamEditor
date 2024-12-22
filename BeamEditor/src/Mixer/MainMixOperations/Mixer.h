#include<AudioFormat.h>
#include<vector>
#include<Circuit.h>
#include<AudioUtils.h>

using namespace std;

#include<thread>

class Mixer {
    private: 
        vector<AudioFormat> formats; //The format of the audio channels
        int numChannels;
        vector<Circuit> circuits; //The circuits for the data processing
        vector<vector<double>> channels; //The actual Channels that have the audio data
        vector<double> amplification; //Sets the amplification output to each circuit
        vector<double> panning; //Sets the panning to each circuit
        AudioUtils utils;

        

    public:
        Mixer() {

        }

        Mixer(vector<AudioFormat> formats, vector<vector<double>> channels) {
            this->numChannels = 0;

            for(AudioFormat f : formats) {
                numChannels++;
            }

            this->formats = formats;
            this->channels = channels;
            
        }

        void setCircuits(vector<Circuit> circuits) { 
            this->circuits = circuits;
        }

        //The mix method has to open n new threads, one for each double array in channels, and all of them run in parallel. When a chunk is finished, all the threads should join and the mixer has to retrieve their output buffer to play it back

        void mix() { 
            vector<vector<vector<double>>> chunks = utils.makeChunks(channels); //Separate the channel data into chunks for the Circuits
            for(Circuit c : circuits) {

            }




        }


        
};