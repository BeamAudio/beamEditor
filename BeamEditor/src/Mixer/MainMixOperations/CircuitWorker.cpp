#include "CircuitWorker.h"

CircuitWorker::CircuitWorker(const Circuit& circuit, size_t workerId) 
    : circuit(circuit), workerId(workerId), stopFlag(false) {
    workerThread = thread(&CircuitWorker::processChunks, this); 
}

CircuitWorker::~CircuitWorker() {
    stop(); 
    if (workerThread.joinable()) {
        workerThread.join(); 
    }
}

void CircuitWorker::addChunk(const vector<double>& chunk, size_t chunkId) {
    unique_lock<mutex> lock(queueMutex); 
    chunks.push({chunkId, chunk}); 
    lock.unlock();
    condVar.notify_one(); 
}

void CircuitWorker::stop() {
    {
        unique_lock<mutex> lock(queueMutex);
        stopFlag = true; 
    }
    condVar.notify_all(); 
}

future<pair<size_t, vector<double>>> CircuitWorker::getProcessedChunk() {
    unique_lock<mutex> lock(queueMutex);
    condVar.wait(lock, [this] { return !processedChunks.empty() || stopFlag; }); 

    if (stopFlag) {
        throw runtime_error("Worker thread stopped.");
    }

    pair<size_t, vector<double>> processedChunk = move(processedChunks.front()); 
    processedChunks.pop();
    lock.unlock();

    return future<pair<size_t, vector<double>>>(move(processedChunk));
}

void CircuitWorker::processChunks() {
    unique_lock<mutex> lock(queueMutex);
    while (!stopFlag) {
        condVar.wait(lock, [this] { return !chunks.empty() || stopFlag; }); 

        if (stopFlag) {
            break;
        }

        auto chunkPair = move(chunks.front());
        size_t chunkId = chunkPair.first;
        vector<double> chunk = move(chunkPair.second);
        chunks.pop();
        lock.unlock(); 

        vector<double> processedChunk = circuit.process(chunk); 
        {
            lock_guard<mutex> lock(queueMutex);
            processedChunks.push({chunkId, move(processedChunk)}); 
        }
        condVar.notify_one(); 
    }
}
