#include "ScenesManager.h"
#include "SoundsManager.h"
#include "LivesManager.h"
#include "AnalyticsManager.h"

#include "Scenes/GameScene.h"
#include "Scenes/MainScene.h"
#include "Scenes/TestScene.h"
#include "Scenes/LoadingScene.h"

#include "GUI/FullVersionLayer.h"
#include "GUI/LivesBoxAsset.h"
#include "GUI/ExitMessageBox.h"

#include <cocos2d.h>

#include <string>

const int FULL_VERSION_LAYER_TAG = 3;
const int SETTINGS_ASSET_TAG     = 5;
const int LIVES_BOX_ASSET_TAG    = 7;
const int EXIT_BOX_LAYER_TAG     = 11;

const std::string EVENT_GAME_SCENE_LOADED = "EVENT_GAME_SCENE_LOADED";

using namespace cocos2d;


void ScenesManager::runWithMain() {
    CCLOG("ScenesManager::runWithMain ENTER");
    _currentScene = nullptr;
    _currentScene = MainScene::create();
    auto director = Director::getInstance();
    director->runWithScene(_currentScene);
    AnalyticsManager::getInstance()->setScene("Main");
    afterInit();
    CCLOG("ScenesManager::runWithMain EXIT");
}

bool ScenesManager::gotoGame() {
    if(LivesManager::getInstance()->isEmpty()) {
        showFullVersion();
        return false;
    }
    LivesManager::getInstance()->takeLife();
    
    if(!_isLoaded) {
        _isLoaded = true;
        _currentScene = LoadingScene::create();
        auto director = Director::getInstance();
        director->replaceScene(_currentScene);

        _currentScene->runAction(Sequence::create(
            CallFunc::create([this,director]() {
                // just creation to cache data
                gotoGameScene(GameScene::create());
            }), nullptr
        ));
        
    } else {
        gotoGameScene();
    }
    
    return true;
}

void ScenesManager::gotoGameScene(GameScene* gscene) {
    if(gscene == nullptr) {
        _currentScene = GameScene::create();
    } else {
        _currentScene = GameScene::create();
    }
    
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.2, _currentScene, Color3B(0,0,0)));
    //director->replaceScene(_currentScene);
    AnalyticsManager::getInstance()->setScene("Game");
    afterInit();
}

void ScenesManager::gotoMain() {
    _currentScene = nullptr;
    _currentScene = MainScene::create();
    auto director = Director::getInstance();
    director->replaceScene(_currentScene);
    AnalyticsManager::getInstance()->setScene("Main");
    afterInit();
}

ScenesManager::ScenesManager() {
    auto keysListener = EventListenerKeyboard::create();
    keysListener->onKeyReleased = 
	[this](EventKeyboard::KeyCode keyCode, Event* event) {
	    if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            if(_currentScene == nullptr) {
                return;
            }
            if(isFullVersionOpen()) {
                hideFullVersion();
            } else {
                _currentScene->onBack();
            }
	    }
	};

    Director::getInstance()->
        getEventDispatcher()->
        addEventListenerWithFixedPriority(keysListener, 1);
}

void ScenesManager::onAppPause() {
    _currentScene->onAppPause();
    AnalyticsManager::getInstance()->sendEvent("pause");
}

void ScenesManager::onAppResume() {
    _currentScene->onAppResume();
    AnalyticsManager::getInstance()->sendEvent("resume");
}

void ScenesManager::showFullVersion() {
    if(isFullVersionOpen()) return;
    // z-order should be more that 200 due to setting asset
    // see showSettings()
    auto l = FullVersionLayer::create();
    _currentScene->addChild(l, 201, FULL_VERSION_LAYER_TAG);
    AnalyticsManager::getInstance()->sendEvent("show full version");
}

void ScenesManager::hideFullVersion() {
    auto l = _currentScene->getChildByTag(FULL_VERSION_LAYER_TAG);
    if(l) { 
        l->removeFromParent();
        AnalyticsManager::getInstance()->sendEvent("hide full version");
    }
}

bool ScenesManager::isFullVersionOpen() {
    return _currentScene->getChildByTag(FULL_VERSION_LAYER_TAG) != nullptr;
}

bool ScenesManager::isFullVersionInProcess() {
    if(!isFullVersionOpen()) {
        return false;
    }
    auto n = _currentScene->getChildByTag(FULL_VERSION_LAYER_TAG);
    return dynamic_cast<FullVersionLayer*>(n)->isInProcess();
}

void ScenesManager::showSettings() {
    if(!_currentScene) {
        _showSettingsAfterInit = true;
        return;
    }
    auto settingsAsset = _currentScene->getChildByTag(SETTINGS_ASSET_TAG);
    if(!settingsAsset) {
        settingsAsset = SettingsAsset::create();
        settingsAsset->setPosition(Utils::coordFromPSD({-70, 70}));
        settingsAsset->setCameraMask((unsigned char)CameraFlag::DEFAULT);
        _currentScene->addChild(settingsAsset, 200, SETTINGS_ASSET_TAG);
    }
    settingsAsset->setVisible(true);
    AnalyticsManager::getInstance()->sendEvent("show settings");
}

void ScenesManager::hideSettings() {
    hideByTag(SETTINGS_ASSET_TAG);
    AnalyticsManager::getInstance()->sendEvent("hide settings");
}

void ScenesManager::showLives() {
    if(!_currentScene) {
        _showLivesAfterInit = true;
        return;
    }
    auto livesAsset = _currentScene->getChildByTag(LIVES_BOX_ASSET_TAG);
    if(!livesAsset) {
        livesAsset = LivesBoxAsset::create();
        livesAsset->setPosition(Utils::coordFromPSD({90, 68}));
        livesAsset->setCameraMask((unsigned char)CameraFlag::DEFAULT);
        _currentScene->addChild(livesAsset, 200, LIVES_BOX_ASSET_TAG);
    }
    livesAsset->setVisible(true);
    AnalyticsManager::getInstance()->sendEvent("show lives");
}

void ScenesManager::hideLifes() {
    hideByTag(LIVES_BOX_ASSET_TAG);
    AnalyticsManager::getInstance()->sendEvent("hide lives");
}

void ScenesManager::afterInit() {
    if(_showSettingsAfterInit) {
        _showSettingsAfterInit = false;
        showSettings();
    }
    if(_showLivesAfterInit) {
        _showLivesAfterInit = false;
        showLives();
    }
}

void ScenesManager::hideByTag(const int tag) {
    if(!_currentScene) {
        return;
    }
    auto asset = _currentScene->getChildByTag(tag);
    if(!asset) {
        return;
    }
    asset->removeFromParent();
}

ExitMessageBox* ScenesManager::toggleExitMessage() {
    
    ExitMessageBox* eLayer;
    eLayer = static_cast<ExitMessageBox*>(
        _currentScene->getChildByTag(EXIT_BOX_LAYER_TAG)
    );
    if(eLayer) {
      eLayer->close();
      AnalyticsManager::getInstance()->sendEvent("hide exit message");
      return nullptr;
    }
    eLayer = ExitMessageBox::create();
    _currentScene->addChild(eLayer, 500, EXIT_BOX_LAYER_TAG);
    AnalyticsManager::getInstance()->sendEvent("show exit message");
    return eLayer;
}

