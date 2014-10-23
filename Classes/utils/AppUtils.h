#ifndef APP_UTILS
#define APP_UTILS

#include "cocos2d.h"

USING_NS_CC;

class AppUtils: public Ref {
protected:
public:
    static void setAdInvisible(bool isInvisible);
    static void showAdFullscreen();
    static bool displayFullscreenAd(bool isDisplay);
    static void shareFacebook(std::string title, std::string caption, std::string description, std::string imgUrl, std::string linkUrl, std::string msg);
    static void shareTwitter(std::string title, std::string caption, std::string description, std::string imgUrl, std::string linkUrl, std::string msg);
    static void openForRate();
    static bool isRemovedAds();
    static void buyInApp();
    static void restoreInApp();
private:
    AppUtils();
    virtual ~AppUtils();
};

#endif /* UTILS_H */


