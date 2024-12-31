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
    vector<double> mixChannelsChunk(const vector<vector<double>>& chunks, const vector<double>& amplification, const vector>double>& panning);
};

#endif // AUDIO_UTILS_H
