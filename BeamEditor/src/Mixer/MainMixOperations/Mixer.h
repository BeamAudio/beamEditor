#include <AudioFormat.h>
#include <vector>
#include <Circuit.h>
#include <AudioUtils.h>
#include <thread>
#include <future>
#include <atomic>

using namespace std;

class Mixer {
private:
    vector<AudioFormat> formats;
    int numChannels;
    vector<Circuit> circuits;
    vector<vector<double>> channels;
    vector<double> amplification;
    vector<double> panning; 
    AudioUtils utils;
    atomic<bool> isRunning; 

public:
    Mixer() : isRunning(true) {}

    Mixer(vector<AudioFormat> formats, vector<vector<double>> channels) 
        : formats(formats), numChannels(formats.size()), channels(channels), 
          amplification(numChannels, 1.0), panning(numChannels, 0.5), isRunning(true) {}

    void setCircuits(const vector<Circuit>& circuits) { 
        this->circuits = circuits;
    }

    void setAmplification(const vector<double>& amplification) {
        this->amplification = amplification;
    }

    void setPanning(const vector<double>& panning) {
        this->panning = panning;
    }

    void setIsRunning(const bool& isRunning) {
        this->isRunning = isRunning;
    }

    

    void start() {
        vector<CircuitWorker> workers; //We create the actual circuit workers from the circuit templates
        int channelSize;
        // Implement real-time processing logic here
        // 1. Continuously receive new audio data for each channel (We assume data is preloaded into the mixer before playing
        // We assume that the vector of circuits contains one circuit per channel of the mixer, independently of stereo or mono
        // If the channel is stereo, we need to adjust for it by running left and right channels through two parallel circuits
        for(size_t i = 0; i<channels.size(); i++) {
            //We are in channel i
            for(size_t j = 0; j<channels[i].size(); j++) {
                workers.push_back(CircuitWorker(circuits[i]));//We create copies in the form of workers for each channel
            }
        }

        vector<vector<vector<double>>> chunks = utils.makeChunks(channels);
        
        
            
        
        // 2. Call processAudio() with the new data
        // 3. Output the mixedOutput 
        // 4. Check isRunning flag to stop the processing loop
    }

    void stop() {
        isRunning = false; 
    }
};
