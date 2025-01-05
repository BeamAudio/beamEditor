// Mixer.cpp

#include "Mixer.h"

Mixer::Mixer(vector<AudioFormat> formats, vector<vector<vector<double>>>& channels) 
    : formats(formats), numChannels(formats.size()), channels(channels), 
      amplification(numChannels, 1.0), panning(numChannels, 0.5), 
      isRunning(true), currentChunkId(0), 
      targetSampleRate(44100.0), 
      maxProcessingLatency(static_cast<chrono::microseconds>(1000000 / targetSampleRate * 0.75)) { 

    chunks = utils.makeChunks(channels); 

    // Create CircuitWorkers for each channel and subchannel
    workers.resize(numChannels);
    for (size_t i = 0; i < numChannels; ++i) {
        workers[i].resize(formats[i].numChannels); 
        for (size_t j = 0; j < formats[i].numChannels; ++j) { 
            workers[i][j] = make_unique<CircuitWorker>(circuits[i], i, j); 
        }
    }
}

Mixer::~Mixer() {
    // Stop and join worker threads in the destructor
    stop(); 
}

void Mixer::setCircuits(const vector<Circuit>& circuits) {
    this->circuits = circuits;
    // Recreate CircuitWorkers with the updated circuits
    workers.clear();
    for (size_t i = 0; i < numChannels; ++i) {
        workers.resize(formats[i].numChannels); 
        for (size_t j = 0; j < formats[i].numChannels; ++j) { 
            workers[i][j] = make_unique<CircuitWorker>(circuits[i], i, j); 
        }
    }
}

void Mixer::setAmplification(const vector<double>& amplification) {
    this->amplification = amplification;
}

void Mixer::setPanning(const vector<double>& panning) {
    this->panning = panning;
}

void Mixer::start() {
    isRunning = true;
    thread mixerThread(&Mixer::processAudio, this); 
    mixerThread.detach(); // Allow the mixer thread to run independently
}

void Mixer::stop() {
    isRunning = false;
    chunkReadyCondVar.notify_all(); 
    for (size_t i = 0; i < workers.size(); ++i) {
        for (size_t j = 0; j < workers[i].size(); ++j) {
            workers[i][j]->isRunning = false; 
            workers[i][j]->condVar_.notify_one(); 
        }
    }
}

void Mixer::setChannels(vector<vector<vector<double>>>& channels) {
    this->channels = channels;
}

void Mixer::processAudio() {
    while (isRunning) {
        // Distribute next chunk to workers
        for (size_t i = 0; i < numChannels; ++i) {
            for (size_t j = 0; j < formats[i].numChannels; ++j) { 
                pendingResults[nextChunkId][i][j] = workers[i][j]->addChunk(chunks[i][j][nextChunkId], nextChunkId); 
            }
        }

        // Retrieve processed chunks (asynchronously with timeout)
        vector<vector<vector<double>>> processedChunks(numChannels); 
        bool allChunksReady = false;
        auto startTime = chrono::steady_clock::now(); 
        while (!allChunksReady && (chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - startTime) < maxProcessingLatency)) { 
            allChunksReady = true;
            for (size_t i = 0; i < numChannels; ++i) {
                processedChunks[i].resize(formats[i].numChannels); 
                for (size_t j = 0; j < formats[i].numChannels; ++j) {
                    if (pendingResults[currentChunkId][i][j].wait_for(chrono::milliseconds(1)) == future_status::ready) { 
                        try {
                            processedChunks[i][j] = pendingResults[currentChunkId][i][j].get(); 
                        } catch (const exception& e) {
                            // Handle exceptions (e.g., log error, stop processing)
                            cerr << "Error processing chunk: " << e.what() << endl;
                            isRunning = false; 
                            return;
                        }
                    } else {
                        allChunksReady = false; 
                        break;
                    }
                }
                if (!allChunksReady) { 
                    break; 
                }
            }
        }

        // Check if all chunks were processed within the time limit
        if (!allChunksReady) {
            cerr << "Timeout: Failed to process all chunks within the allotted time." << endl;
            // Consider handling this case (e.g., dropping the current chunk)
            // ...
        }

        // Remove completed futures from the map
        pendingResults.erase(currentChunkId); 

        // Mix processed chunks (implement mixing logic here)
        vector<double> mixedOutput = utils.mixChannelsChunk(processedChunks, amplification, panning); 

        // Output the mixedOutput (implement output logic here)
        // ... (e.g., write to audio device)

        // Update currentChunkId and notify waiting threads
        {
            unique_lock<mutex> lock(mixerMutex);
            currentChunkId = nextChunkId; 
            nextChunkId = (nextChunkId + 1) % chunks[0][0].size(); 
        }
        chunkReadyCondVar.notify_all(); 

        // Immediately start processing the next chunk
        for (size_t i = 0; i < numChannels; ++i) {
            for (size_t j = 0; j < formats[i].numChannels; ++j) { 
                pendingResults[nextChunkId][i][j] = workers[i][j]->addChunk(chunks[i][j][nextChunkId], nextChunkId); 
            }
        }
    }
}


