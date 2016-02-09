#pragma once

#include <cocos2d.h>

class TutorialArrowAsset : public cocos2d::Node {

public:
    CREATE_FUNC(TutorialArrowAsset);
    
private:
    
    /* STATE */
    
    /* ELEMENTS */
    cocos2d::Sprite* _sprite;
    
    /* INITS */
    bool init() override;
    void initSprite();
    void initAnimation();
    
};