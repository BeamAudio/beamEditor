// memory_loader.h
#ifndef MEMORY_LOADER_H
#define MEMORY_LOADER_H

#include <string>
#include <vector>
#include <AudioData.h>
#include<ProjectFile.h>

class MemoryLoader {
public:
    



    std::vector<AudioData> loadAudioFiles(const std::string& folderPath);
    bool writeAudioFile(const AudioData& audio, const std::string& outputPath);

private:
    AudioData loadAudioFile(const std::string& filePath);
    ProjectFile project;
};

#endif // MEMORY_LOADER_H