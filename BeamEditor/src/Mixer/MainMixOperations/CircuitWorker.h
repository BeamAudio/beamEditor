#ifndef CIRCUIT_WORKER_H
#define CIRCUIT_WORKER_H

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Circuit.h"

class CircuitWorker {
public:
    CircuitWorker(const Circuit& circuit);
    future<vector<double>> addChunk(const vector<double>& chunk, size_t chunkId);

private:
    const Circuit& circuit;
    thread workerThread;
    mutex mutex_;
    condition_variable condVar_;
    queue<pair<size_t, vector<double>>> workQueue; 
    bool isRunning;
    void workerLoop();
};

#endif // CIRCUIT_WORKER_H
