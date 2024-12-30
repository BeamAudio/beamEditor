#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <array>
#include <iostream>

using namespace std;

class CircuitWorker {
public:
    CircuitWorker(Circuit& circuit) : circuit(circuit), stopFlag(false) {
        workerThread = thread(&CircuitWorker::processChunks, this);
    }

    ~CircuitWorker() {
        stop(); 
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    void addChunk(const array<double, 100>& chunk) {
        unique_lock<mutex> lock(queueMutex);
        chunks.push(chunk);
        lock.unlock();
        condVar.notify_one(); 
    }

    void stop() {
        {
            unique_lock<mutex> lock(queueMutex);
            stopFlag = true;
        }
        condVar.notify_all(); 
    }

private:
    Circuit& circuit;
    thread workerThread;
    queue<array<double, 100>> chunks;
    mutex queueMutex;
    condition_variable condVar;
    bool stopFlag;

    void processChunks() {
        unique_lock<mutex> lock(queueMutex);
        while (!stopFlag) {
            condVar.wait(lock, [this] { return !chunks.empty() || stopFlag; });

            if (stopFlag) {
                break;
            }

            array<double, 100> chunk = chunks.front();
            chunks.pop();
            lock.unlock(); 

            array<double, 100> processedChunk = circuit.processChunk(chunk);
            outputProcessedChunk(processedChunk);
        }
    }

    void outputProcessedChunk(const array<double, 100>& chunk) {
        //Output to the playback buffer class
    }
};
