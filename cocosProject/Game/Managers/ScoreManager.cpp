#include "ScoreManager.h"
#include "DataManager.h"

#define DB_BEST_SORE "bestScore"

ScoreManager* ScoreManager::getInstance() {
    static ScoreManager instance;
    return &instance;
}

bool ScoreManager::init() { return true; }

int ScoreManager::getBestScore() {
    return DataManager::getInstance()->get(DB_BEST_SORE, 0);
}

bool ScoreManager::sendMyScore(int v) {
    if(getBestScore() >= v) return false;
    DataManager::getInstance()->put(DB_BEST_SORE, v);
    return true;
}
