#pragma once

#include "Scenes/BasicScene.h"

#include "Models/BirdModel.h"
#include "Models/FieldModel.h"
#include "Models/PlayerModel.h"
#include "Models/PlatformModel.h"

#include "GameElements/BirdAsset.h"
#include "GameElements/PlayerAsset.h"
#include "GameElements/BackgroundAsset.h"
#include "GameElements/TutorialArrowAsset.h"

#include "GUI/MessagesLayer.h"
#include "GUI/HealthAsset.h"
#include "GUI/StageProgressAsset.h"
#include "GUI/TutorialMessageBox.h"
#include "GUI/SettingsAsset.h"
#include "GUI/ResultsLayer.h"
#include "GUI/PauseMessageBox.h"

#include <set>
#include <list>
#include <tuple>
#include <vector>


// TODO: do I have memory leaks here?
// TODO: it is a controller
// TODO: make UI node where health + messages are stored. For Default camera
class FieldAsset;
class GameScene : public BasicScene {
public:
    CREATE_FUNC(GameScene);
    bool init() override;

private:

    typedef std::tuple<float, float, float> hsv;
    
    /* CONFIG */
    // TODO: make a special json config
    const int        RIGHT_CLICK_A	      = 5; // init // TODO: rename it
    const int        RIGHT_CLICK_B	      = 5; // step          success clicks
    const int        RIGHT_CLICK_C	      = 3; // grow
    const float      PHYSICS_SPEED	      = 1;
    const static hsv COLOR_BEGIN;
    const int        MIN_BIRD_STEPS_AHEAD  = 3;
    const int        MAX_BIRD_STEPS_AHEAD  = 6;
    
    
    /* STATE */
    std::vector<bool>	_positionsMask;            // TODO: rename - this is platfroms
                                                   //       possible to jump
    int			_playerMovesCount     = 0; // TODO: rename to init moves?
    bool		_isInitMoves          = true;
    int			_selectedIndex        = -1;
    int			_rightClickCount      = 0; // TODO: rename it - this 
    int			_rightIndex	      = 0; // success clicks
    int			_rightClickNext	      = RIGHT_CLICK_A;
    bool                _gameActivated        = false; // flag for TapToPlay feature
    bool		_gameBegan	      = false;
    
    bool		_introIsActive	      = true;
    float		_introPlayTime	      = 0.0f;
    float		_introDuration	      = 3;
    
    bool		_isGameOver	      = false;
    bool		_isGamePaused	      = false;
    float		_stageColorTime	      = 1; // TODO: move to a special stage model and init for it
    int			_stageNumber	      = 0;
    int                 _stageBirdStepsAhead  = MIN_BIRD_STEPS_AHEAD;
    int                 _stageBirdStepsLength = 4;
    
    int			_score		      = 0;
    
    bool		_errorVisible	      = false;
    float		_errorTime	      = 0;
    
    std::set<int>	_alivePlatforms;
    
    /* MODELS */
    // TODO: make pointers
    PlayerModel		_playerModel;
    BirdModel		_birdModel;
    FieldModel*		_fieldModel;
    
    /* ELEMENTS */
    // TODO: special tag for gui elements
    PlayerAsset*	_player;
    BirdAsset*		_bird;
    FieldAsset*		_field;
    MessagesLayer*	_message;
    BackgroudAsset*	_background;
    HealthAsset*	_healthAsset;
    TutorialArrowAsset* _tutorialArrow;
    TutorialMessageBox*	_tutorialMessageBox = nullptr;
    std::vector<cocos2d::Node*> _errorLabels;
    cocos2d::Camera*	_camera;
    cocos2d::Sprite*	_playerShadow; // TODO: make special asset for shadow
    cocos2d::Sprite*	_birdShadow;
    cocos2d::Node*	_edgeNode;
    
    GUI::SwapButton*	_pauseButton;
    
    ResultsLayer*	_resultsLayer = nullptr;
    PauseMessageBox*	_pauseMessageBox = nullptr;
    
    hsv                 _currentColor;
        
    
    /* INITS */
    // models
    void initModels(); 
    void initPlatforms();
    // elements
    void initBackground();
    void initField();
    void initPlayer();
    void initShadows();
    void initBird();
    void initHealth();
    void initPauseButton();
    void initMessages();
    void initTutorialBox();
    void initTutorial();
    void initTapToPlay();
   
    // modules
    void initPhysics();
    void initLight();
    void initCameras();
    // other
    void initDebug();
    
    /* UPDATES */
    void update(float dt) override;
    void updateIntoAnimation(float dt);
    void updateActivePlatforms(float dt);
    void updatePlayer(float dt);
    void updateShadows(float dt);
    void updateBird(float dt);
    void updateError(float dt);
    void updateHealth(float dt);
    void updateTutorial(float dt);
    void updateStageColor(float dt);

    /* HANDLERS */
    void onMouseDown(cocos2d::Vec2 pos) override;
    void onMouseUp(cocos2d::Vec2 pos) override;
    
    void onPlatfromSelect(int i);
    void onPlatfromSelectSuccess(int i);
    void onPlatfromSelectFail(int i);
    void onPlatfromDeselect();
    
    void onBirdAppear(bool init);
    void onBirdMoved(); // TODO: rename to bird moves changed
    
    void onPlayerStartRotation();
    void onPlayerStartJump();
    void onPlayerFinishJump();
    void onPlayerFailJump();
    void onPlayerFinishRespawn();
    
    void onTutorialBoxClose();
    void onGameActivated();
    void onTutorialMovesComplete();
    void onGameBegan();
    void onGameOver();
    void onNextStage();
    void onGamePause();
    void onGameResume();
    
    void onBack() override;
    
    void onAppPause() override;
    void onAppResume() override;
    
    /* TOOLS */
    int findRandomPossiblePosition() const;
    void addBirdRandomMove();
    void doBirdJump();
    // player
    void playerMoveSuccessTo(int i, bool isLastInStage = false);
    void playerMoveFailTo(int i);
    
    void diactivatePlatform();
    void activatePlatform(int i);
    bool isTouchPlatform(cocos2d::Vec2 pos, int i);
    
    void rebuildMovesIcons();
    void showMovesIcons();
    void hideMovesIcons();
    
    void showPauseLayer();
    void hidePauseLayer();
    
    void showSettings();
    void hideSettings();
    
    void addHealth(float h);
    void removeHealth(float h);
    
    bool isTouchActive() const;
    
    cocos2d::Vec2 getPlatformPos(int i) const;
    cocos2d::Vec2 getPlatformPosRaw(int i) const;
    
    void fillRandomPlatformPositions(std::vector<cocos2d::Vec2>& vec, int count);
    
    void addPlatformEdge();
    void addPlatform(const cocos2d::Vec2& platfrom);
    
    void updateCameraPos();
    
    void generateTopIcePoints();
    
    static float computeRotationArc(
        const cocos2d::Vec2& from,
        const cocos2d::Vec2& to, 
        float fromRotation
    );
    
    hsv getColorFromPrevStage(float t);
    hsv getNextColor();
    
    virtual ~GameScene();
    
};
