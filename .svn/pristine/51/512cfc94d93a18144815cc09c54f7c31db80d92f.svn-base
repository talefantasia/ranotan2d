//
//  NotificationManager.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/02.
//
//

#ifndef __Ranotan2D__NotificationManager__
#define __Ranotan2D__NotificationManager__

#include <string>

#define TAG_NOTIFICATION_ITEM (900001)
#define TAG_NOTIFICATION_LABEL (900002)

#define MESSAGE_TEMPLATE_ITEM "本日は【 %s 】の発売日です！"
#define MESSAGE_TEMPLATE_LABEL "本日は%sの発売日です！"

class NotificationManager {
public:
    
    /**
     * Show local notification
     * @param message  Message should be shown in notificatin
     * @param interval Interval in seconds
     * @param tag      Tag to specify notification
     */
    static void show(std::string message, std::string interval, int tag);
    
    /**
     * Cancel local notification specified by ID
     * @param tag Tag of local notification
     */
    static void cancel(int tag);
};

#endif /* defined(__Ranotan2D__NotificationManager__) */
