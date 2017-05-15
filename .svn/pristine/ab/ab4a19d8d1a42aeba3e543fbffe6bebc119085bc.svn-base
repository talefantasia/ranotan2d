//
//  NotificationFactory.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/02.
//
//

#include "NotificationFactory.h"
#include "UserItemFactory.h"
#include "DateTimeUtil.h"
#include "NotificationManager.h"

NotificationFactory::NotificationFactory()
{
}

NotificationFactory::~NotificationFactory()
{
}

void NotificationFactory::setNotification()
{
    createNotificationItem();
    createNotificationLabel();
}


void NotificationFactory::createNotificationItem()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return;
#endif
    CCArray* ary = UserItemFactory::sharedInstance()->getUserItemList();
    if ( ary==NULL || ary->count()==0 ) {
        return;
    }
    
    ViewItem* notiItem = NULL;
    
    for (int i=0; i<ary->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(ary->objectAtIndex(i));
        if ( !item ) {
            continue;
        }
        
        if ( !DateTimeUtil::isBefore(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
            continue;
        }
        //設定時刻がNULLならばそのまま代入しておく。
        if ( !notiItem ) {
            notiItem=item;
            continue;
        }
        if ( DateTimeUtil::isBefore(notiItem->getReleaseDate(), item->getReleaseDate()) ) {
            continue;
        }
        notiItem=item;
        
    }
    if ( !notiItem ) {
        return;
    }
    CCLOG("CreateNotificationAuthor title %s, date %s", notiItem->getTitle()->getCString(), notiItem->getReleaseDate()->getCString());
    
    CCString* message = CCString::createWithFormat(MESSAGE_TEMPLATE_ITEM, notiItem->getTitle()->getCString());
    
    std::string notidate = notiItem->getReleaseDate()->m_sString;
//    std::string notidate = "2014-01-01";
    notidate += " 09:00";
    
    NotificationManager::cancel(TAG_NOTIFICATION_ITEM);
    NotificationManager::show(message->m_sString, notidate, TAG_NOTIFICATION_ITEM);
}

void NotificationFactory::createNotificationLabel()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return;
#endif
    CCArray* ary = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    if ( ary==NULL || ary->count()==0 ) {
        return;
    }
    
    ViewItem* notiItem = NULL;
    
    for (int i=0; i<ary->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(ary->objectAtIndex(i));
        if ( !item ) {
            continue;
        }
        
        if ( !DateTimeUtil::isBefore(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
            continue;
        }
        //発売日順で並んでいるので最初の物がそのまま直近の発売日になる。
        if ( !notiItem ) {
            notiItem=item;
            break;
        }
    }
    if ( !notiItem ) {
        return;
    }
    CCLOG("CreateNotificationLabel label %s, date %s", notiItem->getLabel()->getCString(), notiItem->getReleaseDate()->getCString());
    
    CCString* message = CCString::createWithFormat(MESSAGE_TEMPLATE_LABEL, notiItem->getLabel()->getCString());
    
    std::string notidate = notiItem->getReleaseDate()->m_sString;
//    std::string notidate = "2014-01-01";
    notidate += " 09:00";
    
    NotificationManager::cancel(TAG_NOTIFICATION_LABEL);
    NotificationManager::show(message->m_sString, notidate, TAG_NOTIFICATION_LABEL);

}

CCString* NotificationFactory::checkItemCommon(ViewItem *item)
{
    CCArray* planList = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    
    if (  !planList || planList->count()==0 ) {
        return NULL;
    }
    
    for (int i=0; planList->count(); i++) {
        ViewItem* planItem = dynamic_cast<ViewItem*>(planList->objectAtIndex(i));
        if ( !planItem ) {
            continue;
        }
        
        if ( !strcmp(planItem->getItemid()->getCString(), item->getItemid()->getCString()) ) {
            continue;
        }
        
        if (DateTimeUtil::isAfter(item->getReleaseDate(), planItem->getReleaseDate())) {
            return planItem->getReleaseDate();
        }
    }
    return NULL;
}

std::string NotificationFactory::createNotificationItemInApp()
{
    std::string beforeDate = CCUserDefault::sharedUserDefault()->getStringForKey("LastDate", "");
    if ( beforeDate.length()==0 ) {
        return "";
    }
    CCString* now = DateTimeUtil::getNowDateTime();
    
//    beforeDate = "2014-03-24";
    
    CCArray* ary = UserItemFactory::sharedInstance()->getUserItemList();
    if ( ary==NULL || ary->count()==0 ) {
        return "";
    }
    
    std::string notice;
    std::string noticeHeader = "前回の起動以降、以下のブックマークが発売されています。\n";
    
    for (int i=0; i<ary->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(ary->objectAtIndex(i));
        if ( !item ) {
            continue;
        }
        
        if ( !DateTimeUtil::isBefore(CCString::create(beforeDate), item->getReleaseDate()) ) {
            continue;
        }
        if ( !DateTimeUtil::isBefore(item->getReleaseDate(), now) ) {
            continue;
        }
        
        notice.append("\n・");
        notice.append(item->getTitle()->getCString());
    }
    if ( notice.length()==0 ) {
        return "";
    }
    return noticeHeader.append(notice);
}

std::string NotificationFactory::createNotificationLabelInApp()
{
    std::string beforeDate = CCUserDefault::sharedUserDefault()->getStringForKey("LastDate", "");
    if ( beforeDate.length()==0 ) {
        return "";
    }
    CCString* now = DateTimeUtil::getNowDateTime();
    
    beforeDate = "2014-03-24";
    
    CCArray* ary = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    if ( ary==NULL || ary->count()==0 ) {
        return "";
    }
    
    CCDictionary* dic = CCDictionary::create();
    
    std::string notice;
    std::string noticeHeader = "前回の起動以降、以下のレーベルが発売されています。\n";
    
    for (int i=0; i<ary->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(ary->objectAtIndex(i));
        if ( !item ) {
            continue;
        }
        
        if ( !DateTimeUtil::isBefore(CCString::create(beforeDate), item->getReleaseDate()) ) {
            continue;
        }
        if ( !DateTimeUtil::isBefore(item->getReleaseDate(), now) ) {
            continue;
        }
        
        if ( dic->objectForKey(item->getLabel()->getCString()!=NULL) ) {
            continue;
        }
        
        dic->setObject(CCString::create(""), item->getLabel()->getCString());
        
        notice.append("\n・");
        notice.append(item->getLabel()->getCString());
    }
    if ( notice.length()==0 ) {
        return "";
    }
    return noticeHeader.append(notice);
}