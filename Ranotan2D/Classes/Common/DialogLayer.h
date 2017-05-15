//
//  DialogLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/06.
//
//

#ifndef __Ranotan2D__DialogLayer__
#define __Ranotan2D__DialogLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define TAG_DIALOG_BG   (10001)

class DialogLayer : public CCLayerColor
{
public:
    enum DialogType
    {
        DialogType_UpdateConfirm = 0,
        DialogType_Color_Black_fadeout = 1,
        DialogType_Color_White_fadeout = 2,
        DialogType_FirstUpdateConfirm = 3,
        DialogType_ReturnList   = 4,
        DialogType_Welcome= 5,
        DialogType_Comeback= 6,
        DialogType_AddBookMark= 7,
        DialogType_AlreadyBookMark= 8,
        DialogType_ChangeListType= 9,
        DialogType_DeleteBookMark= 10,
        DialogType_DeleteBookMarkWithoutRemove= 11,
        DialogType_BuyItem= 12,
        DialogType_Search=13,
        DialogType_Credit=14,
        DialogType_FailItemSync=15,
        DialogType_Help=16,
        DialogType_UpdateConfirmTimeLimit = 17,
        DialogType_BookmarkAuthor = 18,
        DialogType_BookmarkCreator = 19,
        DialogType_BookMarkCountMax= 20,
        DialogType_Color_White_fadeinout = 21,
        DialogType_Content_All = 22,
        DialogType_Bud_Data = 23,
    };
    DialogLayer();
    virtual ~DialogLayer();
    virtual bool init(DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params);
    static DialogLayer* create(DialogType type);
    static DialogLayer* create(DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params );    
    
    //Touch
    void registerWithTouchDispatcher();
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

    
private:
    CCEditBox* editBox;
    CCObject* p_target;
    SEL_CallFunc m_doneSelector;
    
    std::string UrlEncode( const std::string & str );
    void initLayout();
    void createDialog(DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params);
    void initAction(DialogLayer::DialogType type);
    
    void doneBtnCallback();
    void doneBtnCallbackWithoutRemoveParent();
    void closeButtonCallback();
    void closeDialog();
    
    void search(CCObject* obj);
    void searchKeyword();
    
    void changeDetail(CCObject* index);
    
};

#endif /* defined(__Ranotan2D__DialogLayer__) */
