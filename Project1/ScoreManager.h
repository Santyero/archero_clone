#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace Game {

    struct PlayerScore {
        std::string name;
        int score;
    };

    class ScoreManager {
    private:
        static ScoreManager* instance;
        int points;
        int level;

        ScoreManager(); // Private constructor
        std::filesystem::path getScoreFilePath();

    public:
        static ScoreManager* getInstance();

        void addPoints(int points);
        int getPoints() const;
        int getLevel() const;
        void setLevel(int level);
		void resetPoints();
        void saveScore(const std::string& playerName);
        std::vector<PlayerScore> getHighScores();

        ~ScoreManager();
    };
}