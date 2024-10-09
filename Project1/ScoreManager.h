#pragma once

namespace Game {
    class ScoreManager {
    private:
        static ScoreManager* instance;
        int points;
        int level;

        ScoreManager(); // Private constructor
    public:
        static ScoreManager* getInstance();

        void addPoints(int points);
        int getPoints() const;
        int getLevel() const;
        void setLevel(int level);
		void resetPoints();

        ~ScoreManager();
    };
}