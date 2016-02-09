#pragma once

#include <cocos/ui/UIButton.h>
#include <cocos2d.h>

#include <string>
#include <functional>

namespace GUI {
    
    struct SwapButton : cocos2d::Node {
	
	static SwapButton* create(const std::string& icon1, const std::string& icon2);
	
	void setSwitched(bool enabled);
        bool isEnabled() const;
	void setOnChange(const std::function<void(bool)>& fun);
        void triggerChange();
	
    protected:
	/* STATE */
	std::function<void(bool)> _onChangeValue;
	
	/* ELEMENTS */
	cocos2d::ui::Button* _buttonOn;
	cocos2d::ui::Button* _buttonOff;
        
        /* INITS */
        bool init(const std::string& icon1, const std::string& icon2);

    };
    
    class TextButton : public cocos2d::ui::Button {
    public:
	static TextButton* create(
	    const cocos2d::Size& size, const std::string& text, 
	    std::function<void()> callback,
            cocos2d::Color3B c1 = cocos2d::Color3B::WHITE, 
            cocos2d::Color3B c2 = cocos2d::Color3B::WHITE, 
            cocos2d::Color3B c3 = cocos2d::Color3B::WHITE, 
            cocos2d::Color3B c4 = cocos2d::Color3B::BLACK,
            float textSize = 40
	);
    protected:
        
        /* STATE */
        bool _inited = false;
 	std::function<void()> _onClickCallback = nullptr;
        cocos2d::Color3B c3;
        cocos2d::Color3B c4;

        
        /* INITS */ 
	/*
	 \param size is the size in the maket dimensions
	 */
	bool init(
	    const cocos2d::Size& size, const std::string& text, 
	    std::function<void()> callback,
            cocos2d::Color3B c1, cocos2d::Color3B c2, cocos2d::Color3B c3, cocos2d::Color3B c4,
            float textSize
	);
        
        /* ELEMENTS */
        cocos2d::Label* _text;
        
        /* OVERRIDES */
	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;
	
	inline virtual void releaseUpEvent() override {}

    };
    
    
    cocos2d::Label* createLabelR(const std::string& text, float size, 
	cocos2d::TextHAlignment hAlign = cocos2d::TextHAlignment::LEFT);
    
    cocos2d::Label* createLabelB(const std::string& text, float size, 
	cocos2d::TextHAlignment hAlign = cocos2d::TextHAlignment::LEFT);
    
    cocos2d::Label* createLabelI(const std::string& text, float size, 
	cocos2d::TextHAlignment hAlign = cocos2d::TextHAlignment::LEFT);
    
    cocos2d::Label* createLabelBI(const std::string& text, float size, 
	cocos2d::TextHAlignment hAlign = cocos2d::TextHAlignment::LEFT);
    
};