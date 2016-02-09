#pragma once

#include "BasicManager.h"
#include "Scenes/BasicScene.h"
#include "Scenes/GameScene.h"
#include "GUI/ExitMessageBox.h"

class ScenesManager : public BasicManager {
    
public:
    
    inline bool init() override { return true; }
    
    static ScenesManager* getInstance() {
        static ScenesManager instance;
        return &instance;
    }
    
    void runWithMain();
    
    bool gotoGame();
    void gotoMain();
    
    void onAppPause() override;
    void onAppResume() override;
    
    void showFullVersion();
    void hideFullVersion();
    bool isFullVersionOpen();
    bool isFullVersionInProcess();
    
    void showSettings();
    void hideSettings();
    
    void showLives();
    void hideLifes();
    
    ExitMessageBox* toggleExitMessage();

private:

    ScenesManager();
    
    /* STATE */
    bool _showSettingsAfterInit = false;
    bool _showLivesAfterInit = false;
    bool _isLoaded = false;
    
    /* ELEMENTS */
    BasicScene* _currentScene = nullptr;
    
    /* TOOLS */
    void afterInit();
    void hideByTag(const int tag);
    void gotoGameScene(GameScene* gscene = nullptr);
    
    ScenesManager(const ScenesManager &orig);

};



