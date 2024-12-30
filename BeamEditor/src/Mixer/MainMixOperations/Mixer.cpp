#include "Mixer.h"

Mixer::Mixer(vector<AudioFormat> formats, vector<vector<double>> channels)
    : formats(formats), numChannels(formats.size()), channels(channels),
      amplification(numChannels, 1.0), panning(numChannels, 0.5), isRunning(true),
      currentChunkId(0), lastProcessTime(chrono::steady_clock::now()) {
    // Create CircuitWorkers for each channel
    for (size_t i = 0; i < circuits.size(); ++i) {
        workers.push_back(make_unique<CircuitWorker>(circuits[i], i)); 
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
    for (size_t i = 0; i < circuits.size(); ++i) {
        workers.push_back(make_unique<CircuitWorker>(circuits[i], i)); 
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
}

void Mixer::processAudio() {
    vector<vector<vector<double>>> chunks = utils.makeChunks(channels);

    while (isRunning) {
        // Distribute chunks to workers with their IDs
        for (size_t i = 0; i < workers.size(); ++i) {
            pendingResults[currentChunkId] = workers[i]->addChunk(chunks[i][currentChunkId], currentChunkId); 
        }

        // Retrieve processed chunks (asynchronously)
        vector<vector<double>> processedChunks(workers.size());
        bool allChunksReady = false;
        while (!allChunksReady) {
            allChunksReady = true;
            for (size_t i = 0; i < workers.size(); ++i) {
                if (pendingResults[currentChunkId].valid()) { 
                    try {
                        processedChunks[i] = pendingResults[currentChunkId].get(); 
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
            // Calculate time elapsed since last processing
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::microseconds>(now - lastProcessTime);

            // Adjust sleep duration based on target sample rate
            auto targetSleepTime = chrono::microseconds(1000000 / targetSampleRate); // Calculate target sleep time in microseconds
            if (elapsed < targetSleepTime) { 
                this_thread::sleep_for(targetSleepTime - elapsed); 
            }
            lastProcessTime = now; 
        }

        // Remove completed futures from the map
        pendingResults.erase(currentChunkId); 

        // Mix processed chunks (implement mixing logic here)
        vector<double> mixedOutput = utils.mixChannels(processedChunks, amplification, panning);

        // Output the mixedOutput (implement output logic here)
        // ... (e.g., write to audio device)

        // Update currentChunkId and notify waiting threads
        {
            unique_lock<mutex> lock(mixerMutex);
            currentChunkId = (currentChunkId + 1) % chunks[0].size(); 
        }
        chunkReadyCondVar.notify_all(); 
    }
}
