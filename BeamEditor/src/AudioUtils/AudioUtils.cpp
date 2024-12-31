// AudioUtils.cpp

#include "AudioUtils.h"

AudioUtils::AudioUtils() {}

vector<vector<vector<vector<double>>>> AudioUtils::makeChunks(const vector<vector<vector<double>>>& channels) {
    const int chunkSize = 100; 
    vector<vector<vector<vector<double>>>> result; 

    for (size_t i = 0; i < channels.size(); ++i) { // Iterate through channels
        const vector<vector<double>>& channel = channels[i]; 
        vector<vector<vector<double>>> channelChunks(channel.size(), vector<vector<double>>()); // Initialize with subchannel count

        for (size_t j = 0; j < channel.size(); ++j) { // Iterate through subchannels
            const vector<double>& subchannel = channel[j];
            int numChunks = (subchannel.size() + chunkSize - 1) / chunkSize; 

            for (size_t k = 0; k < numChunks; ++k) { 
                size_t startIdx = k * chunkSize; 
                size_t endIdx = min(startIdx + chunkSize, subchannel.size()); 

                vector<double> chunk(subchannel.begin() + startIdx, subchannel.begin() + endIdx);
                channelChunks[j].push_back(chunk); 
            }
        }

        result.push_back(channelChunks); 
    }

    return result;
}

vector<double> AudioUtils::mixChannelsChunk(const vector<vector<vector<double>>>& chunks, const vector<double>& amplification, const vector<double>& panning) {
    vector<double> output;

    for (size_t i = 0; i < chunks.size(); ++i) { // Iterate over channels
        if (chunks[i].size() == 1) { // Mono channel
            for (double sample : chunks[i][0]) { 
                output.push_back(sample * amplification[i]);
            }
        } else if (chunks[i].size() == 2) { // Stereo channel
            for (size_t j = 0; j < chunks[i][0].size(); ++j) { 
                double leftSample = chunks[i][0][j] * amplification[i] * (1.0 - panning[i]);
                double rightSample = chunks[i][1][j] * amplification[i] * panning[i];
                output.push_back(leftSample + rightSample); 
            }
        } else {
            // Handle invalid channel configuration (e.g., more than 2 subchannels)
            cerr << "Invalid channel configuration: " << i << "th channel has " << chunks[i].size() << " subchannels. Expected 1 or 2." << endl;
            return output; // Return an empty vector or handle the error appropriately
        }
    }

    return output;
} 
            
