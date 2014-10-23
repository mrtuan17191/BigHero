#include "AppDelegate.h"
#include "BusinessHandler.h"
#include "AppMacros.h"
#include "controllers/include/controllers.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Beatdown");
        director->setOpenGLView(glview);
    }
    
    FileUtils *fileUtils = FileUtils::getInstance();
	std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
    
    Size size = glview->getFrameSize();
    float frameRate = size.height / size.width;
    float fitWidth = designResolutionSize.height / frameRate;
    float fitHeight = designResolutionSize.height;
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    Size displayResolutionSize = Size(fitWidth, fitHeight);
    Size vectorMove = (displayResolutionSize - designResolutionSize) * 0.5f;

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
    sfc->addSpriteFramesWithFile(DOC_FX);
    sfc->addSpriteFramesWithFile(DOC_GUI);
    sfc->addSpriteFramesWithFile(DOC_GAME);
    sfc->addSpriteFramesWithFile(DOC_PICKUPS);
    
    GameModel* gameModel = GameModel::getInstance();
    gameModel->setDisplayResolutionSize(displayResolutionSize);
    gameModel->setVectorMove(Point(vectorMove.width, vectorMove.height));
    
    SoundModel* soundModel = SoundModel::getInstance();
    SimpleAudioEngine* simpleAudioEngine = SimpleAudioEngine::getInstance();

    auto controller = new GameController();
    CC_SAFE_RETAIN(controller);
    auto bussinessHandler = new BusinessHandler();
    CC_SAFE_RETAIN(bussinessHandler);

    controller->addHandler(bussinessHandler);
    controller->start();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
}

extern "C"{
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
}
