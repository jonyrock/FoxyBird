#include "Managers/SoundsManager.h"
#include "Managers/DataManager.h"
#include "Managers/SettingsManager.h"
#include "Managers/LocalizationManager.h"

#include <SimpleAudioEngine.h>
#include <cocos2d.h>

#include <string>


using namespace std;
using namespace cocos2d;


SoundsManager* SoundsManager::getInstance() {
    static SoundsManager instance;
    return &instance;
}

SoundsManager::SoundsManager() {
}

bool SoundsManager::init() {
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/background.mp3");
    return true;
}

void SoundsManager::playMusic(bool startAgain) {
    _musicTime = true;
    if(!SettingsManager::getInstance()->getIsMusicOn()) return;
    if(startAgain || !_musicInited) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/background.mp3", true);
        _musicInited = true;
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
}

void SoundsManager::stopMusic() {
    _musicTime = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundsManager::playJumpEffect() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/jump.wav"
    ); 
}

void SoundsManager::playGameOverEffect() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn() && 
       !SettingsManager::getInstance()->getIsMusicOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/game_over.mp3"
    ); 
}

void SoundsManager::playErrorEffect() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/wrong_ice.wav"
    ); 
}

void SoundsManager::playNextStageEffect() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/new_stage.wav"
    ); 
}

void SoundsManager::playButtonEffect() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/button.wav"
    ); 
}

void SoundsManager::playNewRecord() {
    if(!SettingsManager::getInstance()->getIsSoundEffectsOn()) return;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
	"sounds/new_record.mp3"
    ); 
}

void SoundsManager::onAppPause() {
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    _musicInited = false;
}

void SoundsManager::onAppResume() {
    if(!_musicTime)
        stopMusic();
}

void SoundsManager::onMusicSettingsChange(bool enabled) {
    if(!_musicTime) return;
    if(enabled) {
	playMusic(false);
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}



