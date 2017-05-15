//
//  UserDataDefault.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/10.
//
//

#include "UserDataDefault.h"
#include "DateTimeUtil.h"

#define KEY_TARGET_PROCESS "target_process"

#define FORMAT_PLAN_PATH_SUFFIX "dir%d/"
#define FORMAT_BOOKMARK_PATH "bookmark/"
#define FORMAT_SEARCH_PATH "search/"
#define SAVE_FILE_NAME_PLAN "savadata.svd"
#define SAVE_FILE_NAME_BOOKMASK "savadatabookmark.svd"
#define SAVE_FILE_NAME_CREATOR "savadatacreator.svd"

#define KEY_UPDATE_RESULT "update_result"
#define KEY_UPDATE_DATE "update_date"
#define KEY_UPDATE_CONFIRM_DATE "update_condifm_date"

#define KEY_SETTING_CONTENT_OPEN "setting_content_open"
#define KEY_SETTING_INFO_OPEN "setting_info_open"
#define KEY_SETTING_ITEMNAVI_OPEN "setting_itemnavi_open"


#define DEFAULT_UPDATE_DATE "1900-01-01"


const std::string SavePath = CCFileUtils::sharedFileUtils()->getWritablePath();

UserDataDefault* userDataDefault = NULL;

UserDataDefault::UserDataDefault()
{
    
}

UserDataDefault::~UserDataDefault()
{
    
}

UserDataDefault* UserDataDefault::sharedInstance()
{
    if (!userDataDefault) {
        userDataDefault = UserDataDefault::create();
        CC_SAFE_RETAIN(userDataDefault);
    }
    return userDataDefault;

}

bool UserDataDefault::init()
{
    return true;
}

int UserDataDefault::getTargetProcess()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_TARGET_PROCESS, 0);
}

int UserDataDefault::getNextTargetProcess()
{
    int targetProcess =CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_TARGET_PROCESS, 0);
    return (targetProcess==0) ? 1 : 0;
}


CCString* UserDataDefault::getNowTarget()
{
    CCString* path = CCString::createWithFormat(FORMAT_PLAN_PATH_SUFFIX, getTargetProcess());
    return path;
}

std::string UserDataDefault::getNowTargetDirPath()
{
   return SavePath + getNowTarget()->m_sString;
}

std::string UserDataDefault::getNowTargetSaveFilePath()
{
    return SavePath + SAVE_FILE_NAME_PLAN;
}


CCString* UserDataDefault::getNextTarget()
{
    CCString* path = CCString::createWithFormat(FORMAT_PLAN_PATH_SUFFIX, getNextTargetProcess());
    return path;
    
}

std::string UserDataDefault::getNextTargetDirPath()
{
    return SavePath + getNextTarget()->m_sString;
}

std::string UserDataDefault::getNextTargetSaveFilePath()
{
    return getNextTargetDirPath() + SAVE_FILE_NAME_PLAN;
}


std::string UserDataDefault::getBookMarkDirPath()
{
//    return SavePath + getNowTarget()->m_sString;
    return SavePath;
}

std::string UserDataDefault::getBookMarkSaveFilePath()
{
    return getBookMarkDirPath() + SAVE_FILE_NAME_BOOKMASK;
}

bool UserDataDefault::getUpdateResult()
{
    int result = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_UPDATE_RESULT, 0);
    return (result==1);
}

void UserDataDefault::modifyUpdateResult(bool result)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_UPDATE_RESULT, (result) ? 1: 0);
}

void UserDataDefault::modifyUpdateDate()
{
    CCString* date = DateTimeUtil::getNowDateTime();
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_UPDATE_DATE, date->m_sString);
}

bool UserDataDefault::isUpdate()
{
    CCString* limitDate = DateTimeUtil::getDateTime(-6);
    CCString* updateDate = CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_UPDATE_DATE, DEFAULT_UPDATE_DATE
                                                                                                ));
    if ( DateTimeUtil::isAfter(limitDate, updateDate) ) {
        return true;
    }
    return false;
}

CCString* UserDataDefault::getUpdateDate()
{
    CCString* updateDate = CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_UPDATE_DATE, "----"));
    return updateDate;
}

void UserDataDefault::modifyUpdateConfirmDate()
{
    CCString* date = DateTimeUtil::getNowDateTime();
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_UPDATE_CONFIRM_DATE, date->m_sString);
}

bool UserDataDefault::isUpdateConfirm()
{
    CCString* limitDate = DateTimeUtil::getDateTime(0);
    CCString* updateDate = CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_UPDATE_CONFIRM_DATE, DEFAULT_UPDATE_DATE));
    if ( DateTimeUtil::isAfter(limitDate, updateDate) ) {
        return true;
    }
    return false;
}

CCString* UserDataDefault::getUpdateConfirmDate()
{
    CCString* updateDate = CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey(KEY_UPDATE_CONFIRM_DATE, "----"));
    return updateDate;
}

std::string UserDataDefault::getCreatorDirPath()
{
    return SavePath;
}

std::string UserDataDefault::getCreatorSaveFilePath()
{
    return getCreatorDirPath() + SAVE_FILE_NAME_CREATOR;

}

bool UserDataDefault::getSettingContent()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_SETTING_CONTENT_OPEN);
}

void UserDataDefault::setSettingContent(bool setting)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_SETTING_CONTENT_OPEN, setting);
}

bool UserDataDefault::getSettingInfo()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_SETTING_INFO_OPEN);
}

void UserDataDefault::setSettingInfo(bool setting)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_SETTING_INFO_OPEN, setting);
}

bool UserDataDefault::getSettingNavi()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_SETTING_ITEMNAVI_OPEN);
}

void UserDataDefault::setSettingNavi(bool setting)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_SETTING_ITEMNAVI_OPEN, setting);
}