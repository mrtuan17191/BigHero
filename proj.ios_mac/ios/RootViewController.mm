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

#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"

static BOOL loadedFullscreen = false;
@implementation RootViewController

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void) setAdVisible: (BOOL) isVisible {
    [bannerView_ setHidden:isVisible];
}

- (void) showFullscreenAd {
    if (interstitial_ != nil) {
        [interstitial_ release];
        interstitial_ = nil;
    }
    // Full screen
    interstitial_ = [[GADInterstitial alloc] init];
    interstitial_.delegate = self;
    interstitial_.adUnitID = @"ca-app-pub-1701972712021698/6131904960";
    [interstitial_ loadRequest:[self request]];
}

- (void) shareFb: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg {
    // Prepare the native share dialog parameters
    FBShareDialogParams *shareParams = [[FBShareDialogParams alloc] init];
    shareParams.link = [NSURL URLWithString:url];
    shareParams.name = title;
    shareParams.caption = caption;
    shareParams.picture = [NSURL URLWithString:imgUrl];
//    shareParams.description = description;
    
    if ([FBDialogs canPresentShareDialogWithParams:shareParams]){
        [FBDialogs presentShareDialogWithParams:shareParams
                                    clientState:nil
                                        handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                            if(error) {
                                                NSLog(@"Error publishing story. %@", [error description]);
                                            } else if (results[@"completionGesture"] && [results[@"completionGesture"] isEqualToString:@"cancel"]) {
                                                NSLog(@"User canceled story publishing.");
                                            } else {
                                                NSLog(@"Story published.");
                                            }
                                        }];
        
    } else {
        if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
            // Put together the dialog parameters
            NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys: title,
                                           @"name", caption,
                                           @"caption", description,
                                           @"description", url,
                                           @"link", imgUrl,
                                           @"picture",
                                           nil];
            
            // Invoke the dialog
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:params
                                                      handler:
             ^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                 if (error) {
                     // Error launching the dialog or publishing a story.
                     NSLog(@"Error publishing story.");
                 } else {
                     if (result == FBWebDialogResultDialogNotCompleted) {
                         // User clicked the "x" icon
                         NSLog(@"User canceled story publishing.");
                     } else {
                         [[[UIAlertView alloc] initWithTitle:@"Share Facebook"
                                                     message:@"Publish succeed, thank you."
                                                    delegate:nil
                                           cancelButtonTitle:@"OK!"
                                           otherButtonTitles:nil] show];
                     }
                 }
             }];
        } else {
            if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook]) {
                SLComposeViewController *fbSheetOBJ = [SLComposeViewController
                                                       composeViewControllerForServiceType:SLServiceTypeFacebook];
                
                fbSheetOBJ.completionHandler = ^(SLComposeViewControllerResult result) {
                    switch(result) {
                            //  This means the user cancelled without sending the Tweet
                        case SLComposeViewControllerResultCancelled:
                            break;
                            //  This means the user hit 'Send'
                        case SLComposeViewControllerResultDone:
                            break;
                    }
                    
                    //  dismiss the Tweet Sheet
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [self dismissViewControllerAnimated:NO completion:^{
                            NSLog(@"Facebook Sheet has been dismissed.");
                        }];
                    });
                };
                
                [fbSheetOBJ setInitialText:caption];
                [fbSheetOBJ addImage:[UIImage imageNamed:imgUrl]];
                [fbSheetOBJ addURL:[NSURL URLWithString:url]];
                [self presentViewController:fbSheetOBJ animated:YES completion:nil];
            } else {
                UIAlertView *a = [[UIAlertView alloc]initWithTitle:@"Error Sending Facebook" message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
                [a show];
            }
        }
    }
}

- (void) shareTw: (NSString*) title withCaption:(NSString*) caption withDescription:(NSString*) description withImage:(NSString*) imgUrl withUrl:(NSString*) url errorMsg: (NSString*) msg {
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:@"Share Twitter Fail"
                              message:@"This feature only work in iOS6 or higher, sorry and thank you."
                              delegate:nil
                              cancelButtonTitle:@"Done"
                              otherButtonTitles:nil,
                              nil];
        [alert show];
    } else {
        if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
            SLComposeViewController *tweetSheetOBJ = [SLComposeViewController
                                                      composeViewControllerForServiceType:SLServiceTypeTwitter];
            
            tweetSheetOBJ.completionHandler = ^(SLComposeViewControllerResult result) {
                switch(result) {
                        //  This means the user cancelled without sending the Tweet
                    case SLComposeViewControllerResultCancelled:
                        break;
                        //  This means the user hit 'Send'
                    case SLComposeViewControllerResultDone:
                        break;
                }
                
                //  dismiss the Tweet Sheet
                dispatch_async(dispatch_get_main_queue(), ^{
                    [self dismissViewControllerAnimated:NO completion:^{
                        NSLog(@"Twitter Sheet has been dismissed.");
                    }];
                });
            };
            
            
            [tweetSheetOBJ setInitialText:caption];
            [tweetSheetOBJ addImage:[UIImage imageNamed:imgUrl]];
            [tweetSheetOBJ addURL:[NSURL URLWithString:url]];
            [self presentViewController:tweetSheetOBJ animated:YES completion:nil];
        } else {
            // Dont set twitter account
            UIAlertView *a = [[UIAlertView alloc]initWithTitle:@"Error Sending Tweet" message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
            [a show];
        }
    }
}

- (BOOL) displayFullscreenAd: (BOOL) isDisplay {
    if ([self isRemovedAds] == false) {
        if (isDisplay == true && loadedFullscreen == true) {
            [interstitial_ presentFromRootViewController:self];
            loadedFullscreen = false;
            return true;
        } else if (isDisplay == false && loadedFullscreen == false) {
            [self showFullscreenAd];
        } else if (isDisplay == true && loadedFullscreen == false) {
            [self showFullscreenAd];
        }
    }
    return false;
}

- (void) buyInApp {
    if ([SKPaymentQueue canMakePayments]) {
        // Replace "Your IAP Product ID" with your actual In-App Purchase Product ID.
        SKPayment *paymentRequest = [SKPayment paymentWithProductIdentifier: @"removead"];
        
        // Request a purchase of the selected item.
        [[SKPaymentQueue defaultQueue] addPayment:paymentRequest];
    }
}

- (void) restoreInApp {
    if ([SKPaymentQueue canMakePayments]) {
        // Request a purchase of the selected item.
        [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    }
}

- (BOOL) isRemovedAds {
//    NSUserDefaults* user = [NSUserDefaults standardUserDefaults];
//    if ([user objectForKey:@"gnt_removeads"] != nil) {
//        NSString* statusRemoveAds = (NSString*) [user objectForKey:@"gnt_removeads"];
//        if (strcmp([statusRemoveAds UTF8String], "1") == 0) {
//            return YES;
//        }
//    }
    return NO;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    inappObserver = [[InAppPurchaseObserver alloc] init];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:inappObserver];
    
    if ([self isRemovedAds] == false) {
        bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerLandscape];
        //    bannerView_.layer.anchorPoint = CGPointMake(0, 1);
        //    bannerView_.layer.position = CGPointMake(0, self.view.frame.size.height);
        bannerView_.adUnitID = @"ca-app-pub-1701972712021698/7608638168";
        bannerView_.rootViewController = self;
        [self.view addSubview:bannerView_];
        [bannerView_ loadRequest:[self request]];
        
        interstitial_ = nil;
        [self showFullscreenAd];
    }
}

- (GADRequest*) request {
    GADRequest *request = [GADRequest request];
    request.testDevices = @[
                            GAD_SIMULATOR_ID,
                            @"82cbbd9e1063c1de632c5eb0e39774ea"
                            ];
    return request;
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)interstitial {
    loadedFullscreen = true;
}

- (void)interstitial:(GADInterstitial *)interstitial
didFailToReceiveAdWithError:(GADRequestError *)error {
    
}

- (void)interstitialWillPresentScreen:(GADInterstitial *)interstitial {
    
}

- (void)interstitialWillDismissScreen:(GADInterstitial *)interstitial {
    
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)interstitial {
    
}

- (void)interstitialWillLeaveApplication:(GADInterstitial *)interstitial {
    
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [bannerView_ release];
    [super dealloc];
}


@end
