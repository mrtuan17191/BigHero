/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import <UIKit/UIKit.h>
#import "GADBannerView.h"
#import "GADInterstitial.h"
#import <Social/Social.h>
#import <FacebookSDK/FacebookSDK.h>
#import <StoreKit/StoreKit.h>
#import "InAppPurchaseObserver.h"

@interface RootViewController : UIViewController<GADInterstitialDelegate> {
    GADBannerView *bannerView_;
    GADInterstitial *interstitial_;
    InAppPurchaseObserver *inappObserver;
}
- (BOOL) prefersStatusBarHidden;
- (void) setAdVisible: (BOOL) isVisible;
- (void) showFullscreenAd;
- (void) shareFb: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg;
- (void) shareTw: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg;
- (BOOL) displayFullscreenAd: (BOOL) isDisplay;
- (void) buyInApp;
- (void) restoreInApp;
- (BOOL) isRemovedAds;
@end
