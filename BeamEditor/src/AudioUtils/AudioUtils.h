// AudioUtils.h

#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <vector>

using namespace std;

class AudioUtils {
private:
    int chunkSize = 100; 

public:
    AudioUtils();
    vector<vector<vector<double>>> makeChunks(vector<vector<double>> channels);
};

#endif // AUDIO_UTILS_H
