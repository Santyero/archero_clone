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

        // Carrega uma m�sica
        void loadMusic(const std::string& id, const std::string& filePath);

        // Carrega um efeito sonoro
        void loadSound(const std::string& id, const std::string& filePath);

        // Reproduz uma m�sica
        void playMusic(const std::string& id, int loops = -1);

        // Reproduz um efeito sonoro
        void playSound(const std::string& id, int loops = 0);

        // Para a m�sica atual
        void stopMusic();

        // Pausa a m�sica atual
        void pauseMusic();

        // Retoma a m�sica pausada
        void resumeMusic();

        // Define o volume da m�sica (0-128)
        void setMusicVolume(int volume);

        // Define o volume dos efeitos sonoros (0-128)
        void setSoundVolume(int volume);

    private:
        using MusicPtr = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;
        using SoundPtr = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>;

        std::unordered_map<std::string, MusicPtr> music;
        std::unordered_map<std::string, SoundPtr> sounds;

        // Fun��o auxiliar para lan�ar exce��es
        void throwSDLMixerError(const std::string& message);
    };

}