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

    vector<double> processAudio(const vector<vector<double>>& inputChunks) {
        vector<future<vector<double>>> results;
        vector<double> mixedOutput(inputChunks[0].size(), 0.0); 

        for (size_t i = 0; i < numChannels; ++i) {
            results.push_back(async(launch::async, [&, i]() { 
                vector<double> processedChunk = circuits[i].processChunk(inputChunks[i]);
                for (size_t j = 0; j < processedChunk.size(); ++j) {
                    processedChunk[j] *= amplification[i]; 
                }
                return processedChunk; 
            }));
        }

        for (size_t i = 0; i < numChannels; ++i) {
            vector<double> channelOutput = results[i].get(); 
            for (size_t j = 0; j < channelOutput.size(); ++j) {
                mixedOutput[j] += channelOutput[j]; 
            }
        }

        return mixedOutput;
    }

    void start() {
        // Implement real-time processing logic here
        // 1. Continuously receive new audio data for each channel
        // 2. Call processAudio() with the new data
        // 3. Output the mixedOutput 
        // 4. Check isRunning flag to stop the processing loop
    }

    void stop() {
        isRunning = false; 
    }
};
