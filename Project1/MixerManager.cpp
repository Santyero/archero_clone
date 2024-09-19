#include "MixerManager.h"
#include <stdlib.h>
#include <iostream>

namespace Game {

    MixerManager::MixerManager() {
        initialize();
    }

    MixerManager::~MixerManager() {
        Mix_CloseAudio();
        Mix_Quit();
    }

    void MixerManager::initialize() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            throwSDLMixerError("SDL_mixer could not initialize!");
        }
    }

    void MixerManager::loadMusic(const std::string& id, const std::string& filePath) {
        Mix_Music* musicPtr = Mix_LoadMUS(filePath.c_str());
        if (!musicPtr) {
            throwSDLMixerError("Failed to load music: " + filePath);
        }
        music.emplace(id, MusicPtr(musicPtr, Mix_FreeMusic));
    }

    void MixerManager::loadSound(const std::string& id, const std::string& filePath) {
        Mix_Chunk* soundPtr = Mix_LoadWAV(filePath.c_str());
        if (!soundPtr) {
            throwSDLMixerError("Failed to load sound: " + filePath);
        }
        sounds.emplace(id, SoundPtr(soundPtr, Mix_FreeChunk));
    }

    void MixerManager::playMusic(const std::string& id, int loops) {
        auto it = music.find(id);
        if (it != music.end()) {
            if (Mix_PlayMusic(it->second.get(), loops) == -1) {
                throwSDLMixerError("Failed to play music: " + id);
            }
        }
        else {
            throw std::runtime_error("Music not found: " + id);
        }
    }

    void MixerManager::playSound(const std::string& id, int loops) {
        auto it = sounds.find(id);
        if (it != sounds.end()) {
            int freeChannel = Mix_GroupAvailable(-1);
            if (freeChannel == -1) {
                // Nenhum canal livre, force a liberação do canal mais antigo
                Mix_HaltChannel(0);
                freeChannel = 0;
            }
            if (Mix_PlayChannel(freeChannel, it->second.get(), loops) == -1) {
                throwSDLMixerError("Failed to play sound: " + id + ". Reason: ");
            }
        }
        else {
            throw std::runtime_error("Sound not found: " + id);
        }
    }

    void MixerManager::stopMusic() {
        Mix_HaltMusic();
    }

    void MixerManager::pauseMusic() {
        Mix_PauseMusic();
    }

    void MixerManager::resumeMusic() {
        Mix_ResumeMusic();
    }

    void MixerManager::setMusicVolume(int volume) {
        Mix_VolumeMusic(volume);
    }

    void MixerManager::setSoundVolume(int volume) {
        Mix_Volume(-1, volume);
    }

    void MixerManager::throwSDLMixerError(const std::string& message) {
        //throw std::runtime_error(message + " SDL_mixer Error: " + Mix_GetError());
        std::cout << message << " SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

} // namespace Game