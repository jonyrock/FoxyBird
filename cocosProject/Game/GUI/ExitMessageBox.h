#pragma once

#include <cocos2d.h>
#include "GUI/GUI.h"
#include <functional>


class ExitMessageBox : public cocos2d::LayerColor {

public:
    CREATE_FUNC(ExitMessageBox);
    inline void setOnNo(std::function<void()> fun) {
	_onNo = fun;
    }
    void close();
    
private:
    /* DATA */
    cocos2d::Size _size;
    std::function<void()> _onNo;
    
    /* INITS */
    bool init() override;
    void initGUI();
    
};


