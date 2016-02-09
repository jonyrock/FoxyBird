#pragma once

#include <cocos2d.h>

class MessagesLayer : public cocos2d::Node {
    
public:
    CREATE_FUNC(MessagesLayer);
    bool init() override;
    
    void showBeginningMessage();
    void hideBeginningMessage();
    void showSuccessMessage(int i);
    void showLevel(int num);
    
    void setScore(int score);
    void setScoreMultNone();
    
    void showHint();
    

private:
    /* STATE */
    cocos2d::Size _size;
    
    /* ELEMENTS */
    cocos2d::Label* _label;
    cocos2d::Label* _scoreLabel;
    cocos2d::Label* _scoreMultLabel;
    cocos2d::Label* _beginningMessage1 = nullptr;
    cocos2d::Label* _beginningMessage2 = nullptr;
    cocos2d::Label* _beginningMessage3 = nullptr;
    
    /* INITS */
    void initLabel();
    void initScore();
    
    static cocos2d::Label* createLabel(
	const std::string& text, cocos2d::Color3B color, float size
    );
    
    
};