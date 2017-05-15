//
//  BrowserLauncher.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/06/28.
//
//

#include "BrowserLauncher.h"
#include "BrowserLauncher_objc.h"

static void static_LaunchUrl(const char* pszUrl)
{
    [BrowserLauncher launchUrl:[NSString stringWithUTF8String: pszUrl]];
}

namespace Cocos2dExt
{
    void BrowserLauncher::launchUrl(const char* pszUrl)
    {
        static_LaunchUrl(pszUrl);
    }
}