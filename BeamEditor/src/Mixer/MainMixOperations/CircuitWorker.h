#ifndef CIRCUIT_WORKER_H
#define CIRCUIT_WORKER_H

#include <thread>
#include <future>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>



class CircuitWorker {
public:
    CircuitWorker(const Circuit& circuit, size_t workerId); 
    ~CircuitWorker();

    void addChunk(const vector<double>& chunk, size_t chunkId); 

    future<pair<size_t, vector<double>>> getProcessedChunk(); 

private:
    const Circuit& circuit;
    size_t workerId;
    thread workerThread;
    queue<pair<size_t, vector<double>>> chunks; 
    queue<pair<size_t, vector<double>>> processedChunks;
    mutex queueMutex;
    condition_variable condVar;
    atomic<bool> stopFlag;

    void processChunks(); 
};

#endif // CIRCUIT_WORKER_H
