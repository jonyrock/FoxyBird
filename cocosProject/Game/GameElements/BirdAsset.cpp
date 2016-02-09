#include "BirdAsset.h"

#include "Utils.h"
#include "Config.h"
#include <cmath>
#include <utility>


using namespace cocos2d;
using namespace std;


struct BirdAsset::MySprite3d : public Sprite3D {
    
    inline static MySprite3d* create(const std::string &modelPath) {
	if (modelPath.length() < 4) {
	    CCASSERT(false, "improper name specified when creating Sprite3D");
        }
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
        rotated = transform * rotated;
        Sprite3D::draw(renderer, rotated, flags);
    }
    
    void setYRotation(float a) {
        a -= 2 * M_PI * (int)(a / (2 * M_PI));
        if(a < 0) {
            a += 2 * M_PI;
        }
        _yRotation = a;
    }
    
    float getYRotation() { return _yRotation; }
    
    
private:
    float _yRotation;
    inline MySprite3d():_yRotation(0) { }
    
};

bool BirdAsset::init() {
    if(!Node::init()) {
        return false;
    }
    initSprites();
    onNextStage();
    setContentSize(Size(200, 200));
    return true;
}

bool BirdAsset::initSprites() {

    _birdPathes.push_back({"bird1.obj", "bird1.png"});
    _birdPathes.push_back({"bird2.obj", "bird2.jpg"});
    _birdPathes.push_back({"bird3.obj", "bird3.jpg"});
    _birdPathes.push_back({"bird6.obj", "bird6.png"});
    _birdPathes.push_back({"bird4.obj", "bird4.jpg"});
    _birdPathes.push_back({"bird5.obj", "bird5.png"});

    for(int i = 0; i < _birdPathes.size(); i++) {
        _sprites.push_back(nullptr);
        if(!Config::Game::BIRD_LAZY_LOAD) {
            getSprite(i);
        }
    }
    
    return true;
}

BirdAsset::MySprite3d* BirdAsset::getSprite(size_t index) {
    if(_sprites.at(index) == nullptr) {
        auto sprite = MySprite3d::create(
            _birdPathes.at(index).first
        );
        if(!sprite) {
            return nullptr;
        }
        
        sprite->setTexture(_birdPathes.at(index).second);
        sprite->setRotation3D(Vec3(90,0,0));
        sprite->setVisible(false);
        sprite->setCameraMask(getCameraMask());
        addChild(sprite);
        _sprites.at(index) = sprite;
    }
    return _sprites.at(index);
}

void BirdAsset::setRotationInRadians(float angle) {
    if(angle > 180) angle -= 360;
    if(angle < -180) angle += 360;
    angle = M_PI * angle / 180;
    _activeSprite->setYRotation(angle);
}

float BirdAsset::getRotationInDegrees() const {
    return 180 * _activeSprite->getYRotation() / M_PI;
}

void BirdAsset::addRotation(float angle) {
    angle = M_PI * angle / 180;
    _activeSprite->setYRotation(_activeSprite->getYRotation() + angle);
}

float BirdAsset::getRotation() const {
    float a = _activeSprite->getYRotation();
    a = a * 180 / M_PI;
    return a;
}

void BirdAsset::onNextStage() {
    if(_activeSprite) {
        _activeSprite->setVisible(false);
    }
    _activeSprite = getSprite(_nextSpriteIndex);
    _nextSpriteIndex++;
    _nextSpriteIndex %= _sprites.size();
    
    //auto c = Color3B(Utils::colorFromHSV(rand() % 360, 90, 90));
    //_activeSprite->setColor(c);
    _activeSprite->setVisible(true);
    setScale(getScale() * 0.985);
}

void BirdAsset::blink(function<void()> callback) {
    auto makeBlink = [this](float t) {
        return Sequence::create(
            CallFunc::create([this]() {
                setVisible(false); 
            }),
            DelayTime::create(t),
            CallFunc::create([this]() { 
                setVisible(true); 
            }), DelayTime::create(t), 
            nullptr
        );
    };
    runAction(Sequence::create(
        makeBlink(0.2), makeBlink(0.15), makeBlink(0.1), makeBlink(0.1), makeBlink(0.1), 
        CallFunc::create([this, callback](){ callback(); }), nullptr
    ));
}





