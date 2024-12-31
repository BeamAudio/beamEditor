
#ifndef MIXER_H
#define MIXER_H

#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <unordered_map>
#include <chrono>

#include "CircuitWorker.h"
#include "AudioFormat.h"
#include "AudioUtils.h"

class Mixer {
public:
    Mixer(vector<AudioFormat> formats, vector<vector<vector<double>>> channels);
    ~Mixer();

    void setCircuits(const vector<Circuit>& circuits);
    void setAmplification(const vector<double>& amplification);
    void setPanning(const vector<double>& panning);

    void start();
    void stop();

private:
    vector<AudioFormat> formats;
    int numChannels;
    vector<Circuit> circuits;
    vector<vector<vector<double>>> channels;
    vector<double> amplification;
    vector<double> panning;
    AudioUtils utils;
    atomic<bool> isRunning;
    vector<unique_ptr<CircuitWorker>> workers; 
    size_t currentChunkId;
    mutex mixerMutex;
    condition_variable chunkReadyCondVar; 
    unordered_map<size_t, future<vector<double>>> pendingResults; 
    chrono::steady_clock::time_point lastProcessTime; 
    const double targetSampleRate = 44100.0; // Sample rate in Hz

    void processAudio(); 
};

#endif // MIXER_H

