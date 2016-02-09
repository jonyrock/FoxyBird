#pragma once

#include <cocos2d.h>

class HealthAsset : public cocos2d::Node {

public:
    CREATE_FUNC(HealthAsset);
    void setHealth(float h);
    
private:
    
    /* STATE */
    cocos2d::Size _size;
    float _targetLineWidth;
    
    /* ELEMENTS */
    cocos2d::LayerColor* _back;
    cocos2d::LayerColor* _line;
    
    /* INITS */
    bool init() override;
    void initBack();
    void initLine();
    void initIcon();
    
};