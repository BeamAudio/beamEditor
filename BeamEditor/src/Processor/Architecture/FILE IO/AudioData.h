#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

struct AudioData {
    std::string filename;
    std::vector<std::vector<double>> data;
    int sampleRate;
    int channels;
    int bitRate;
    long long frameCount;
    double duration;
};

#endif AUDIO_DATA_H