#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <algorithm>

bool isAbsolutePath(const std::string& path) {
#ifdef _WIN32
    // e.g., C:\... or C:/...
    return path.size() > 2 && std::isalpha(path[0]) && path[1] == ':' &&
           (path[2] == '/' || path[2] == '\\');
#else
    // Treat only root paths like "/home" as absolute
    return !path.empty() && path[0] == '/' && path.substr(0, 2) != "./" && path.substr(0, 3) != "../";
#endif
}



std::string resolvePath(const std::string& filePath) {
    if (isAbsolutePath(filePath)) return filePath;

    std::string basePath = __FILE__;
    size_t lastSlash = basePath.find_last_of("/\\");
    std::string dir = (lastSlash != std::string::npos) ? basePath.substr(0, lastSlash + 1) : "";

    return dir + filePath;
}



class AudioManager {
private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    std::unordered_map<std::string, ALuint> buffers;
    std::unordered_map<std::string, ALuint> sources;
    bool isInitialized = false;

    bool isAbsolutePath(const std::string& path) {
#ifdef _WIN32
        return path.size() > 1 && path[1] == ':';
#else
        return !path.empty() && path[0] == '/';
#endif
    }

    void cleanupSource(const std::string& name) {
        auto it = sources.find(name);
        if (it != sources.end()) {
            alSourceStop(it->second);
            alDeleteSources(1, &it->second);
            sources.erase(it);
        }
    }

public:
    bool initialize() {
        if (isInitialized) return true;

        device = alcOpenDevice(nullptr);
        if (!device) {
            std::cerr << "Failed to open audio device\n";
            return false;
        }

        context = alcCreateContext(device, nullptr);
        if (!context) {
            std::cerr << "Failed to create audio context\n";
            alcCloseDevice(device);
            return false;
        }

        alcMakeContextCurrent(context);
        isInitialized = true;
        std::cerr << "AudioManager initialized.\n";
        return true;
    }

    bool loadSound(const std::string& name, const std::string& filePath) {
        if (!isInitialized) return false;

        std::ifstream file(resolvePath(filePath), std::ios::binary);

        if (!file) {
            std::cerr << "Failed to open sound file: " << filePath << "\n";
            return false;
        }

        char riffHeader[4], waveHeader[4];
        int chunkSize;
        file.read(riffHeader, 4);
        file.read(reinterpret_cast<char*>(&chunkSize), 4);
        file.read(waveHeader, 4);

        if (std::strncmp(riffHeader, "RIFF", 4) != 0 || std::strncmp(waveHeader, "WAVE", 4) != 0) {
            std::cerr << "Invalid WAV file header: " << filePath << "\n";
            return false;
        }

        short audioFormat = 0, numChannels = 0, bitsPerSample = 0;
        int sampleRate = 0, dataSize = 0;
        ALenum format = 0;
        std::vector<char> audioData;

        while (file && !file.eof()) {
            char chunkId[4];
            int chunkLen = 0;
            file.read(chunkId, 4);
            file.read(reinterpret_cast<char*>(&chunkLen), 4);

            if (std::strncmp(chunkId, "fmt ", 4) == 0) {
                std::vector<char> fmt(chunkLen);
                file.read(fmt.data(), chunkLen);
                audioFormat   = *reinterpret_cast<short*>(&fmt[0]);
                numChannels   = *reinterpret_cast<short*>(&fmt[2]);
                sampleRate    = *reinterpret_cast<int*>(&fmt[4]);
                bitsPerSample = *reinterpret_cast<short*>(&fmt[14]);

                if (audioFormat != 1) {
                    std::cerr << "Unsupported WAV format (not PCM): " << audioFormat << "\n";
                    return false;
                }

                if (numChannels == 1 && bitsPerSample == 8) format = AL_FORMAT_MONO8;
                else if (numChannels == 1 && bitsPerSample == 16) format = AL_FORMAT_MONO16;
                else if (numChannels == 2 && bitsPerSample == 8) format = AL_FORMAT_STEREO8;
                else if (numChannels == 2 && bitsPerSample == 16) format = AL_FORMAT_STEREO16;
                else {
                    std::cerr << "Unsupported format (channels=" << numChannels
                              << ", bits=" << bitsPerSample << ")\n";
                    return false;
                }
            } else if (std::strncmp(chunkId, "data", 4) == 0) {
                dataSize = chunkLen;
                audioData.resize(dataSize);
                file.read(audioData.data(), dataSize);
                break;
            } else {
                file.seekg(chunkLen, std::ios::cur);
            }
        }

        if (audioData.empty()) {
            std::cerr << "No audio data found in: " << filePath << "\n";
            return false;
        }

        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, audioData.data(), dataSize, sampleRate);

        ALenum err = alGetError();
        if (err != AL_NO_ERROR) {
            std::cerr << "OpenAL error buffering data: " << alGetString(err) << "\n";
            alDeleteBuffers(1, &buffer);
            return false;
        }

        buffers[name] = buffer;
        std::cerr << "Sound loaded: " << name << "\n";
        return true;
    }

    void playSound(const std::string& name, bool loop = false) {
        if (!isInitialized || buffers.find(name) == buffers.end()) return;

        cleanupSource(name);

        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffers[name]);
        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(source, AL_GAIN, 0.5f);
        alSourcePlay(source);

        sources[name] = source;
    }

    void stopSound(const std::string& name) {
        if (!isInitialized) return;
        cleanupSource(name);
    }

    void setVolume(const std::string& name, float volume) {
        if (!isInitialized || sources.find(name) == sources.end()) return;
        alSourcef(sources[name], AL_GAIN, std::clamp(volume, 0.0f, 1.0f));
    }

    bool isPlaying(const std::string& name) {
        if (!isInitialized || sources.find(name) == sources.end()) return false;
        ALint state;
        alGetSourcei(sources[name], AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void cleanup() {
        if (!isInitialized) return;

        for (auto& [_, source] : sources) {
            alSourceStop(source);
            alDeleteSources(1, &source);
        }
        sources.clear();

        for (auto& [_, buffer] : buffers) {
            alDeleteBuffers(1, &buffer);
        }
        buffers.clear();

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
        isInitialized = false;

        std::cout << "AudioManager cleaned up.\n";
    }

    ~AudioManager() {
        cleanup();
    }
};
