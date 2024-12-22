#include<vector>

using namespace std;

class AudioUtils {

    private:

        int chunkSize = 100;

    public:
        AudioUtils() {}

        vector<vector<vector<double>>> makeChunks(vector<vector<double>> channels) {
            const int chunkSize = 100;  // Size of each chunk
            vector<vector<vector<double>>> result;

            // Iterate through each channel
            for (const vector<double>& channel : channels) {
                vector<vector<double>> channelChunks;
                int numChunks = (channel.size() + chunkSize - 1) / chunkSize;  // Calculate the number of chunks

                // Split the channel into chunks
                for (size_t i = 0; i < numChunks; i++) {
                    size_t startIdx = i * chunkSize;  // Starting index of the chunk
                    size_t endIdx = min(startIdx + chunkSize, channel.size());  // Ending index (exclusive)

                    // Create the chunk
                    vector<double> chunk(channel.begin() + startIdx, channel.begin() + endIdx);
                    channelChunks.push_back(chunk);
                }

                // Add the chunks of this channel to the result
                result.push_back(channelChunks);
            }

            return result;
        }

    
};