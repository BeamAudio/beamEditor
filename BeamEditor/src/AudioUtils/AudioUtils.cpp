// AudioUtils.cpp

#include "AudioUtils.h"

AudioUtils::AudioUtils() {}

vector<vector<vector<double>>> AudioUtils::makeChunks(vector<vector<double>> channels) {
    const int chunkSize = 100; 
    vector<vector<vector<double>>> result;

    for (const vector<double>& channel : channels) {
        vector<vector<double>> channelChunks;
        int numChunks = (channel.size() + chunkSize - 1) / chunkSize; 

        for (size_t i = 0; i < numChunks; i++) {
            size_t startIdx = i * chunkSize; 
            size_t endIdx = min(startIdx + chunkSize, channel.size()); 

            vector<double> chunk(channel.begin() + startIdx, channel.begin() + endIdx);
            channelChunks.push_back(chunk);
        }

        result.push_back(channelChunks);
    }

    return result;
}
