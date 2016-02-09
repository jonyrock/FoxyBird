
#include "2d/CCLight.h"

#include "TestScene.h"

#include "Utils.h"
#include "GUI/GUI.h"

using namespace cocos2d; 

#define STATIC_SPRITE_TAG 10

// TODO: contribute bug circe stencil->drawDot(Vec2(0,0), 200, Color4F::WHITE);
//       which looks like a rectange

bool TestScene::init() {
    if(!Scene::initWithPhysics()) return false;
    
    _size = Director::getInstance()->getVisibleSize();
    
    initButtonTest();
    return true;
    
    initHSLTest();
    return true;
    
    initMessagesTest();
    return true;
    
    initShadingTest();
    return true;
    
    initCollisionTest();
    return true;
    
    initLight3DTest();
    return true;
    
    initAnimation3DTest();
    return true;
    
    initClippingTest();
    //init3dTest();
    initClipingTestCamera(); // uncomment this line to get clipped sprite
    return true;
    
}

void TestScene::initButtonTest() {
    auto b = GUI::TextButton::create(_size / 6, "Text", [](){});
    
    b->setPosition(_size/2);
    addChild(b);
}

void TestScene::initHSLTest() {
    Color4B clr(Utils::colorFromHSV(121,84,70));
    auto layer = LayerColor::create(clr);
    addChild(layer);
}


void TestScene::initMessagesTest() {
    // 1136x640
    auto myC = [this](Vec2 pos){
	return Vec2(
	    _size.width  * pos.x/1136,
	    _size.height * (1 - pos.y/640)
	);
    };
    float durationIn = 1;
    float durationMiddle = 0.8;
    float durationOut = 0.2;
    auto sprite = Sprite::create("message_perfect.png");
    
    // ROTATION
    sprite->runAction(Sequence::create(
	EaseOut::create(RotateBy::create(durationIn * 0.25, -4), 2), 
	EaseIn::create(RotateTo::create(durationIn * 0.25, 0), 2), 
	nullptr
    ));
    
    // OPACITY
    sprite->setOpacity(0);
    sprite->runAction(Sequence::create(
	FadeTo::create(durationIn * 0.5, 255), 
	DelayTime::create(durationIn * 0.5 + durationMiddle),
	FadeTo::create(durationOut, 0), 
	nullptr
    ));
    
    // POSITION
    sprite->setPosition(myC({560, 185}));
    sprite->runAction(Sequence::create(
	MoveBy::create(durationIn * 0.5, myC({560, 125}) - myC({560, 185})),
	DelayTime::create(durationIn * 0.5 + durationMiddle),
	MoveBy::create(durationOut,  myC({560, 85}) - myC({560, 125})),
	nullptr
    ));
    
    // SCALE
    sprite->setScale(0.9);
    sprite->runAction(Sequence::create(
	EaseOut::create(ScaleTo::create(durationIn * 0.5, 1.2), 2), 
	EaseOut::create(ScaleTo::create(durationIn * 0.3, 0.9), 2), 
	EaseBackInOut::create(ScaleTo::create(durationIn * 0.2, 1)), 
	nullptr
    ));
    
//    sprite->runAction(Sequence::create(
//	MoveTo::create(1, {100, 100}),
//	MoveTo::create(1, {100, 100}),
//	nullptr
//    ));
    addChild(sprite, 1);
}


void TestScene::initShadingTest() {
    
//    auto sprite = Sprite::create("tweet_twitter_bird_gold_2-1111px.png");
//    addChild(sprite);
//    sprite->setPosition(_size/3);
//    graySprite(sprite);
    
    auto sprite = Sprite::create("heart-icon.png");
    addChild(sprite);
    sprite->setPosition(_size / 2);
    genericSprite(sprite);
    
    
}

void TestScene::graySprite(Sprite* sprite) {
    if(sprite) {
	GLProgram * p = new GLProgram();
        p->initWithFilenames("gray.vsh", "gray.fsh");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,   GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR,	    GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD,  GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        sprite->setGLProgram(p);
    }
}

void TestScene::genericSprite(cocos2d::Sprite* s) {
    auto p = GLProgram::createWithFilenames("generic.vsh", "generic.fsh");
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    p->link();
    p->updateUniforms();
    s->setGLProgram(p);
}




void TestScene::initCollisionTest() {
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
//    auto edgeBody = PhysicsBody::createEdgeBox(_size, PHYSICSBODY_MATERIAL_DEFAULT, 10);
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(_size/2);
//    edgeNode->setPhysicsBody(edgeBody);
    //addChild(edgeNode);
    auto c1 = Sprite::create("circle.png");
    c1->setTag(STATIC_SPRITE_TAG);
    c1->setPhysicsBody(PhysicsBody::createCircle(25));
    c1->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    addChild(c1, 1);
    c1->setPosition(_size/2);
    c1->getPhysicsBody()->setDynamic(false);
    
    auto c2 = Sprite::create("circle.png");
    c2->setPhysicsBody(PhysicsBody::createCircle(25));
    addChild(c2, 1);
    c2->setPosition(Vec2(_size/2) + Vec2(2, 100));
    c2->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    
    
    //adds contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [](PhysicsContact& contact) {
	CCLOG("Contact begin");
	return true;
    };
    CCLOG("app begin");
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
      this);
    
}

void TestScene::initLight3DTest() {
    // init model
    //std::string fileName = "L200-FBX.c3b";
    std::string fileName = "fox_obj_08_12.obj";
    auto sprite = Sprite3D::create(fileName);
    
    sprite->setTexture("fox_map.png");
    sprite->setScale(1);
    sprite->setRotation3D(Vec3(10,10,0));
    addChild(sprite);
    sprite->setPosition( Vec2(_size / 2) - Vec2(0, 70));
    
    sprite->runAction(RepeatForever::create(
	RotateBy::create(0.01, Vec3(0,1,0))
    ));
    
    // init light
    auto light = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B::WHITE);
    addChild (light);
    
    auto light2 = AmbientLight::create(Color3B::WHITE);
    light2->setIntensity(0.4);
    sprite->addChild(light2);
    
}

void TestScene::initAnimation3DTest() {
    std::string fileName = "Karen.c3b";
    auto sprite = Sprite3D::create(fileName);
    //sprite->setTexture("truck.png");
    //sprite->setScale(15);
    sprite->setRotation3D(Vec3(45,45,0));
    addChild(sprite);
    sprite->setPosition( _size / 2 );
    
    sprite->runAction(RepeatForever::create(
	RotateBy::create(0.01, Vec3(0,1,0))
    ));
    auto animation = Animation3D::create(fileName);
    if (animation)
    {
        auto animate = Animate3D::create(animation);
        bool inverse = (std::rand() % 3 == 0);

        int rand2 = std::rand();
        float speed = 1.0f;
        if(rand2 % 3 == 1)
        {
            speed = animate->getSpeed() + CCRANDOM_0_1();
        }
        else if(rand2 % 3 == 2)
        {
            speed = animate->getSpeed() - 0.5 * CCRANDOM_0_1();
        }
        animate->setSpeed(inverse ? -speed : speed);

        sprite->runAction(RepeatForever::create(animate));
    }
}

void TestScene::initClippingTest() {
    auto whiteBack = LayerColor::create(Color4B::WHITE);
    addChild(whiteBack);
    auto cl = ClippingNode::create();
    cl->setContentSize(Size(1000, 1000));
    auto stencil = DrawNode::create();
    stencil->drawDot(Vec2(0,0), 700, Color4F::WHITE); // which looks like a rect btw 
    cl->setStencil(stencil);
    auto sp = Sprite::create("shadow_corner.png");
    sp->setAnchorPoint(Vec2(1,1));
    sp->setColor(Color3B::BLACK);
    cl->setPosition(_size/2);
    cl->addChild(sp);
    addChild(cl, 20);
    auto back = DrawNode::create();
    back->drawRect(Vec2(-100, -100), Vec2(100, 100), Color4F::MAGENTA);
    addChild(back);
    back->setPosition(_size / 2);
    stencil->setCameraMask((unsigned short) CameraFlag::USER2);
}

void TestScene::initClipingTestCamera() {
    // like default camera
    float zeye = Director::getInstance()->getZEye();
    auto cameraRegular = Camera::createPerspective(
	60, _size.width/_size.height, 10, zeye + _size.height / 2.0f);
    Vec3 eye2(_size.width/2, _size.height/2, zeye), 
	 center(_size.width/2, _size.height/2, 0.0f),
	 up(0.0f, 1.0f, 0.0f);
    cameraRegular->setPosition3D(eye2);
    cameraRegular->lookAt(center, up);
    cameraRegular->setCameraFlag(CameraFlag::USER2);
    addChild(cameraRegular, 1);
    setCameraMask((unsigned short)CameraFlag::USER2);
}

void TestScene::initClippingTestBackground() {
    
}

void TestScene::init3DTest() {
    // init sprite
    auto sprite = Sprite3D::create("boss.obj");
    sprite->setTexture("boss.png");
    sprite->setPosition(_size/2);
    sprite->setScale(10);
    sprite->setRotation3D(Vec3(45,0,0));
    addChild(sprite, 1);
    
    // init camera
    Vec3 eye(0,0,100);
    auto camera = Camera::createOrthographic(
	_size.width, _size.height, 1, 2147483647
	// replace 2147483647 to 1000 to avoid the bug
    );
    camera->setCameraFlag(CameraFlag::USER1);
    camera->setPosition3D(eye);
    camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
    addChild(camera, 1);
    
    sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    
}

