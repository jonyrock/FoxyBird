#pragma once

#include "Managers/BasicManager.h"
#include "Managers/SettingsManager.h"

#include <string>


class SoundsManager : public BasicManager {

public:
    static SoundsManager* getInstance();
    
    virtual bool init() override;
    
    void playMusic(bool startAgain=true);
    void stopMusic();
    void playJumpEffect();
    void playGameOverEffect();
    void playErrorEffect();
    void playNextStageEffect();
    void playButtonEffect();
    void playNewRecord();
    std::string setNextState();
    
    virtual void onMusicSettingsChange(bool enabled);
    virtual void onAppPause() override;
    virtual void onAppResume() override;

private:
    friend SettingsManager;
    
    SoundsManager();
    SoundsManager(const SoundsManager& orig);
    
    bool _musicInited = false;
    bool _musicTime = false;
    
    
} ;



