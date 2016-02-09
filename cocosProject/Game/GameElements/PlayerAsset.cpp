#include "PlayerAsset.h"
#include <string>

using namespace cocos2d;

// TODO: use cocos macros for radians conversion

// TODO: use standart cocos rotations
struct PlayerAsset::MySprite3d : public Sprite3D {
    
    inline static MySprite3d* create(const std::string &modelPath) {
	if (modelPath.length() < 4)
	    CCASSERT(false, "improper name specified when creating Sprite3D");
	auto sprite = new (std::nothrow) MySprite3d();
	if (sprite && sprite->initWithFile(modelPath)) {
	    sprite->_contentSize = sprite->getBoundingBox().size;
	    sprite->autorelease();
	    return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
    }
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override {
	Mat4 rotated;
	Mat4::createRotationY(_yRotation, &rotated);
	rotated = transform*rotated;
	Sprite3D::draw(renderer, rotated, flags);
    }
    
    void setYRotation(float a) {
	a -= 2 * 3.141592 * ((int)(a / (2 * 3.141592)));
	if(a < 0) a += 2 * 3.141592;
	_yRotation = a;
    }
    
    float getYRotation() { return _yRotation; }
    
    
private:
    float _yRotation;
    inline MySprite3d():_yRotation(0) { }
    
};

bool PlayerAsset::init() {
    if(!Node::init()) return false;
    _sprite = MySprite3d::create("fox.obj");
    _sprite->setTexture("fox_map.png");
    _sprite->setRotation3D({90, 0, 0});
    addChild(_sprite);
    setContentSize({300, 300});
    return true;
}

void PlayerAsset::setRotationInRadians(float angle) {
    if(angle > 180) angle -= 360;
    if(angle < -180) angle += 360;
    angle = 3.141592 * angle / 180;
    _sprite->setYRotation(angle);
}

float PlayerAsset::getRotationInDegrees() const {
    return 180 * _sprite->getYRotation() / 3.141592;
}

void PlayerAsset::addRotation(float angle) {
    angle = 3.141592 * angle / 180;
    _sprite->setYRotation(_sprite->getYRotation() + angle);
}

float PlayerAsset::getRotation() const {
    float a = _sprite->getYRotation();
    a = a * 180 / 3.141592;
    return a;
}






