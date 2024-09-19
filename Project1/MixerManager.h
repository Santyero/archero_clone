#pragma once
#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace Game {

    class MixerManager {
    public:
        MixerManager();
        ~MixerManager();

        // Inicializa o SDL_mixer
        void initialize();

        // Carrega uma música
        void loadMusic(const std::string& id, const std::string& filePath);

        // Carrega um efeito sonoro
        void loadSound(const std::string& id, const std::string& filePath);

        // Reproduz uma música
        void playMusic(const std::string& id, int loops = -1);

        // Reproduz um efeito sonoro
        void playSound(const std::string& id, int loops = 0);

        // Para a música atual
        void stopMusic();

        // Pausa a música atual
        void pauseMusic();

        // Retoma a música pausada
        void resumeMusic();

        // Define o volume da música (0-128)
        void setMusicVolume(int volume);

        // Define o volume dos efeitos sonoros (0-128)
        void setSoundVolume(int volume);

    private:
        using MusicPtr = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;
        using SoundPtr = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>;

        std::unordered_map<std::string, MusicPtr> music;
        std::unordered_map<std::string, SoundPtr> sounds;

        // Função auxiliar para lançar exceções
        void throwSDLMixerError(const std::string& message);
    };

}