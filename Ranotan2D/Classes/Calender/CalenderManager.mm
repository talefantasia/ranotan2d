//
//  CalenderManager.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/05/08.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "CalenderManager.h"
#include <EventKit/EventKit.h>

void CalenderManager::checkPrivacy()
{
    EKEventStore *eventStore = [[EKEventStore alloc] init];
    
    EKAuthorizationStatus status = [EKEventStore authorizationStatusForEntityType:EKEntityTypeEvent];
    
    switch (status) {
        case EKAuthorizationStatusNotDetermined:
            [eventStore requestAccessToEntityType:EKEntityTypeEvent
                                       completion:^(BOOL granted, NSError *error)
             {
                 if(granted){
                 }else{
                 }
             }];
            break;
        case EKAuthorizationStatusDenied:
        case EKAuthorizationStatusRestricted:
            break;
        case EKAuthorizationStatusAuthorized:
            break;
        default:
            break;
    }

}

bool CalenderManager::addCalender(const char* title, const char* date)
{
    NSString* nsTitle = [NSString stringWithCString:title encoding:NSUTF8StringEncoding];
    NSString* nsDate = [NSString stringWithCString:date encoding:NSUTF8StringEncoding];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSDate* dateTitle = [formatter dateFromString:nsDate];
    
    bool result = false;
    EKEventStore *eventStore = [[EKEventStore alloc] init];
    
    EKAuthorizationStatus status = [EKEventStore authorizationStatusForEntityType:EKEntityTypeEvent];
    
    switch (status) {
        case EKAuthorizationStatusNotDetermined:
            break;
        case EKAuthorizationStatusDenied:
        case EKAuthorizationStatusRestricted:
            result=false;
            break;
        case EKAuthorizationStatusAuthorized:
        {
            EKEvent *event = [EKEvent eventWithEventStore:eventStore];
            
            event.title = nsTitle;
            event.timeZone = [NSTimeZone defaultTimeZone];
            event.startDate = dateTitle;
            event.endDate = event.startDate;
            event.allDay = YES;
            event.calendar = eventStore.defaultCalendarForNewEvents;
            
            NSTimeInterval timer = 60 * 60 * 10;
            EKAlarm * alerm = [EKAlarm alarmWithRelativeOffset:timer];
            [event addAlarm:alerm];
            
            NSError *error = NULL;
            result = [eventStore saveEvent:event
                                           span:EKSpanThisEvent
                                         commit:YES
                                          error:&error];
            NSString *message = nil;
            if(!result){
                return result;
            }
            message = [NSString stringWithFormat:@"発売日をカレンダーへ登録しました。"];
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
                                                            message:message
                                                           delegate:nil
                                                  cancelButtonTitle:nil
                                                  otherButtonTitles:@"OK", nil];
            [alert show];
        }
            break;
        default:
            break;
    }
    return result;
}

void CalenderManager::fetchEventsAndDisplay()
{
}

void CalenderManager::presentError()
{
}

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS