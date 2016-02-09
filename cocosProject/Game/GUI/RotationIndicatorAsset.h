#pragma once

#include <cocos2d.h>

class RotationIndicatorAsset : public cocos2d::Node {
public:
    CREATE_FUNC(RotationIndicatorAsset);
    void setInfoRotation(float a);
    
private:
    
    /* ELEMENTS */
    cocos2d::Sprite* _sprite;

    /* INITS */
    bool init() override;
    void initSprite();
    
};