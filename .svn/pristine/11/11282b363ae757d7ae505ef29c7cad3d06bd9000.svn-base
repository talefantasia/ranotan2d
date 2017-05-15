//
//  CreatorFactory.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/06/21.
//
//

#include "CreatorsDataFactory.h"
#include "UserDataDefault.h"
#include "ViewItemFactory.h"
#include "UserItemFactory.h"

#define KEY_NAME_AUTHOR_LIST "KEY_AUTHOR_LIST"
#define KEY_NAME_CREATOR_LIST "KEY_CREATOR_LIST"


#define KEY_NAME_CRETOR_NAME            "name"
#define KEY_NAME_CRETOR_PROF_ITEMID     "itemid"
#define KEY_NAME_CRETOR_PLAN_CNT        "planCnt"
#define KEY_NAME_CRETOR_BOOKMARK_CNT    "bookmarkCnt"

CreatorsDataFactory* creatorsDataFactory = NULL;

CreatorsDataFactory::CreatorsDataFactory()
{
    plans=NULL;
    bookmarks=NULL;
    authors=NULL;
    creators=NULL;
    listIndex=0;
}

CreatorsDataFactory::~CreatorsDataFactory()
{
    CC_SAFE_RELEASE_NULL(plans);
    CC_SAFE_RELEASE_NULL(bookmarks);
    CC_SAFE_RELEASE_NULL(authors);
    CC_SAFE_RELEASE_NULL(creators);
    listIndex=0;
}

bool CreatorsDataFactory::init()
{
    
    setPlans(ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate());
    setBookmarks(UserItemFactory::sharedInstance()->getUserItemListOrderbyRegdt());
    setAuthors(getAuthorList());
    setCreators(getCreatorList());
    return true;
    
}

CreatorsDataFactory* CreatorsDataFactory::sharedInstance()
{
    if (!creatorsDataFactory) {
        creatorsDataFactory = CreatorsDataFactory::create();
        CC_SAFE_RETAIN(creatorsDataFactory);
    }
    return creatorsDataFactory;
    
}

CCDictionary* CreatorsDataFactory::getCreatorsList(CREATOR_TYPE type)
{
    if ( bookmarks==NULL || bookmarks->count()==0 ) {
        return CCDictionary::create();
    }
    CCDictionary* dic = CCDictionary::create();

    for (int i=0; i<bookmarks->count(); i++) {
        ViewItem* item = (ViewItem*)bookmarks->objectAtIndex(i);
        CCString* keyVal = (type==CREATOR_TYPE_AUTHOR) ? item->getAuther(): item->getCreator();
        if ( keyVal==NULL || keyVal->length()==0 ) {
            continue;
        }
        dic->setObject(item->getItemid(), keyVal->m_sString);
    }
    return dic;
}

CCArray* CreatorsDataFactory::getCheckList()
{
    CCArray* checks = CCArray::create();
    for (int i=0; i<plans->count(); i++) {
        ViewItem* plan = (ViewItem*)plans->objectAtIndex(i);
        for (int j=0; j<bookmarks->count(); j++) {
            ViewItem* bookmark = (ViewItem*)bookmarks->objectAtIndex(j);
            if ( strcmp(plan->getItemid()->getCString(), bookmark->getItemid()->getCString())==0 ) {
                checks->addObject(plan);
                break;
            }
            if ( plan->getAuther()->length()!=0 && strcmp(plan->getAuther()->getCString(), bookmark->getAuther()->getCString())==0 ) {
                checks->addObject(plan);
                break;
            }
            if ( plan->getCreator()->length()!=0 && strcmp(plan->getCreator()->getCString(), bookmark->getCreator()->getCString())==0 ) {
                checks->addObject(plan);
                break;
            }
        }
    }
    return checks;
}
