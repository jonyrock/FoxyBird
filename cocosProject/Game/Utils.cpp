#include "Utils.h"

#include "Managers/SettingsManager.h"

#include <time.h>
#include <ctype.h>


using namespace std;
using namespace cocos2d;


string Utils::getFileData(const string& path) {
    
    // TODO: think how to make it faster
    // TODO: checkout CCFileUtils::getStringFromFile()
    
    ssize_t fileSize = 0;
    unsigned char * fileContents = NULL;
    string contents;

    fileContents = FileUtils::getInstance()->getFileData(path.c_str() , "r", &fileSize);
    
    if(!fileContents) {
	CCLOG("Can't open level %s", path.c_str());
	assert(false);
    }

    contents.append(string((char *) fileContents), 0, fileSize);
    
    if (fileContents) {
	delete [] fileContents;
	fileContents = NULL;
    }
    
    return contents;
    
}

void Utils::drawRect(
    DrawNode* node, 
    const Rect& rect, 
    const Color4F &fillColor, 
    float borderWidth,
    const Color4F &borderColor
) {
    Vec2 vs[] = {
	rect.origin,
	rect.origin + Vec2(rect.size.width, 0),
	rect.origin + rect.size,
	rect.origin + Vec2(0, rect.size.height)
    };
    node->drawPolygon(vs, 4, fillColor, borderWidth, borderColor);
}


float Utils::getFontSize(float fs) {
    const float fontSizeToHeight = 1.44;
    const float desightHeight = 640;
    return Director::getInstance()->getVisibleSize().height * (fs * fontSizeToHeight) / desightHeight;
}

void Utils::setNodeWidth(cocos2d::Node* node, float width) {
    auto sf = width / node->getContentSize().width;
    node->setScale(sf);
}

float Utils::setNodeWidthR(cocos2d::Node* sprite, float swidth) {
    CCASSERT(sprite->getContentSize().height > 0, "Node must have non-zero width");
    float parentWidth = Director::getInstance()->getWinSize().width;
    if(sprite->getParent()) {
	parentWidth = sprite->getParent()->getContentSize().width;
    }
    auto sf = (parentWidth * swidth) / sprite->getContentSize().width;
    sprite->setScale(sf);
    return sf;
}

void Utils::setNodeHeight(cocos2d::Node* node, float height) {
    auto sf = height / node->getContentSize().height;
    node->setScale(sf);
}

float Utils::setNodeHeightR(cocos2d::Node* sprite, float sheight) {
    CCASSERT(sprite->getContentSize().height > 0, "Node must have non-zero height");
    float parentHeight = Director::getInstance()->getWinSize().height;
    if(sprite->getParent()) {
	parentHeight = sprite->getParent()->getContentSize().height;
    }
    auto sf = (parentHeight * sheight) / sprite->getContentSize().height;
    sprite->setScale(sf);
    return sf;
}

void Utils::setPosition(cocos2d::Node* node, float x, float y) {
    Size size = SettingsManager::getInstance()->getWinSize();
    if(node->getParent() != nullptr) 
	size = node->getParent()->getContentSize();
    node->setPosition(size.width * x, size.height * y);
}


unsigned long Utils::getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return millisecs;
}

void Utils::exit() {
    Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
    #endif
}

cocos2d::Color3B Utils::colorFromString(const std::string& str) {
    CCASSERT(str.length() >= 6, "To short string. Need something like this #0f51c8");
    int cursor = 0;
    if(str.at(0) == '#') cursor++;
    auto consumeByte = [&cursor, &str]() {
	auto charToInt = [](char c) {
	    c = toupper(c);
	    if(c <= '9')
		return c - '0';
	    return 10 + (c - 'A');
	};
	char res = (charToInt(str.at(cursor)) << 4) + charToInt(str.at(cursor+1));
	cursor+=2;
	return res;
    };
    char r,g,b;
    r = consumeByte(); g = consumeByte(); b = consumeByte();
    // it's not the same as Color3B(consumeByte(), consumeByte(), consumeByte())
    return Color3B(r, g, b);
}


/**
 * took from
 * http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
 */
Color4F Utils::colorFromHSV(float h, float s, float v) {
    s /= 100.0;
    v /= 100.0;
    if(v < 0) v = 0;
    if(s < 0) s = 0;
    double hh, p, q, t, ff;
    long i;
    Color4F res;
    res.a = 1;

    if(s <= 0.0) {       // < is bogus, just shuts up warnings
        res.r = v;
        res.g = v;
        res.b = v;
        return res;
    }
    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
	case 0:
	    res.r = v;
	    res.g = t;
	    res.b = p;
	    break;
	case 1:
	    res.r = q;
	    res.g = v;
	    res.b = p;
	    break;
	case 2:
	    res.r = p;
	    res.g = v;
	    res.b = t;
	    break;
	case 3:
	    res.r = p;
	    res.g = q;
	    res.b = v;
	    break;
	case 4:
	    res.r = t;
	    res.g = p;
	    res.b = v;
	    break;
	case 5:
	default:
	    res.r = v;
	    res.g = p;
	    res.b = q;
	    break;
    }
    return res;     
}