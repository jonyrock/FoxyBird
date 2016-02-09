#pragma once

#include <cocos2d.h>
#include <string>
#include <functional>


class ErrorMessageBox : public cocos2d::LayerColor {

public:
    CREATE_FUNC(ErrorMessageBox);
    void setTitle(const std::string& txt);
    void setSubTitle(const std::string& txt);
    void setOnOk(std::function<void()> fun);
    
private:
    /* DATA */
    cocos2d::Size _size;
    std::function<void()> _onOkCallback;
    
    /* INITS */
    bool init() override;
    void initGUI();
    
};


