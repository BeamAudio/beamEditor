// CircuitWorker.cpp

#include "CircuitWorker.h"

CircuitWorker::CircuitWorker(const Circuit& circuit) 
    : circuit(circuit), isRunning(true) {
    workerThread = thread(&CircuitWorker::workerLoop, this); 
}

future<vector<double>> CircuitWorker::addChunk(const vector<double>& chunk, size_t chunkId) {
    unique_lock<mutex> lock(mutex_);
    workQueue.push({chunkId, chunk});
    condVar_.notify_one(); 

    promise<vector<double>> promise;
    future<vector<double>> futureResult = promise.get_future();

    // Store the promise in the pendingResults map
    pendingResults[chunkId] = move(promise); 

    return futureResult;
}

void CircuitWorker::workerLoop() {
    while (isRunning) {
        unique_lock<mutex> lock(mutex_);
        condVar_.wait(lock, [this] { return !workQueue.empty() || !isRunning; });

        if (isRunning && !workQueue.empty()) {
            pair<size_t, vector<double>> chunkPair = workQueue.front();
            size_t chunkId = chunkPair.first;
            vector<double> chunk = move(chunkPair.second);
            workQueue.pop();
            lock.unlock();

            vector<double> processedChunk = circuit.process(chunk); 

            // Get the corresponding promise from the map
            auto it = pendingResults.find(chunkId);
            if (it != pendingResults.end()) {
                it->second.set_value(processedChunk); 
                pendingResults.erase(it); // Remove the promise from the map
            }
        }
    }
}
