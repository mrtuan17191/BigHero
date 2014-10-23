#include "BusinessHandler.h"

BusinessHandler::BusinessHandler() {
}

BusinessHandler::~BusinessHandler() {
}

void BusinessHandler::processEvent(GameEvent* event) {    
    switch (event->getEventCode()) {
        case EVT_START_GAME: {
            auto scene = new BeginScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->runWithScene(scene);
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            break;
        }
        case EVT_BACK_BEGIN: {
            auto scene = new BeginScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->getScheduler()->unscheduleUpdate(Director::getInstance()->getRunningScene());
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            break;
        }
        case EVT_START_MAP: {
            auto scene = new ChapterScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->getScheduler()->unscheduleUpdate(Director::getInstance()->getRunningScene());
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            break;
        }
        case EVT_START_PLAY: {
            DataEvent* dataEvent = (DataEvent*) event;
            auto scene = new GameScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->getScheduler()->unscheduleUpdate(Director::getInstance()->getRunningScene());
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            this->gameModel->transit(1);
            this->gameModel->play(dataEvent->getArgumentInt());
            break;
        }
        case EVT_REPLAY_MAP: {
            Director::getInstance()->getScheduler()->unscheduleAllForTarget(Director::getInstance()->getRunningScene());
            auto scene = new GameScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            
            this->gameModel->transit(1);
            this->gameModel->replay();
            break;
        }
        case EVT_NEXT_MAP: {
            Director::getInstance()->getScheduler()->unscheduleAllForTarget(Director::getInstance()->getRunningScene());
            auto scene = new GameScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            
            this->gameModel->transit(1);
            this->gameModel->next();
            this->gameModel->updateLevel();
            this->gameModel->save();
            break;
        }
        case EVT_COMPLETED_GAME: {
            auto scene = new ChapterScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->getScheduler()->unscheduleUpdate(Director::getInstance()->getRunningScene());
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            
            this->gameModel->quit();
            scene->showCongratulationMessage();
            break;
        }
        case EVT_JOYSTICK_MOVE: {
            DataEvent* dataEvent = (DataEvent*) event;
            Man* man = this->gameModel->getCurrentLevel()->getMan();
            man->run(CONF_MAN_VELOCITY, dataEvent->getArgumentFloat());
            break;
        }
        case EVT_JOYSTICK_STAND: {
            Man* man = this->gameModel->getCurrentLevel()->getMan();
            man->run(0, 0);
            break;
        }
        case EVT_NEXT_CHECK_POINT: {
            this->gameModel->getCurrentLevel()->nextCheckPoint();
            break;
        }
        case EVT_CREATE_END_ZONE: {
            DataEvent* dataEvent = (DataEvent*) event;
            this->gameModel->getCurrentLevel()->createEndzone(Size(dataEvent->getArgumentPoint().x, dataEvent->getArgumentPoint().y));
            break;
        }
        case EVT_PUNCH: {
            this->gameModel->getCurrentLevel()->getMan()->punch();
            break;
        }
        case EVT_JUMP: {
            this->gameModel->getCurrentLevel()->getMan()->jump();
            break;
        }
        case EVT_SHARE_FB: {
            __Array* captions = __Array::create();
            captions->addObject(__String::create("We're ninjas."));
            captions->addObject(__String::create("Technically, we're turtles."));
            captions->addObject(__String::create("We're mutants!"));
            captions->addObject(__String::create("You look like a ninja turtle..."));
            captions->addObject(__String::create("Let's go save the world!"));
            
            string name = "Gravity Ninja Turtle (iOS/Android) Apps";
            string caption = ((__String*) captions->getRandomObject())->getCString();
            char description[200];
            int currentLevelSoFar = this->gameModel->getSavedData()->getLevel();
            int totalLevelSoFar = this->gameModel->getMapData()->getLevelSize();
            if (currentLevelSoFar == totalLevelSoFar) {
                sprintf(description, "I won this game, do you wanna try?");
            } else {
                sprintf(description, "I completed %d/%d level in Gravity Ninja Turtle. Do you want play with me?", currentLevelSoFar, totalLevelSoFar);
            }
            string link = "https://itunes.apple.com/us/app/gravity-ninja-turtle/id909603503?ls=1&mt=8";
            string image = "https://farm6.staticflickr.com/5561/15027425015_282358a2d2_s.jpg";
            string errorFacebook = "Make sure you are connected to the internet and have atleast one Facebook account set up.";
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            link = "https://play.google.com/store/apps/details?id=vn.clonerteam.ninjaturle";
            char facebookTxt[500];
            sprintf(facebookTxt, "%s:break:%s:break:%s:break:%s:break:%s", name.data(), caption.data(), description, link.data(), image.data());
            CCApplication::sharedApplication()->openURL(facebookTxt);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            AppUtils::shareFacebook(name, caption, description, image, link, errorFacebook);
#endif
            break;
        }
        case EVT_SHARE_TW: {
            __Array* captions = __Array::create();
            captions->addObject(__String::create("We're ninjas."));
            captions->addObject(__String::create("Technically, we're turtles."));
            captions->addObject(__String::create("We're mutants!"));
            captions->addObject(__String::create("You look like a ninja turtle..."));
            captions->addObject(__String::create("Let's go save the world!"));
            
            string name = "Gravity Ninja Turtle (iOS/Android) Apps";
            string caption = ((__String*) captions->getRandomObject())->getCString();
            char description[200];
            sprintf(description, "I completed %d/%d level in Gravity Ninja Turtle. Do you want play with me?", gameModel->getSavedData()->getLevel(), gameModel->getMapData()->getLevelSize());
            string link = "https://itunes.apple.com/us/app/gravity-ninja-turtle/id909603503?ls=1&mt=8";
            string image = "https://farm6.staticflickr.com/5561/15027425015_282358a2d2_s.jpg";
            string errorTwitter = "Make sure you are connected to the internet and have atleast one Twitter account set up.";
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            AppUtils::shareTwitter(name, caption, description, image, link, errorTwitter);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCApplication::sharedApplication()->openURL(":shareTwitter:");
#endif
            break;
        }
        case EVT_RATE: {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            AppUtils::openForRate();
#endif
            break;
        }
        case EVT_CHANGE_MUSIC: {
            DataEvent* dataEvent = (DataEvent*) event;
            this->gameModel->getSavedData()->setMusicOn(dataEvent->getArgumentInt() == 1 ? true : false);
            this->gameModel->save();
            break;
        }
        case EVT_CHANGE_SFX: {
            DataEvent* dataEvent = (DataEvent*) event;
            this->gameModel->getSavedData()->setSfxOn(dataEvent->getArgumentInt() == 1 ? true : false);
            this->gameModel->save();
            break;
        }
        case EVT_PAUSE_GAME: {
            this->gameModel->pause();
            break;
        }
        case EVT_RESUME_GAME: {
            this->gameModel->resume();
            break;
        }
        case EVT_QUIT_GAME: {
            auto scene = new ChapterScene();
            this->getController()->listen(scene);
            
            Director::getInstance()->getScheduler()->unscheduleUpdate(Director::getInstance()->getRunningScene());
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
            Director::getInstance()->getScheduler()->scheduleUpdate(scene, SCHEDULE_SCENE, false);
            
            this->gameModel->quit();
            break;
        }
        default: {
            break;
        }
    }
}