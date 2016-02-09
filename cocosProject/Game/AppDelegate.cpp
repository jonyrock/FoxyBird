#include "AppDelegate.h"

#include "Managers/BasicManager.h"
#include "Managers/ScenesManager.h"
#include "Managers/DataManager.h"
#include "Managers/AdvertisingManager.h"
#include "Managers/SettingsManager.h"
#include "Managers/SoundsManager.h"
#include "Managers/LivesManager.h"
#include "Managers/ScoreManager.h"
#include "Managers/EconomicsManager.h"
#include "Managers/AnalyticsManager.h"
#include "Managers/ReviewManager.h"

#include "SonarFrameworks.h"

#include "SimpleAudioEngine.h"

#include <iostream>
#include <vector>


using namespace cocos2d;
using namespace std;


/* INITS */

bool AppDelegate::init() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::createWithRect(
            "Foxy Bird",
            Rect(100, 100,
                 //1136 * 1.3, 640 * 1.3
                 1130, 640
            )
        );
        director->setOpenGLView(glview);
    }
    multiresolutionSupport();
    FileUtils::getInstance()->addSearchPath(
        FileUtils::getInstance()->getWritablePath()
    );
#ifdef COCOS2D_DEBUG
    //director->setDisplayStats(true);
    //TODO: add choosed resource folder
#endif
    director->setAnimationInterval(1.0 / 60);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SonarCocosHelper::IOS::Setup();
#endif
    
    initManagers();
    
    ReviewManager::getInstance()->triggerReview();
    
    //ScenesManager::getInstance()->runWithGame();
    ScenesManager::getInstance()->runWithMain();
    //ScenesManager::getInstance()->showFullVersion();
    //ScenesDirector::getInstance()->runWithTest();

    return true;
}

bool AppDelegate::initManagers() {
    auto p = [this](BasicManager* bm) { 
        _managers.push_back(bm); 
    };
    p(DataManager::getInstance());
    p(SettingsManager::getInstance());
    p(SoundsManager::getInstance());
    p(LivesManager::getInstance());
    p(ScoreManager::getInstance());
    p(ScenesManager::getInstance());
    p(EconomicsManager::getInstance());
    p(AdvertisingManager::getInstance());
    p(AnalyticsManager::getInstance());
    p(ReviewManager::getInstance());
    bool ok = true;
    forAllManagers([&ok](BasicManager* m) {
        if(!m->init()) {
            ok = false;
        }
    });
    if(!ok) {
        return false;
    }
    return true;
}

/* HANDLERS */

bool AppDelegate::applicationDidFinishLaunching() {
    return init();
}

void AppDelegate::applicationDidEnterBackground() {
    forAllManagers([](BasicManager* m) { 
        m->onAppPause();
    });
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    Director::getInstance()->resume();
    forAllManagers([](BasicManager* m) {
        m->onAppResume();
    });
}

/* FUNS */

void AppDelegate::multiresolutionSupport() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    Size designSize = cocos2d::Size(640, 320);
    Size resourceSize = cocos2d::Size(640, 320);
    Size screenSize = glview->getFrameSize();
    designSize = Size(screenSize.width / 2, screenSize.height / 2);
    resourceSize = screenSize;
    director->setContentScaleFactor(resourceSize.width / designSize.width);
    glview->setDesignResolutionSize(640, 320, ResolutionPolicy::FIXED_WIDTH);
    if (screenSize.width < 480) {
	// it is HD. we decided not to support SD
	FileUtils::getInstance()->addSearchResolutionsOrder("HD");
    } else if (480 <= screenSize.width && screenSize.width < 1152) {
        FileUtils::getInstance()->addSearchResolutionsOrder("HD");
    } else {
        FileUtils::getInstance()->addSearchResolutionsOrder("HDR");
    }
}

/* TOOLS */

void AppDelegate::forAllManagers(function<void(BasicManager*)> f) {
    for(size_t t = 0; t < _managers.size(); t++) {
        BasicManager* m = _managers.at(t);
        f(m);
    }
}
