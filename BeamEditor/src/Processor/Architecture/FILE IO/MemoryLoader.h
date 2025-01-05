// memory_loader.h
#ifndef MEMORY_LOADER_H
#define MEMORY_LOADER_H

#include <string>
#include <vector>

class MemoryLoader {
public:
    struct AudioData {
        std::string filename;
        std::vector<std::vector<double>> data;
        int sampleRate;
        int channels;
        int bitRate;
        long long frameCount;
        double duration;
    };

    std::vector<AudioData> loadAudioFiles(const std::string& folderPath);
    bool writeAudioFile(const AudioData& audio, const std::string& outputPath);

private:
    AudioData loadAudioFile(const std::string& filePath);
};

#endif // MEMORY_LOADER_H