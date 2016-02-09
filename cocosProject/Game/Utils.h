#pragma once

#include "PlatformMacros.h"

#include <cocos2d.h>

#include <sstream>
#include <string>

#define DEBUG_PREFIX "FoxyDebug"

// TODO: move to a special directory and split into modules
namespace Utils {

    template<class T>
    inline std::string toString(T obj) {
        std::stringstream str;
        str << obj;
        return str.str();
    }
    
    void drawRect(
	cocos2d::DrawNode* node, 
	const cocos2d::Rect& rect,
	const cocos2d::Color4F &fillColor,
	float borderWidth = 0,
	const cocos2d::Color4F &borderColor = cocos2d::Color4F::BLACK
    );
    
    void  setNodeWidth(cocos2d::Node* node, float width);
    float setNodeWidthR(cocos2d::Node* node, float relativeWidth);
    void  setNodeHeight(cocos2d::Node* node, float height);
    float setNodeHeightR(cocos2d::Node* node, float relativeHeight);
    
    // relative to parent in percent
    void setPosition(cocos2d::Node* node, float x, float y);
    inline void setPosition(cocos2d::Node* node, cocos2d::Vec2 pos) {
        setPosition(node, pos.x, pos.y);
    }
    inline void setPosition(cocos2d::Node* node, float v) {
        setPosition(node, v, v);
    }
    
    std::string getFileData(const std::string& path);
    
    float getFontSize(float fs);
    
    unsigned long getTimeTick();
    
    void exit();
    
    PLATFORM_DEPENDENT void openMarket();
    PLATFORM_DEPENDENT void openUrl(const char* url);
    
    cocos2d::Color3B colorFromString(const std::string& str);
    
    inline cocos2d::Vec2 coordFromPSD(cocos2d::Vec2 pos) {
	auto _size = cocos2d::Director::getInstance()->getVisibleSize();
	if(pos.x < 0) pos.x += 1136;
	if(pos.y < 0) pos.y += 640;
	return cocos2d::Vec2(
	    _size.width  * pos.x/1136,
	    _size.height * (1 - pos.y/640)
	);
    };
    
    /**
     * Computes color from hsv
     * see http://colorizer.org/ for more info
     * @param  float h	The hue in [0, 360]
     * @param  float s	The situation in [0, 100]
     * @param  float v	The value in [0, 100]
     * @return color4F	The color in RGB boxed in Color4F
     */
    cocos2d::Color4F colorFromHSV(float h, float s, float v);
    
}






