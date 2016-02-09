#include "GameScene.h"

#include "Managers/ScenesManager.h"
#include "Managers/SettingsManager.h"

#include "GameElements/FieldAsset.h"
#include "GameElements/TutorialArrowAsset.h"

#include "GUI/GUI.h"
#include "GUI/ResultsLayer.h"
#include "GUI/SettingsAsset.h"

#include "Managers/SoundsManager.h"
#include "Managers/LivesManager.h"
#include "Managers/SettingsManager.h"
#include "Managers/AdvertisingManager.h"
#include "Managers/AnalyticsManager.h"

#include "Utils.h"

#include <cocos2d.h>
#include <random>
#include <algorithm>


using namespace cocos2d;
using namespace std;


const GameScene::hsv GameScene::COLOR_BEGIN = GameScene::hsv(219, 93, 78);


bool GameScene::init() {
    if(!BasicScene::initWithPhysics()) {
        return false;
    }
    initModels();
    
    initBackground();
    initPlatforms(); // TODO it is model init actually, wrong place
    initField();
    initPlayer();
    initBird();
    initHealth();
    initMessages();
    initTutorialBox();
    
    initPhysics();
    initLight();
    initCameras();
    
    //initDebug();
    //onGameOver();
    //showPauseLayer();

    return true;
}

/* INITS */

void GameScene::initModels() {
    _fieldModel = new FieldModel();
    _fieldModel->yScale = 1;
    _fieldModel->size = _size;
    _fieldModel->setColorIfHSV(
        get<0>(COLOR_BEGIN),
        get<1>(COLOR_BEGIN),
        get<2>(COLOR_BEGIN)
    );
    _currentColor = COLOR_BEGIN;
    generateTopIcePoints();
}

void GameScene::initBackground() {
    _background = BackgroudAsset::create(_fieldModel);
    addChild(_background, 2);
}

void GameScene::initPlatforms() {
    std::vector<Vec2> _platformPoss;
    fillRandomPlatformPositions(_platformPoss, 5);
    for(size_t i = 0; i < _platformPoss.size(); i++) {
        addPlatform(_platformPoss.at(i));
    }
    // see addPlatform()
}

void GameScene::initField() {
    _field = FieldAsset::create(_fieldModel);
    addChild(_field, 3);
}

void GameScene::initPlayer() {
    _player = PlayerAsset::create();
    // TODO: there is a copy of this in onPlayerRespawn()
    Utils::setNodeWidthR(_player, 0.2f);
    addChild(_player, 5);
    _positionsMask.at(1) = true;
    _playerModel.targetPos = 1;
    initShadows();
}

void GameScene::initBird() {
    _bird = BirdAsset::create();
    Utils::setNodeWidthR(_bird, 0.25f);
    addChild(_bird, 4);
    _fieldModel->birdSpeed = 2;
    onBirdAppear(true);
}

void GameScene::initShadows() {
    _playerShadow = Sprite::create("radgr.png");
    _birdShadow	  = Sprite::create("radgr.png");
    _playerShadow->setColor(Color3B::BLACK);
    _birdShadow	 ->setColor(Color3B::BLACK);
    addChild(_playerShadow, 3);
    addChild(_birdShadow,   3);
    Utils::setNodeWidthR(_playerShadow, 0.12);
    Utils::setNodeWidthR(_birdShadow,   0.09);
}

void GameScene::initPauseButton() {
    // see onGameActivated()
    _pauseButton = GUI::SwapButton::create("ic_pause.png", "ic_pause_active.png");
    Utils::setNodeWidthR(_pauseButton, 82.0f/1136);
    _pauseButton->setOnChange([this](bool v) {
        if(!v) {
            onGamePause();
        } else {
            onGameResume();
        }
    });
    _pauseButton->setPosition(Utils::coordFromPSD({70, 70}));
    _pauseButton->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    addChild(_pauseButton, 200);
}

void GameScene::initHealth() {
    _healthAsset = HealthAsset::create();
    _healthAsset->setPosition(Utils::coordFromPSD({140, 70}));
    addChild(_healthAsset, 4);
    _healthAsset->setHealth(_playerModel.health);
}

void GameScene::initTutorialBox() {
    if(!SettingsManager::getInstance()->getIsTutorial()) { 
        onTutorialBoxClose();
        return;
    }
    _tutorialMessageBox = TutorialMessageBox::create();
    _tutorialMessageBox->setOnClose([this]() {
        onTutorialBoxClose();
    });
    addChild(_tutorialMessageBox, 20);
}

void GameScene::initMessages() {
    _message = MessagesLayer::create();
    _message->setPosition(0, 0);
    addChild(_message, 5);
}

void GameScene::initTutorial() {
    _tutorialArrow = TutorialArrowAsset::create();
    auto targetPos = getPlatformPos(_fieldModel->moves.front());
    _tutorialArrow->setPosition(targetPos);
    _tutorialArrow->setVisible(true);
    addChild(_tutorialArrow, 8);
}

void GameScene::initTapToPlay() {
    _message->showBeginningMessage();
}

void GameScene::initPhysics() {
    //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    float borderOut = 200;
    PhysicsBody* edgeBody = PhysicsBody::createEdgeBox(_size + Size(borderOut * 2, borderOut * 2), PHYSICSBODY_MATERIAL_DEFAULT, borderOut + 10);
    _edgeNode = Node::create();
    _edgeNode->setPosition(_size/2);
    _edgeNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _edgeNode->setPhysicsBody(edgeBody);
    edgeBody->setContactTestBitmask(0xFFFFFFFF);

    getPhysicsWorld()->setGravity(Vec2(0,0));
    getPhysicsWorld()->setSpeed(PHYSICS_SPEED);
    addChild(_edgeNode, 50);
    
    // TOP ICE STATIC BODY
    vector<Vec2> pnts;
    pnts.resize(4);
    for(size_t i = 0; i + 1 < _fieldModel->topIcePoints.size(); i++) {
	pnts.at(0) = _fieldModel->topIcePoints.at(i);
	pnts.at(1) = _fieldModel->topIcePoints.at(i + 1);
	pnts.at(2).x = _fieldModel->topIcePoints.at(i + 1).x;
	pnts.at(2).y = _fieldModel->size.height;
	pnts.at(3).x = _fieldModel->topIcePoints.at(i).x;
	pnts.at(3).y = _fieldModel->size.height;
	auto topIceElementNode = Node::create();
	topIceElementNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto topIceElementBody = PhysicsBody::createEdgePolygon(
        pnts.data(), (int)pnts.size()
    );
	topIceElementBody->setContactTestBitmask(0xFFFFFFFF);
	topIceElementNode->setPhysicsBody(topIceElementBody);
	addChild(topIceElementNode);
    }
    
    // SOUNDS ON COLLISION
//    auto contactListener = EventListenerPhysicsContact::create();
//    contactListener->onContactBegin = [](PhysicsContact& contact) {
//	SoundsManager::getInstance()->playIceCollisionEffect();
//	CCLOG("Collistion");
//	return true;
//    };
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(
//	    contactListener, this);
    
}

void GameScene::initCameras() {
    /* DEFAULT CAMERA OVERRIDE */
    // this camera is absolute copy of Camera::initDefault()
    float zeye = Director::getInstance()->getZEye();
    auto cameraRegular = Camera::createPerspective(
        60, _size.width/_size.height, 10, zeye + _size.height / 2.0f
    );
    Vec3 eye2(_size.width/2, _size.height/2, zeye), 
         center(_size.width/2, _size.height/2, 0.0f), 
         up(0.0f, 1.0f, 0.0f);
    
    cameraRegular->setPosition3D(eye2);
    cameraRegular->lookAt(center, up);
    cameraRegular->setCameraFlag(CameraFlag::USER2);
    addChild(cameraRegular, 1);
    
    /* ORTHO CAMERA FOR REAL 3D MODELS */
    _camera = Camera::createOrthographic(_size.width, _size.height, -200, 50000);
    _camera->setCameraFlag(CameraFlag::USER1);
    addChild(_camera, 1);
    updateCameraPos();
    
    
    /* BIND CAMERAS */
    // TODO: find all gui elements by tag
    setCameraMask((unsigned short)CameraFlag::USER2);
    _player->setCameraMask((unsigned short)CameraFlag::USER1);
    _bird->setCameraMask((unsigned short)CameraFlag::USER1);
    _healthAsset->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    _message->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    if(_tutorialMessageBox) {
        _tutorialMessageBox->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    }
}

void GameScene::initLight() {
    auto light = DirectionLight::create(
        Vec3(-1.0f, 0.0f, -1.0f), 
        Color3B::WHITE
    );
    addChild(light);
    light->setIntensity(0.9);
    auto light2 = AmbientLight::create(Color3B::WHITE);
    light2->setIntensity(0.7);
    addChild(light2);
}

void GameScene::initDebug() {}

/* UPDATES */

void GameScene::update(float dt) {
    if(_isGamePaused) {
        return;
    }
    BasicScene::update(dt);
    if(_introIsActive) { 
        updateIntoAnimation(dt);
    }
    
    updateActivePlatforms(dt);
    updatePlayer(dt);
    updateBird(dt);
    updateShadows(dt);
    if(_isGameOver) {
        return;
    }
    updateError(dt);
    updateHealth(dt);
    
    updateTutorial(dt);
    updateStageColor(dt);
}

// TODO: move it to tools or something
Vec2 getBezierValue(Vec2 from, Vec2 control, Vec2 to, float t) {
    float x = powf(1 - t, 2) * from.x + 
              2.0f * (1 - t) * t * control.x + t * t * to.x;
    float y = powf(1 - t, 2) * from.y +
              2.0f * (1 - t) * t * control.y + t * t * to.y;
    return Vec2(x, y);
}

void GameScene::updateActivePlatforms(float dt) {
    vector<int> toRemoveIndeces;
    for(auto i: _alivePlatforms) {
        auto p = _fieldModel->platforms.at(i);
        float step = - 5 * 60 * dt; // cuz 60 frames per second
        p->setHealth(p->getHeath() + step);
        if(p->getHeath() < 0) {
            toRemoveIndeces.push_back(i);
            p->setHealth(0);
        }
    }
    for(auto i: toRemoveIndeces) {
        _alivePlatforms.erase(i);
    }
}

void GameScene::updateIntoAnimation(float dt) {
    _introPlayTime += dt;

    if(_introPlayTime > _introDuration) {
        _introPlayTime = _introDuration;
    }

    float t = _introPlayTime / _introDuration;
    t = tweenfunc::easeInOut(t, 3);
    _fieldModel->setPartToTargetScale(t);

    // update sky position
    _background->setPosition(
	_fieldModel->size.width / 2, 
	_fieldModel->size.height * _fieldModel->yScale 
	    + _background->getContentSize().height * _background->getScaleY() / 2
    );

    updateCameraPos();
    
    if(_introPlayTime == _introDuration) {
        _introIsActive = false;
        _fieldModel->setPartToTargetScale(1);
        return;
    }
    
}

// TODO: a lot of things happens here
void GameScene::updatePlayer(float dt) {
    // see tryToSetPlayerPosition() also
    
    // TODO: make some pre computations for jump (from and to) for this setPlayerPosition()
    
    // STATE: on platform
    if(!_playerModel.jumping || _playerModel.respawning) {
        _player->setPosition(getPlatformPosRaw(_playerModel.targetPos));
        float rotationNow = _fieldModel->platforms.at(_playerModel.targetPos)
                            ->getRotation();
        _player->addRotation(_playerModel.prevRotation - rotationNow);
        _playerModel.prevRotation = rotationNow;
	return;
    }
    
    auto from = getPlatformPosRaw(_playerModel.sourcePos);
    auto to = getPlatformPosRaw(_playerModel.targetPos);

    // STATE: rotation before jump
    if(!_playerModel.isRotated) {
        _player->setPosition(getPlatformPosRaw(_playerModel.sourcePos));
        _playerModel.rotationTime += dt;
        // rotation end
        if(_playerModel.rotationTime > _playerModel.timeToRotate) {
            _playerModel.isRotated = true;
            _player->setRotationInRadians(
                _playerModel.fromRotation + _playerModel.rotationArc
            );
            onPlayerStartJump();
            return;
        }
        auto t = _playerModel.rotationTime / _playerModel.timeToRotate;
        t = tweenfunc::easeIn(t, 3);
        _player->setRotationInRadians(
            _playerModel.fromRotation + t * _playerModel.rotationArc
        );
        return;
    }

    // STATE: jumping
    
    const float speedScale = 3.0f;
    float t = _playerModel.jumpingTime;
    Vec2 control = (from+to)/2 + Vec2(0, from.distance(to)/5);
    
    if(t > 1) {
        _player->setPosition(to);
        _playerModel.jumping = false;
        _playerModel.jumpingTime = 0;
        _playerModel.prevRotation = _fieldModel->platforms.at(_playerModel.targetPos)->getRotation();
        _playerModel.pseudoHight = 0;
        onPlayerFinishJump();
        return;
    }
    
    _player->setPosition(getBezierValue(from, control, to, t));
    _playerModel.pseudoHight = from.distance(to)/5 * (1-2*abs(t-0.5));
    _playerModel.jumpingTime += dt * speedScale;
}

void GameScene::updateShadows(float dt) {
    _playerShadow->setScaleY(_playerShadow->getScaleX() * _fieldModel->yScale);
    _birdShadow  ->setScaleY(_birdShadow->getScaleX() * _fieldModel->yScale);
    Vec2 posPlayer(_player->getPosition()); posPlayer.y *= _fieldModel->yScale;
    Vec2 posBird(_bird->getPosition()); posBird.y *= _fieldModel->yScale;
    _playerShadow->setPosition(posPlayer + Vec2(-1, 0) * _playerModel.pseudoHight * 1.5);
    _birdShadow->setPosition(posBird + Vec2(-1, 0) * _birdModel.pseudoHight * 1.5);
    int playerOpacity = 50 - 50 * tweenfunc::easeOut(_playerModel.pseudoHight / 30, 2);
    int birdOpacity = 50 - 50 * tweenfunc::easeOut(_birdModel.pseudoHight / 30, 2);
    if(playerOpacity < 0) {
        playerOpacity = 0;
    }
    if(birdOpacity < 0) {
        birdOpacity = 0;
    }
    _playerShadow->setOpacity(playerOpacity);
    _birdShadow->setOpacity(birdOpacity);
}

void GameScene::updateBird(float dt) {
    // TODO: store it only once
    _fieldModel->birdJumping = _birdModel.jumping;
    
    // STATE: ON PLATFROM
    if(!_birdModel.jumping) {
        // TODO: this functions are connected!
        _bird->setPosition(getPlatformPosRaw(_birdModel.targetPos));
        _fieldModel->birdPosition = _fieldModel->from3dToPseudoPos(_bird->getPosition());
        float rotationNow =  _fieldModel->platforms.at(_birdModel.targetPos)->getRotation();
        _bird->addRotation(_birdModel.fromRotation - rotationNow);
        _birdModel.fromRotation = rotationNow;
        return;
    }
    
    // STATE: JUMPING
    auto to = getPlatformPosRaw(_birdModel.targetPos);
    auto from = getPlatformPosRaw(_birdModel.sourcePos);
    Vec2 control = (to+from)/2 + Vec2(0, to.distance(from)/5);
    float t = _fieldModel->birdJumpProgress;
    if(t > 1) {
        _bird->setPosition(to);
        _bird->setRotationInRadians(
            _birdModel.fromRotation + _birdModel.rotationArc
        );
        _birdModel.fromRotation = _fieldModel->platforms.at(_birdModel.targetPos)->getRotation();
        _birdModel.jumping = false;
        _fieldModel->birdJumpProgress = 1;
        _fieldModel->birdPosition = _fieldModel->from3dToPseudoPos(_bird->getPosition());
            onBirdMoved();
        return;
    }
    _bird->setPosition(getBezierValue(from, control, to, t));
    _fieldModel->birdPosition = _fieldModel->from3dToPseudoPos(_bird->getPosition());
    _fieldModel->birdJumpProgress += 2 * dt;
    _bird->setRotationInRadians(
        _birdModel.fromRotation + _birdModel.rotationArc * tweenfunc::easeOut(t, 3)
    );
    
    _birdModel.pseudoHight = from.distance(to)/5 * (1 - 2 * abs(t-0.5));
    
}

void GameScene::updateError(float dt) {
    if(!_errorVisible) return;
    _errorTime += dt;
    if(_fieldModel->isTutotialFinished()) {
        if(_errorTime > 10) {
            hideMovesIcons();
            return;
        }
    }
    size_t i = 0;
    for(auto j : _fieldModel->moves) {
        if(_errorLabels.size() == i) {
            break;
        }
        auto s = _errorLabels.at(i);
        s->setPosition(getPlatformPos(j));
        Utils::setNodeHeightR(s, 0.1);
        s->setScaleY(_fieldModel->yScale * s->getScale());
        i++;
    }
}

void GameScene::updateHealth(float dt) {
    if(!_gameBegan) { 
        return;
    }
    removeHealth(1.5f * dt);
}

void GameScene::updateTutorial(float dt) {
    if(_gameBegan) {
        return;
    }
    if(!_tutorialArrow) {
        return;
    }
    auto pos = getPlatformPos(_fieldModel->moves.front());
    _tutorialArrow->setPosition(pos);
}

void GameScene::updateStageColor(float dt) {
    _stageColorTime += dt;
    if(_stageColorTime > 1) { 
	if(_stageColorTime < 2) {
	    _currentColor = getNextColor();
	}
	_stageColorTime = 3;
	return;
    }
    auto c = getColorFromPrevStage(_stageColorTime);
    _fieldModel->setColorIfHSV(get<0>(c), get<1>(c), get<2>(c));
}


/* HANDLERS */

void GameScene::onMouseDown(Vec2 pos) {
    if(!_gameActivated) {
        _gameActivated = true;
        onGameActivated();
        return;
    }
    if(_isGameOver) return;
    if(_isGamePaused) return;
    if(!isTouchActive()) return;
    CCASSERT(_selectedIndex == -1, "_selectedIndex is not released");
    for(int i = 0; i < (int)_fieldModel->platforms.size(); i++) {
	if(!isTouchPlatform(pos, i)) continue;
	// TODO: merge it
	if(!_gameBegan && i != _fieldModel->moves.front()) {
	    return;
	}
	activatePlatform(i);
	onPlatfromSelect(i);
	break;
    }
}

void GameScene::onMouseUp(Vec2 pos) {
    if(!isTouchActive()) {
        diactivatePlatform();
        return;
    }
    if(_isGameOver) {
        return;
    }
    if(_selectedIndex == -1) {
        return;
    }
    // TODO: merge it 
    // TODO: or remove at all (deprecated?)
    diactivatePlatform();
    onPlatfromDeselect();
}

void GameScene::onBirdAppear(bool init) {
    int nextPosition = findRandomPossiblePosition();
    _positionsMask.at(nextPosition) = true;
    _birdModel.targetPos = nextPosition;
    _birdModel.moveQueueCount = _stageBirdStepsAhead;
    _birdModel.stepsRemain = _stageBirdStepsLength;
    _fieldModel->moves.push_back(_birdModel.targetPos);
    _fieldModel->birdPosition = _fieldModel->from3dToPseudoPos(_bird->getPosition());
    _fieldModel->birdJumpProgress = 0;
    _bird->setPosition(getPlatformPosRaw(_birdModel.targetPos));
    _bird->setVisible(true);
    
    // new bird case
    if(!init) {
        _bird->onNextStage();
        _bird->blink([this](){ this->onBirdMoved(); });
    }
}

void GameScene::onBirdMoved() {
    // I were thinking about _birdModel.moveQueueCount--; move inside if
    // bacse now i neet tut moves queue and it's more natural
    // make experiment with it. But remember my initBird() and queue in it now!
    _birdModel.moveQueueCount--;
    if(_birdModel.moveQueueCount > 0) {
        doBirdJump();
        return;
    }
}

void GameScene::onPlayerStartRotation() {
}

void GameScene::onPlayerStartJump() {
    SoundsManager::getInstance()->playJumpEffect();
    Vec2 jumpDirection = _player->getPosition() - _fieldModel->platforms.at(_playerModel.targetPos)->getPosition();
    jumpDirection *= _fieldModel->foxPower;
    _fieldModel->platforms.at(_playerModel.sourcePos)
               ->getPhysicsBody()
               ->setVelocity(
                    jumpDirection
                );
}

void GameScene::onPlayerFinishJump() {
    if(_birdModel.targetPos == _playerModel.targetPos) {
        _bird->setVisible(false);
        onNextStage();
    }
}

void GameScene::onPlayerFinishRespawn() {
    _playerModel.respawning = false;
}

void GameScene::onPlayerFailJump() {
    _playerModel.respawning = true;
    _playerModel.jumping = false;
    _player->setRotationInRadians(
        _playerModel.fromRotation + _playerModel.rotationArc
    );
    
    // TODO: it should be in the player class
    auto makeBlink = [this](float t) {
	return Sequence::create(
	    CallFunc::create([this](){ 
		_player->setVisible(false); 
	    }),
	    DelayTime::create(t),
	    CallFunc::create([this]() { 
            _player->setVisible(true); 
	    }), DelayTime::create(t),
	    nullptr
	);
    };
    _player->runAction(Sequence::create(
	makeBlink(0.2), makeBlink(0.15), makeBlink(0.1), makeBlink(0.1), makeBlink(0.1), 
	CallFunc::create([this](){ onPlayerFinishRespawn(); }), nullptr
    ));
}

void GameScene::onPlatfromSelect(int i) {
    if(i == -1) {
        return;
    }
    if(_playerMovesCount < 3) {
        _playerMovesCount++;
    }
    if(_playerMovesCount == 3) {
        assert(!_gameBegan);
        _playerMovesCount++;
        onGameBegan();
    }
    
    if(i == _playerModel.targetPos) {
        _message->showHint();
        return;
    }
    
    if(i != _fieldModel->moves.front()) {
        onPlatfromSelectFail(i);
    } else { 
        onPlatfromSelectSuccess(i);
    }
}

void GameScene::onPlatfromSelectSuccess(int i) {
    if(_fieldModel->isTutotialFinished()) {
        _score += (_rightClickCount / 5 + 1) * (int)(log(2 + _stageNumber) * 5);
        _message->setScore(_score);
    }
    _rightClickCount++;
    if(_rightClickCount == _rightClickNext) {
        _rightIndex++;
        _rightClickNext += RIGHT_CLICK_C * _rightIndex + RIGHT_CLICK_B;
        _message->showSuccessMessage(_rightIndex);
    }
    // see below how is it used with _score
    _fieldModel->platforms.at(i)->setIsSelected(true);
    _fieldModel->platforms.at(i)->setIsWrong(false);
    _fieldModel->moves.pop_front(); // TODO: it must happen only in one place
    _fieldModel->jumpNumber++;
    if(_fieldModel->jumpNumber == 3) {
        onTutorialMovesComplete();
    }
    if(_fieldModel->isTutotialFinished()) {
        hideMovesIcons();
    }
    _positionsMask.at(_playerModel.targetPos) = false;
    
    bool isLast = _birdModel.stepsRemain == 0;
    
    playerMoveSuccessTo(i, isLast);
    // TODO: it is the only place where I can notify about player first state.
    onPlayerStartRotation(); 
    // see also updatePlayer()
    
    if(!isLast) {
        addBirdRandomMove();
        _birdModel.stepsRemain--;
    }
    
}

void GameScene::onPlatfromSelectFail(int i) {
    _rightClickCount = 0;
    _rightIndex	     = 0;
    _rightClickNext  = RIGHT_CLICK_A;
    SoundsManager::getInstance()->playErrorEffect();
    _fieldModel->platforms.at(i)->setIsSelected(true);
    _fieldModel->platforms.at(i)->setIsWrong(true);
//    int rightPosition = _fieldModel->moves.front();
//    _fieldModel->moves.pop_front(); // TODO: it should happen only in one place
//    _positionsMask.at(_playerModel.targetPos) = false;
    //addBirdRandomMove();
    showMovesIcons();
    // TODO: make a special constant
    onPlayerFailJump();
//    playerMoveFailTo(rightPosition);
    _message->setScoreMultNone();
    return;
}

void GameScene::onPlatfromDeselect() {
    if(_playerModel.jumping && _playerModel.jumpingTime < 0.8) {
	//onPlayerFailJump();
    }
}

void GameScene::playerMoveSuccessTo(int i, bool isLastInStage) {
    _playerModel.sourcePos = _playerModel.targetPos;
    _playerModel.targetPos = i;
    _fieldModel->playerPos = _playerModel.targetPos;
    _playerModel.jumping = true;
    _playerModel.speed = _player->getPosition().distance(
	_fieldModel->platforms.at(_playerModel.targetPos)->getPosition()) * 2;
    _playerModel.jumpingTime = 0;
    _playerModel.fromRotation = _player->getRotation();
    _playerModel.isRotated = false;
    _playerModel.rotationTime = 0;
    auto from = getPlatformPosRaw(_playerModel.sourcePos);
    auto to   = getPlatformPosRaw(_playerModel.targetPos);
    _playerModel.rotationArc = computeRotationArc(from, to, _playerModel.fromRotation);
    _playerModel.timeToRotate = abs(_playerModel.rotationArc) / 360;
    _playerModel.timeToRotate /= (_fieldModel->foxPower + 1);
    // see also updatePlayer()
}

void GameScene::playerMoveFailTo(int i) {
    _playerModel.sourcePos = _playerModel.targetPos;
    _playerModel.targetPos = i;
    _fieldModel->playerPos = _playerModel.targetPos;
    auto from = getPlatformPosRaw(_playerModel.sourcePos);
    auto to = getPlatformPosRaw(_playerModel.targetPos);
    auto v = to - from;
    auto targetAngle = atan2(v.y, v.x);
    targetAngle = targetAngle * 180 / 3.141592;
    targetAngle += 90; // because fox looks down
}

bool GameScene::isTouchPlatform(Vec2 pos, int i) {
    pos.y /= _fieldModel->yScale;
    return _fieldModel->platforms.at(i)->isInside(pos);
}

void GameScene::activatePlatform(int i) {
    if(_selectedIndex == i) {
        return;
    }
    if(_selectedIndex != i) {
        diactivatePlatform();
    }
    _selectedIndex = i;
    _fieldModel->platforms.at(_selectedIndex)->setIsSelected(false);
    _fieldModel->platforms.at(_selectedIndex)->setHealth(100);
    _alivePlatforms.insert(i);
}

void GameScene::diactivatePlatform() {
    _selectedIndex = -1; 
    onPlatfromDeselect(); 
}

void GameScene::rebuildMovesIcons() {
    for(auto l: _errorLabels) {
        l->removeFromParent();
    }
    _errorLabels.clear();
    for(size_t i = 0; i < _fieldModel->moves.size(); i++) {
        size_t numName = i + 1;
        if(!_fieldModel->isTutotialFinished()) {
            numName += _fieldModel->jumpNumber;
            if(i + _fieldModel->jumpNumber == _fieldModel->moves.size()) {
                break;
            }
        }
        auto label = Sprite::create(
            "el_" + Utils::toString(numName) + ".png"
        );
        // there isn`t Utils::setNodeHeightR cuz it is in updateError()
        label->setCameraMask((unsigned short)CameraFlag::USER2);
        addChild(label, 7);
        _errorLabels.push_back(label);
    }

}

void GameScene::showMovesIcons() {
    rebuildMovesIcons();
    _field->setMovesVisible(true);
    _errorVisible = true;
    _errorTime = 0;
}

void GameScene::hideMovesIcons() {
    for(auto l: _errorLabels) l->removeFromParent();
    _errorLabels.clear();
    _errorVisible = false;
    _errorTime = 0;
    _field->setMovesVisible(false);
    return;
}

void GameScene::showPauseLayer() {
    if(_pauseMessageBox == nullptr)  {
        _pauseMessageBox = PauseMessageBox::create();
        _pauseMessageBox->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        _pauseMessageBox->setOnContinue([this]() {
            onGameResume();
        });
        addChild(_pauseMessageBox, 20);
    } else {
        _pauseMessageBox->setVisible(true);
    }
}

void GameScene::hidePauseLayer() {
    _pauseMessageBox->setVisible(false);
}

void GameScene::showSettings() {
    ScenesManager::getInstance()->showSettings();
}

void GameScene::hideSettings() {
    ScenesManager::getInstance()->hideSettings();
}

void GameScene::addHealth(float h) {
    _playerModel.health += h;
    if(_playerModel.health > 100) _playerModel.health = 100;
    _healthAsset->setHealth(_playerModel.health);
}

void GameScene::removeHealth(float h) {
    _playerModel.health -= h;
    if(_playerModel.health <= 0) {
        _playerModel.health = 1;
        onGameOver();
        return;
    }
    _healthAsset->setHealth(_playerModel.health);
}

Vec2 GameScene::getPlatformPos(int i) const {
    auto pos = getPlatformPosRaw(i);
    return Vec2(pos.x, pos.y * _fieldModel->yScale);
}

Vec2 GameScene::getPlatformPosRaw(int i) const {
    return _fieldModel->platforms.at(i)->getPosition();
}

void GameScene::fillRandomPlatformPositions(vector<Vec2>& vec, int count) {
    vec.empty();
    int xSteps = 8;
    int ySteps = 3;
    float minY = min_element(
        _fieldModel->topIcePoints.begin(), _fieldModel->topIcePoints.end(), 
        [](const Vec2& a, const Vec2& b){ return a.y > b.y; }
    )->y;
    float topPartLimit = minY / _size.height;
    for(int i = 0; i < xSteps; i++) {
        for(int j = 0; j < ySteps; j++) {
            vec.push_back(Vec2(
                0.05 + i * 0.9 / xSteps + 0.5 / xSteps, 
                0.05 + j * 0.9 * topPartLimit / ySteps + 0.5 / ySteps
            ));
        }
    }
    random_shuffle(vec.begin(), vec.end());
    vec.resize(count);
}

void GameScene::addPlatformEdge() {
    float w = 0.5;
    Vec2 pos(w, 0);
    addPlatform(pos);
}

void GameScene::addPlatform(const Vec2& pos) {
    // \see initPlatforms()
    auto platform = PlatformModel::create();
    addChild(platform, 2);
    _fieldModel->platforms.push_back(platform);
    Utils::setPosition(platform, pos);
    _positionsMask.push_back(false);
}

void GameScene::updateCameraPos() {
    Vec3 eye(0,-1, 100);
    // compute eye y
    // if we want to get y that it look like pseudo 3d render
    // that we need to compute tan(acos(FieldModel::yScale))
    // but tan(acos(x)) = (sqrt(1-x) * sqrt(1+x)) / x 
    float t = _fieldModel->yScale;
    eye.y = -sqrt(1 - t * t) / t;
    eye.y *= 100;
    _camera->setPosition3D(eye);
    _camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
}

/* TOOLS */

int GameScene::findRandomPossiblePosition() const {
    int nextRandom = rand() % _positionsMask.size();
    // TODO: prove that all possible variants have same probability
    while(_positionsMask.at(nextRandom)) {
	nextRandom = (nextRandom + 1) % _positionsMask.size();
    }
    return nextRandom;
}

void GameScene::addBirdRandomMove() {
    _birdModel.moveQueueCount++;
    if(_birdModel.jumping) return;
    doBirdJump();
}

void GameScene::doBirdJump() {
    int nextRandom = findRandomPossiblePosition();
    _birdModel.sourcePos = _birdModel.targetPos;
    _birdModel.targetPos = nextRandom;
    _fieldModel->moves.push_back(_birdModel.targetPos);
    _positionsMask.at(_birdModel.targetPos) = true;
    _birdModel.jumping = true;
    _fieldModel->birdJumpProgress = 0;
    auto from = getPlatformPosRaw(_birdModel.sourcePos);
    auto to   = getPlatformPosRaw(_birdModel.targetPos);
    float fromRotation = _bird->getRotationInDegrees();
    _birdModel.fromRotation = fromRotation;
    _birdModel.rotationArc = computeRotationArc(from, to, fromRotation);
    if(_errorVisible) rebuildMovesIcons();
    // \see updateBird()
}
    
void GameScene::generateTopIcePoints() {
    // see FieldDrawer::drawTopIceHeight()
    // see FieldAsset::update() - there floatMinOffset is used
    const float floatMinOffset = 7;
    float pointCount = 15;
    for(int i = 0; i <= pointCount; i++) {
	Vec2 res;
	res.x = i * _fieldModel->size.width / pointCount;
	res.y = _fieldModel->size.height - (floatMinOffset + rand() % 8);
	_fieldModel->topIcePoints.emplace_back(res);
    }
}

bool GameScene::isTouchActive() const {
    if(_birdModel.jumping) return false;
    if(_playerModel.jumping) return false;
    if(_playerModel.respawning) return false;
    if(_birdModel.moveQueueCount > 0) return false;
    return true;
}

void GameScene::onTutorialBoxClose() {
    initTapToPlay();
}

void GameScene::onGameActivated() {
    SoundsManager::getInstance()->playMusic();
    _message->hideBeginningMessage();
    initPauseButton();
    // not good, I know, but logic is the same. It's because 
    // we want to notice about tutorial moves count setting
    if(SettingsManager::getInstance()->getIsTutorial()) { 
        // TODO: not good
        initTutorial();
        showMovesIcons();
    } else {
        _playerMovesCount = 4;
        _fieldModel->jumpNumber = 4;
        onGameBegan();
    }
    onBirdMoved();
}

void GameScene::onTutorialMovesComplete() {
    hideMovesIcons();
    _message->showHint();
}

void GameScene::onGameBegan() {
    _gameBegan = true;
    if(_tutorialArrow) {
        _tutorialArrow->removeFromParent();
    }
}

void GameScene::onGameOver() {
    // remove other things
    if(_isGameOver) {
        return;
    }
    AdvertisingManager::getInstance()->triggerAdv();
    hideMovesIcons();
    _isGameOver = true;
    SoundsManager::getInstance()->stopMusic();
    _healthAsset->setVisible(false);
    _message->setVisible(false);
    _edgeNode->removeFromParent();
    if(_pauseButton) _pauseButton->removeFromParent();
    getPhysicsWorld()->setGravity(Vec2(0, -98));
    _resultsLayer = ResultsLayer::create();
    _resultsLayer->setScore(_score);
    addChild(_resultsLayer, 100);
    showSettings();
    AnalyticsManager::getInstance()->sendEvent("game over");
    if(LivesManager::getInstance()->getLivesTookCurrentSession() > 2) {
        LivesManager::getInstance()->askAboutNotificatoins();
    }
}

void GameScene::onNextStage() {

    SoundsManager::getInstance()->playNextStageEffect();
    
    addHealth(30);

    _stageColorTime = 0;
    _stageNumber++;
    int c = _stageNumber % 4;
    if(c == 0) {
        if(_stageBirdStepsAhead == MAX_BIRD_STEPS_AHEAD) {
            c = 1;
        } else {
            _stageBirdStepsAhead++;
        }
    }
    if(c == 1) {
	    addPlatformEdge();
    }
    if(c == 2) {
	    _fieldModel->birdSpeed++;
    }
    if(c == 3) {
        _fieldModel->foxPower += 0.1;
    }
    
    _message->showLevel(_stageNumber + 1);
    
    onBirdAppear(false);
    
    AnalyticsManager::getInstance()->sendEvent(
        Utils::toString(_stageNumber + 1)
    );
    
}

void GameScene::onGamePause() {
    // TODO: maybe remove this
    if(!_gameActivated) {
        return;
    }
    _pauseButton->setSwitched(false);
    _isGamePaused = true;
    showPauseLayer();
    showSettings();
    SoundsManager::getInstance()->stopMusic();
    getPhysicsWorld()->setSpeed(0);
}

void GameScene::onGameResume() {
    _isGamePaused = false;
    if(_pauseButton) _pauseButton->setSwitched(true);
    hideSettings();
    SoundsManager::getInstance()->playMusic(false);
    hidePauseLayer();
    getPhysicsWorld()->setSpeed(PHYSICS_SPEED);
}

void GameScene::onBack() {
    // assume that the results layer is open
    if(_isGameOver) {
	_resultsLayer->onBack();
	return;
    }
    if(_gameBegan) onGamePause();
    auto e = ScenesManager::getInstance()->toggleExitMessage();
    if(!e) return;
    if(_pauseMessageBox) _pauseMessageBox->setVisible(false);
    e->setOnNo([this]() {
        if(_pauseMessageBox) {
            _pauseMessageBox->setVisible(true);
        }
    });
}

void GameScene::onAppPause() {
    if(_isGameOver) { 
        return;
    }
    if(!_isGamePaused) {
        onGamePause();
    }
}

void GameScene::onAppResume() { }

float GameScene::computeRotationArc(
    const Vec2& from, const Vec2& to, float fromRotation
) {
    auto v = to - from;
    auto targetAngle = atan2(v.y, v.x);
    targetAngle = targetAngle * 180 / 3.141592;
    targetAngle += 90; // because fox looks down
    float rotationArc = targetAngle - fromRotation;
    if(abs(rotationArc) > 180) {
        if(rotationArc > 0) {
            rotationArc -= 360;
        } else {
            rotationArc += 360;
        }
    }
    assert(abs(rotationArc) < 181);
    return rotationArc;
}

GameScene::hsv GameScene::getColorFromPrevStage(float t) {
    auto fromColor   = _currentColor;
    auto toColor     = getNextColor();
    
    float fromH = get<0>(fromColor);
    float toH	= get<0>(toColor);
    float distH = toH - fromH;
    
    if(distH < 0) {
        distH += 360;
    }
    
    float resH = fromH + distH * t;
    if(resH > 360) {
        resH -= 360;
    }
    
    return hsv(
        resH, 
        get<1>(fromColor) * (1.0 - t) + get<1>(toColor) * t,
        get<2>(fromColor) * (1.0 - t) + get<2>(toColor) * t
    );
    
}

GameScene::hsv GameScene::getNextColor() {
    auto h = get<0>(_currentColor);
    auto s = get<1>(_currentColor);
    auto v = get<2>(_currentColor);
    
    //s += 3;
    v -= 5;
    if(s > 100) {
        s = 100;
    }
    if(v < 0) {
        v =0;
    }
    
    return hsv(h, s, v);
}

GameScene::~GameScene() {
    delete _fieldModel;
}
