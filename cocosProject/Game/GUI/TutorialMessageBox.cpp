#include "TutorialMessageBox.h"
#include "Utils.h"
#include "Managers/SoundsManager.h"
#include "GUI.h"
#include "cocos/ui/UIButton.h"


using namespace cocos2d;


bool TutorialMessageBox::init() {
    LayerColor::initWithColor(Color4B(0,0,0,200));
    initTitle();
    initSubTitle();
    initScene();
    initCheckbox();
    initPlay();
    initOverlayListener();
    return true;
}

void TutorialMessageBox::initTitle() {
    auto t = GUI::createLabelB("HOW TO PLAY", 50);
    t->setColor(Color3B::WHITE);
    t->setAnchorPoint({0.5,0.8});
    t->setPosition(Utils::coordFromPSD({1136/2, 70}));
    addChild(t);
}


void TutorialMessageBox::initSubTitle() {
    auto t2 = GUI::createLabelR("You're the fox. Remember how the \n bird jumps over pieces of ice.", 20, TextHAlignment::CENTER);
    t2->setColor(Color3B::WHITE);
    t2->setAnchorPoint({0.5,1});
    t2->setPosition(Utils::coordFromPSD({1136/2,150}));
    addChild(t2);
}

void TutorialMessageBox::initScene() {
    auto s = Sprite::create("tutorial_box_scene.png");
    s->setPosition(Utils::coordFromPSD({1136/2,253}));
    s->setAnchorPoint({0.5,1});
    Utils::setNodeHeightR(s,135.0f/640);
    addChild(s);
}

void TutorialMessageBox::initCheckbox() {
    auto checkBoxNode = Node::create();
    auto cb = GUI::SwapButton::create("checkbox.png", "checkbox_active.png");
    cb->setOnChange([](bool value) {
	SettingsManager::getInstance()->setIsTutorial(value);
    });
    cb->setAnchorPoint({-0.5, 0.5});
    Utils::setNodeHeightR(cb, 36.f/640);
    
    auto t = GUI::createLabelR("Don't show it again", 24);
    t->setColor(Color3B::WHITE);
    t->setAnchorPoint({1, 1});

    checkBoxNode->addChild(t);
    checkBoxNode->addChild(cb);
    checkBoxNode->setContentSize(Size(cb->getContentSize().width * cb->getScale() * 1.5 + t->getContentSize().width, t->getContentSize().height));
    checkBoxNode->setPosition(Utils::coordFromPSD({1136/2, 430}));
    
    cb->setPosition({-checkBoxNode->getContentSize().width/2, 0});
    t->setPosition({checkBoxNode->getContentSize().width/2, 0});
    addChild(checkBoxNode);
    
    /* INVISIBLE BUTTON */
    auto btn = ui::Button::create();
    btn->setScale9Enabled(true);
    btn->setContentSize(t->getContentSize());
    btn->setPosition(t->getPosition());
    btn->setAnchorPoint(t->getAnchorPoint());
    checkBoxNode->addChild(btn);
    
    btn->addTouchEventListener([cb](Ref*,ui::Widget::TouchEventType type){
        if(type!=ui::Widget::TouchEventType::ENDED) return;
        cb->triggerChange();
    });
    
}

void TutorialMessageBox::initPlay() {
    GUI::TextButton* b = GUI::TextButton::create(Size(447, 88), "OK", [this]() {
	// it's a hack, otherwise SIGFAULT
	this->runAction(Sequence::create(
	    FadeOut::create(0.2),
	    CallFunc::create([this](){ 
                this->removeFromParent(); 
                if(_onClose) _onClose();
            }),
	    nullptr
	));
    });
    Utils::setNodeHeightR(b, 88.0f/640);
    b->setAnchorPoint({0.5,1});
    b->setPosition(Utils::coordFromPSD({1136/2,492}));
    addChild(b);
}

void TutorialMessageBox::initOverlayListener() {
    auto l = EventListenerTouchOneByOne::create();
    l->onTouchBegan = [this](Touch*, Event*) { return true; };
    l->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l, this);
}

void TutorialMessageBox::setOnClose(const std::function<void()>& fun) {
    _onClose = fun;
}



