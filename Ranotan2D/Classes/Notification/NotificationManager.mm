//
//  NotificationManager.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/02.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "NotificationManager.h"


void NotificationManager::show(std::string message, std::string datestr, int tag)
{
    
    NSDate* date_converted;
    NSString* date_source = [NSString stringWithCString:datestr.c_str()
                                               encoding:[NSString defaultCStringEncoding]];
    
    // NSDateFormatter を用意します。
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    
    // 変換用の書式を設定します。
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    
    // NSString を NSDate に変換します。
    date_converted = [formatter dateFromString:date_source];
    
    // 使い終わった NSDateFormatter を解放します。
    [formatter release];
    
    // 通知を作成する
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    
    notification.fireDate = date_converted;
    notification.timeZone = [NSTimeZone defaultTimeZone];
 //   notification.alertBody = [NSString stringWithCString:message.c_str()                                                encoding:[NSString defaultCStringEncoding]];
    notification.alertBody = [NSString stringWithUTF8String:message.c_str()                               ];
    notification.alertAction = @"Open";
    notification.soundName = UILocalNotificationDefaultSoundName;
    
    NSNumber* tag1 = [NSNumber numberWithInteger:tag];
    NSDictionary *infoDict = [NSDictionary dictionaryWithObject:tag1 forKey:@"ID"];
    notification.userInfo = infoDict;
    
    // 通知を登録する
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    
    [notification release];
}

void NotificationManager::cancel(int tag)
{
    for(UILocalNotification *notification in [[UIApplication sharedApplication] scheduledLocalNotifications]) {
        if([[notification.userInfo objectForKey:@"ID"] integerValue] == tag) {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS