#pragma once

#include <cocos2d.h>
#include "GUI/GUI.h"
#include <functional>

class PauseMessageBox : public cocos2d::LayerColor {

public:
    CREATE_FUNC(PauseMessageBox);
    void setOnContinue(std::function<void()> fun);
    
private:
    /* DATA */
    cocos2d::Size _size;
    std::function<void()> _onContinue;
    
    /* INITS */
    bool init() override;
    void initGUI();
    
};


