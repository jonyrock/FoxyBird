#pragma once

#include <cocos2d.h>

class StageProgressAsset : public cocos2d::Node {

public:
    CREATE_FUNC(StageProgressAsset);
    void setProgress(float h);
    void onNextStage();
    
private:
    
    /* STATE */
    cocos2d::Size _size;
    float _targetLineWidth;
    
    /* ELEMENTS */
    cocos2d::LayerColor* _back;
    cocos2d::LayerColor* _line;
    cocos2d::Sprite*	 _icon;
    
    /* INITS */
    bool init() override;
    void initBack();
    void initLine();
    void initIcon();

};