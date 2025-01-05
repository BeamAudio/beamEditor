// memory_loader.cpp
#include "memory_loader.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sndfile.h>
#include <cmath>
#include <limits>

namespace fs = std::filesystem;
using namespace std;

vector<MemoryLoader::AudioData> MemoryLoader::loadAudioFiles(const string& folderPath) {
    vector<AudioData> audioDataList;

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        cerr << "Invalid folder path: " << folderPath << endl;
        return audioDataList;
    }

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string filePath = entry.path().string();
            string extension = entry.path().extension().string();

            if (extension == ".wav" || extension == ".mp3") {
                AudioData audio = loadAudioFile(filePath);
                if (!audio.filename.empty()) {
                    audioDataList.push_back(audio);
                }
            }
        }
    }
    return audioDataList;
}

MemoryLoader::AudioData MemoryLoader::loadAudioFile(const string& filePath) {
    SF_INFO sfinfo;
    SNDFILE* sndFile = sf_open(filePath.c_str(), SFM_READ, &sfinfo);

    AudioData audio;
    audio.filename = fs::path(filePath).filename().string();

    if (!sndFile) {
        cerr << "Error opening file: " << filePath << endl;
        return audio;
    }

    audio.sampleRate = sfinfo.samplerate;
    audio.channels = sfinfo.channels;
    audio.frameCount = sfinfo.frames;
    audio.duration = static_cast<double>(audio.frameCount) / audio.sampleRate;

    if (audio.sampleRate <= 0 || audio.channels <= 0 || audio.frameCount < 0 || audio.duration < 0) {
        cerr << "Invalid audio parameters in file: " << filePath << endl;
        sf_close(sndFile);
        return AudioData{};
    }

    vector<int16_t> buffer(audio.frameCount * audio.channels);
    sf_count_t framesRead = sf_read_short(sndFile, buffer.data(), audio.frameCount * audio.channels);
    sf_close(sndFile);

    if (framesRead != audio.frameCount * audio.channels) {
        cerr << "Could not read all frames from file: " << filePath << endl;
        return AudioData{}; // Return empty AudioData
    }

    audio.data.resize(audio.channels);
    for (int c = 0; c < audio.channels; ++c) {
        audio.data[c].resize(audio.frameCount);
        for (long long i = 0; i < audio.frameCount; ++i) {
            audio.data[c][i] = static_cast<double>(buffer[i * audio.channels + c]) / numeric_limits<int16_t>::max();
        }
    }

    audio.bitRate = static_cast<int>(fs::file_size(filePath) * 8 / audio.duration);

    return audio;
}

bool MemoryLoader::writeAudioFile(const AudioData& audio, const string& outputPath) {
    SF_INFO sfinfo;
    sfinfo.samplerate = audio.sampleRate;
    sfinfo.channels = audio.channels;
    sfinfo.frames = audio.frameCount;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(outputPath.c_str(), SFM_WRITE, &sfinfo);

    if (!outfile) {
        cerr << "Error opening output file: " << outputPath << endl;
        return false;
    }

    vector<int16_t> buffer(audio.frameCount * audio.channels);

    for (long long i = 0; i < audio.frameCount; ++i) {
        for (int c = 0; c < audio.channels; ++c) {
            double sample = audio.data[c][i] * numeric_limits<int16_t>::max();

            if (sample > numeric_limits<int16_t>::max()) {
                sample = numeric_limits<int16_t>::max();
            }
            else if (sample < numeric_limits<int16_t>::min()) {
                sample = numeric_limits<int16_t>::min();
            }

            buffer[i * audio.channels + c] = static_cast<int16_t>(round(sample));
        }
    }

    sf_count_t framesWritten = sf_write_short(outfile, buffer.data(), audio.frameCount * audio.channels);
    sf_close(outfile);

    if (framesWritten != audio.frameCount * audio.channels) {
        cerr << "Could not write all frames to file: " << outputPath << endl;
        return false;
    }

    return true;
}