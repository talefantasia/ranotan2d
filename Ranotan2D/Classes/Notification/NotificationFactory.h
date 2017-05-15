//
//  NotificationFactory.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/02.
//
//

#ifndef __Ranotan2D__NotificationFactory__
#define __Ranotan2D__NotificationFactory__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "ViewItemFactory.h"

USING_NS_CC;
USING_NS_CC_EXT;

class NotificationFactory{
public:
    
    NotificationFactory();
    virtual ~NotificationFactory();
    static void setNotification();
    static void createNotificationItem();
    static void createNotificationLabel();
    static CCString* checkItemCommon(ViewItem* item);
    
    static std::string createNotificationItemInApp();
    static std::string createNotificationLabelInApp();
    
private:
    void createNotificationCreator();
    
    
};

#endif /* defined(__Ranotan2D__NotificationFactory__) */
