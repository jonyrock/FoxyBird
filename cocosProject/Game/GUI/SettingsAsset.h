#pragma once

#include <cocos2d.h>

class SettingsAsset : public cocos2d::Node {

public:
    CREATE_FUNC(SettingsAsset);
    void close();
private:
    
    /* DATA */
    const int ELEMENTS_NUMBER = 3;
    
    /* ELEMENTS */
    cocos2d::Node* _back;
    
    /* INITS */
    bool init() override;
    void initBack();
    void initIcons();
    
    cocos2d::Size _size;
    
};