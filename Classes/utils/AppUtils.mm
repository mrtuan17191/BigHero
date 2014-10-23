#include "AppUtils.h"
#include "AppController.h"

void AppUtils::setAdInvisible(bool isInvisible) {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    
    if (isInvisible) {
        [appController setAdVisible:YES];
    } else {
        [appController setAdVisible:NO];
    }
}

void AppUtils::showAdFullscreen() {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    [appController showFullscreenAd];
}

bool AppUtils::displayFullscreenAd(bool isDisplay) {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    return [appController displayFullscreenAd:isDisplay];
}

void AppUtils::shareFacebook(std::string title, std::string caption, std::string description, std::string imgUrl, std::string linkUrl, std::string msg) {
    NSString* nsTitle = [NSString stringWithUTF8String:title.data()];
    NSString* nsCaption = [NSString stringWithUTF8String:caption.data()];
    NSString* nsDescription = [NSString stringWithUTF8String:description.data()];
    NSString* nsImgUrl = [NSString stringWithUTF8String:imgUrl.data()];
    NSString* nsLinkUrl = [NSString stringWithUTF8String:linkUrl.data()];
    NSString* nsMsg = [NSString stringWithUTF8String:msg.data()];
    
    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    [appController shareFb:nsTitle withCaption:nsCaption withDescription:nsDescription withImage:nsImgUrl withUrl:nsLinkUrl errorMsg:nsMsg];
}

void AppUtils::shareTwitter(std::string title, std::string caption, std::string description, std::string imgUrl, std::string linkUrl, std::string msg) {
    NSString* nsTitle = [NSString stringWithUTF8String:title.data()];
    NSString* nsCaption = [NSString stringWithUTF8String:caption.data()];
    NSString* nsDescription = [NSString stringWithUTF8String:description.data()];
    NSString* nsImgUrl = [NSString stringWithUTF8String:imgUrl.data()];
    NSString* nsLinkUrl = [NSString stringWithUTF8String:linkUrl.data()];
    NSString* nsMsg = [NSString stringWithUTF8String:msg.data()];
    
    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    [appController shareTw:nsTitle withCaption:nsCaption withDescription:nsDescription withImage:nsImgUrl withUrl:nsLinkUrl errorMsg:nsMsg];
}

void AppUtils::openForRate() {
    NSString* link = @"itms-apps://itunes.apple.com/app/id909603503";
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 7.0) {
        link = @"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=909603503";
    }
    NSURL* nsUrl = [NSURL URLWithString:link];
    [[UIApplication sharedApplication] openURL:nsUrl];
}

bool AppUtils::isRemovedAds() {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    return [appController isRemovedAds];
}

void AppUtils::buyInApp() {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    [appController buyInApp];
}

void AppUtils::restoreInApp() {
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    [appController restoreInApp];
}