#include "ScoreManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

namespace Game {
    ScoreManager* ScoreManager::instance = nullptr;

    ScoreManager::ScoreManager() : points(0), level(1) {}

    ScoreManager* ScoreManager::getInstance() {  
        if (instance == nullptr) {
            instance = new ScoreManager();
        }
        return instance;
    }

    void ScoreManager::addPoints(int points) {
        this->points += 10 * level;
    }

    int ScoreManager::getPoints() const {
        return points;
    }

    int ScoreManager::getLevel() const {
        return level;
    }

    void ScoreManager::setLevel(int level) {
        this->level = level;
    }

	void ScoreManager::resetPoints() {
		points = 0;
		level = 1;
	}

    std::filesystem::path ScoreManager::getScoreFilePath() {
        char* pValue;
        size_t len;
        errno_t err = _dupenv_s(&pValue, &len, "LOCALAPPDATA");
        if (err) {
            // Lidar com o erro, talvez usar um caminho padrão
            return "scores.txt";
        }
        auto appDataPath = std::filesystem::path(pValue) / "ArcheroClone";
        free(pValue);
        std::filesystem::create_directories(appDataPath);
        return appDataPath / "scores.txt";
    }

    void ScoreManager::saveScore(const std::string& playerName) {
        std::ofstream file(getScoreFilePath(), std::ios::app);
        if (file.is_open()) {
            file << playerName << "," << this->points << "\n";
            file.close();
        }
    }

    std::vector<PlayerScore> ScoreManager::getHighScores() {
        std::vector<PlayerScore> scores;
        std::ifstream file(getScoreFilePath().string());
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (std::getline(iss, name, ',') && iss >> score) {
                scores.push_back({ name, score });
            }
        }

        std::sort(scores.begin(), scores.end(),
            [](const PlayerScore& a, const PlayerScore& b) { return a.score > b.score; });

        return scores;
    }

    ScoreManager::~ScoreManager() {}
}