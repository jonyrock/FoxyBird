#pragma once

#include "PlatformModel.h"
#include <cocos2d.h>
#include <list>
#include <vector>
#include "Utils.h"

// TODO: get constat points to player and bird models
struct FieldModel {
    cocos2d::Size size;
    float yScale;
    std::vector<PlatformModel*> platforms;
    std::vector<cocos2d::Vec2> topIcePoints;
    std::list<int> moves; // TODO: get only const acces to it. Other operatoins throuht special funcs
    cocos2d::Vec2 birdPosition;
    bool birdJumping;
    float birdJumpProgress;
    int playerPos;
    float birdSpeed;
    float foxPower = 0.2;
    size_t jumpNumber = 0; // TODO: maybe I can use tutorial move insted. Howw do the arrow dissapear?
    
    inline bool isTutotialFinished() const {
        return jumpNumber >= 3;
    }
    
    cocos2d::Color4F colorSky1;
    cocos2d::Color4F colorSky2;
    cocos2d::Color4F colorMountains;
    cocos2d::Color4F colorIceTop;
    cocos2d::Color4F colorWater1;
    cocos2d::Color4F colorWater2;
    cocos2d::Color4F colorMountainsLight;

    static constexpr float SOURCE_YSCALE = 0.95f;
    static constexpr float TARGET_YSCALE = 0.754f;
    
    inline float partToTargetScale() const {
	CCASSERT(yScale >= TARGET_YSCALE, "yScale should be bigger than TARGET_YSCALE");
	return 1 - (yScale - TARGET_YSCALE) / (SOURCE_YSCALE - TARGET_YSCALE);
    }
    
    inline void setPartToTargetScale(float part) {
	CCASSERT(part <= 1, "Part should be at most 1");
	CCASSERT(part >= 0, "Part should be at least 0");
	if(part >= 1) {
	    yScale = TARGET_YSCALE;
	    return;
	}
	yScale = SOURCE_YSCALE - (SOURCE_YSCALE - TARGET_YSCALE) * part;
    }
    
    inline cocos2d::Vec2 from3dToPseudoPos(const cocos2d::Vec2& pos) {
	return cocos2d::Vec2(pos.x, pos.y * yScale);
    }
    
    inline void setColorIfHSV(float h, float s, float v) {
        if(v < 0) v = 0;
	auto p = [h,s,v](float a, float b, float c) { return Utils::colorFromHSV(h*a, s*b, v+c); };
	colorSky2	= p(1, 1, 0);
	colorSky1	= p(0.93, 1.07, 3.5);
	colorMountains	= p(213/219.0, 1, -18);
        colorIceTop     = Utils::colorFromHSV(205, 89 * (v / 78.0), 80 - 50 * ((78 - v)/ 78.0) * ((78 - v)/ 78.0));
	colorWater1	= p(1, 1.07, -40);
        colorWater2	= p(213/219.0, 1.07, 2);
        colorMountainsLight = Utils::colorFromHSV(176, 66 * (v / 78.0) * (v / 78.0), 100);
    }
    
};