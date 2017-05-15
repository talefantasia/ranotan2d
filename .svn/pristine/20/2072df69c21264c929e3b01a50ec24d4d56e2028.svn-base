//
//  BrowserLauncher_objc.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/06/28.
//
//

#include "BrowserLauncher_objc.h"
#import "EAGLView.h"

@implementation BrowserLauncher

static UIWebView *webview = nil;

+ (void)launchUrl:(NSString *)pszUrl {
    NSURL *url = [NSURL URLWithString:pszUrl];
    [[UIApplication sharedApplication] openURL:url];
}

@end
