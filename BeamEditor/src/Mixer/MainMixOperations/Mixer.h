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
    vector<vector<unique_ptr<CircuitWorker>>> workers; // 2D vector for workers (channel x subchannel)
    size_t currentChunkId;
    mutex mixerMutex;
    condition_variable chunkReadyCondVar; 
    unordered_map<size_t, vector<vector<future<vector<double>>>>> pendingResults; // Map of futures for each channel and subchannel
    chrono::steady_clock::time_point lastProcessTime; 
    const double targetSampleRate = 44100.0; // Sample rate in Hz

    void processAudio(); 
};

#endif // MIXER_H

// CircuitWorker.h

#ifndef CIRCUIT_WORKER_H
#define CIRCUIT_WORKER_H

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include "Circuit.h"

class CircuitWorker {
public:
    CircuitWorker(const Circuit& circuit, size_t channelIndex, size_t subchannelIndex);
    future<vector<double>> addChunk(const vector<double>& chunk, size_t chunkId);

private:
    const Circuit& circuit;
    size_t channelIndex;
    size_t subchannelIndex;
    thread workerThread;
    mutex mutex_;
    condition_variable condVar_;
    queue<pair<vector<double>, size_t>> workQueue; 
    bool isRunning;
    void workerLoop();
};

#endif // CIRCUIT_WORKER_H
