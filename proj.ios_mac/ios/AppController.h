#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

- (void) setAdVisible: (BOOL) isVisible;
- (void) showFullscreenAd;
- (void) shareFb: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg;
- (void) shareTw: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg;
- (BOOL) displayFullscreenAd: (BOOL) isDisplay;
- (void) buyInApp;
- (void) restoreInApp;
- (BOOL) isRemovedAds;

@property(nonatomic, readonly) RootViewController* viewController;

@end

