#include "ScoreManager.h"

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

    ScoreManager::~ScoreManager() {}
}