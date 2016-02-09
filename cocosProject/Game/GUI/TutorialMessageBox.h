#pragma once

#include <cocos2d.h>
#include <functional>


class TutorialMessageBox : public cocos2d::LayerColor {
public:
    CREATE_FUNC(TutorialMessageBox);
    void setOnClose(const std::function<void()>&);
private:
    /* INITS */
    bool init() override;
    void initTitle();
    void initSubTitle();
    void initScene();
    void initPlay();
    void initCheckbox();
    void initOverlayListener();
    std::function<void()> _onClose;
} ;



