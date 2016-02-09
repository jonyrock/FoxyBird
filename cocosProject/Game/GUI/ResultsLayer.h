#pragma once 

#include <cocos2d.h>

class ResultsLayer : public cocos2d::Layer {
    
public:
    CREATE_FUNC(ResultsLayer);
    void setScore(int score);
    void onBack();

protected:
    
    /* DATA */
    cocos2d::Size	_size;

    /* ELEMENTS */
    cocos2d::DrawNode*  _background;
    
    /* STATE */
    bool _isFullVersion = false;

    /* INITS */
    bool init() override;
    void initBackground();
    void initLogo();
    void initPlayButton();
    void initOverlayListener();
    // trial version
    void initLives();
    void initGetUnlimited();
    // full version
    void initFullVersionLogo();
    
    /* HANDLERS */
    void onShowFull();
    
};