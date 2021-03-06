//
//  DialogLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/06.
//
//

#include "DialogLayer.h"
#include "CommonButton.h"
#include "CommonDefine.h"
#include "AnimationFactory.h"
#include "AsyncSearchItem.h"
#include "SerifLayer.h"
#include "SubCharactor.h"
#include "ViewItemFactory.h"
#include "ItemViewScene.h"
#include "CreatorsDataFactory.h"

const CCSize LabelTitleSize = CCSizeMake(380, 50);
const CCSize LabelSearchSize = CCSizeMake(300, 25);
const CCSize LabelDetailSize = CCSizeMake(380, 300);
const CCSize LabelDetailSize2 = CCSizeMake(420, 450);
const CCSize LabelBookmarkTitleSize = CCSizeMake(480, 50);
const CCSize LabelBookmarkInfoSize = CCSizeMake(460, 25);
const CCSize LabelBookmarkSize = CCSizeMake(400, 75);
const CCSize ButtonBookmarkSize = CCSizeMake(450, 120);
const ccColor4B ccc4Nomal = ccc4(0, 0, 0, 128);
//const ccColor4B ccc4Nomal = ccc4(255, 255, 255, 128);
const ccColor4B ccc4Black = ccc4(0, 0, 0, 255);
const ccColor4B ccc4White = ccc4(255, 255, 255, 255);

DialogLayer::DialogLayer()
{
    p_target=NULL;
    m_doneSelector=NULL;
    editBox = NULL;
}

DialogLayer::~DialogLayer()
{
    p_target=NULL;
    m_doneSelector=NULL;
    editBox = NULL;
}

DialogLayer* DialogLayer::create(DialogType type)
{
    return create(type, NULL, NULL, NULL);
}

DialogLayer* DialogLayer::create(DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params )
{
    DialogLayer* pRet = new DialogLayer();
    if (pRet && pRet->init(type, target, doneSelector, params)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

bool DialogLayer::init(DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params)
{
    
    ccColor4B color = ccc4Nomal;
    if ( type==DialogType_Color_Black_fadeout ) color = ccc4Black;
    if ( type==DialogType_AddBookMark ) color = ccc4Black;
    if ( type==DialogType_Welcome ) color = ccc4Black;
    if ( type==DialogType_ReturnList ) color = ccc4Black;
    if ( type==DialogType_ChangeListType ) color = ccc4White;
    if ( type==DialogType_Color_White_fadeout ) color = ccc4White;
    if ( type==DialogType_Color_White_fadeinout ) color = ccc4White;
    
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    setTouchEnabled(true);
    
    p_target=target;
    m_doneSelector=doneSelector;
    
    //レイアウトの初期設定
    initLayout();
    //コンテンツ配置
    createDialog(type, target, doneSelector, params);
    //表示アニメーション
    initAction(type);
        
    return true;
}

void DialogLayer::initLayout()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //背景設置
    CCSprite* sprite = NULL;
    sprite = CCSprite::createWithSpriteFrameName("bg_common_dialog.png");
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    this->addChild(sprite, 0, TAG_DIALOG_BG);
}

void DialogLayer::createDialog(DialogLayer::DialogType type, cocos2d::CCObject *target, cocos2d::SEL_CallFunc doneSelector, cocos2d::CCDictionary *params)
{
    CCSprite* bg = dynamic_cast<CCSprite*>(getChildByTag(TAG_DIALOG_BG));
    if ( bg==NULL ) {
        return;
    }
    CCSize bgSize = bg->getContentSize();
    CCSprite* sprite = NULL;
    CCLabelTTF* label = NULL;
    CommonButton* button = NULL;
    
    switch (type) {
        case DialogType_FailItemSync:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            
            sprite = CCSprite::create("Charactor_h_350_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0-70, bgSize.height/2.0-80.0));
            sprite->setRotationY(180);
            bg->addChild(sprite);
            
            sprite = CCSprite::create("Sub_h_90_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0+100, bgSize.height/2.0-50));
            bg->addChild(sprite);
            
            sprite = CCSprite::createWithSpriteFrameName("nc71064.png");
            sprite->setPosition(ccp(bgSize.width/2.0+150, bgSize.height/2.0+20));
            sprite->setScale(0.5);
            bg->addChild(sprite);
            
            
            label = CCLabelTTF::create("データの更新", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            label = CCLabelTTF::create("データの取得が出来ませんでした。ネットワークの状態を確認して、もう一度お試しください。", FONT_W6, 24, LabelDetailSize2, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop );
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            //    label->setFontFillColor(ccc3(0, 0, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
        }
            break;
        case DialogType_UpdateConfirm:
        case DialogType_UpdateConfirmTimeLimit:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();

            
            sprite = CCSprite::create("Charactor_h_350_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0-80, bgSize.height/2.0-80.0));
            sprite->setRotationY(180);
            bg->addChild(sprite,1);

            sprite = CCSprite::create("Sub_h_90_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0+100, bgSize.height/2.0-50));
            bg->addChild(sprite,1);

            sprite = CCSprite::createWithSpriteFrameName("nc71060.png");
            sprite->setPosition(ccp(bgSize.width/2.0+150, bgSize.height/2.0+20));
            sprite->setScale(0.5);
            bg->addChild(sprite,1);

            sprite = CCSprite::create("text_update_confirm.png");
            if (type==DialogType_UpdateConfirmTimeLimit) {
                sprite = CCSprite::create("text_7days_update_confirm.png");                
            }
            sprite->setPosition(ccp( bgSize.width / 2.0, bgSize.height-sprite->getContentSize().height/2.0));
            bg->addChild(sprite);

            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_yes.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallback));
            button->setPosition(ccp( bgSize.width/3.0+20, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button, 0);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_no.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width * 2.0 /3.0-20, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button, 0);
        }
            break;
        case DialogType_Credit:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            
            CCInteger* bgType = (CCInteger*)params->objectForKey("BgType");
            
            if (bgType==NULL) {
                bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            } else if ( bgType->getValue()==1 ) {
                bg = CCSprite::create("bg_common_dialog_bookmark_author.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close_purple.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));

            } else if ( bgType->getValue()==2 ) {
                bg = CCSprite::create("bg_common_dialog_bookmark_creator.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close_green.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            }
            
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            
            sprite = CCSprite::create("text_credit.png");
            sprite->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+20));
            bg->addChild(sprite);
            
            button->setPosition(ccp( bgSize.width / 2.0, 45 ));
            button->setTouchPriority(0);
            bg->addChild(button);
        }
            break;

        case DialogType_Help:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            
            CCInteger* bgType = (CCInteger*)params->objectForKey("BgType");
            
            
            if (bgType==NULL) {
                bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            } else if ( bgType->getValue()==1 ) {
                bg = CCSprite::create("bg_common_dialog_bookmark_author.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close_purple.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            } else if ( bgType->getValue()==2 ) {
                bg = CCSprite::create("bg_common_dialog_bookmark_creator.png");
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close_green.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            }
            
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            
            sprite = CCSprite::create("text_help.png");
            sprite->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+70));
            bg->addChild(sprite);
            
            button->setPosition(ccp( bgSize.width / 2.0, 45 ));
            button->setTouchPriority(0);
            bg->addChild(button);
        }
            break;

        case DialogType_Color_White_fadeout:
        case DialogType_Color_White_fadeinout:
        case DialogType_Color_Black_fadeout:
        {
            removeChildByTag(TAG_DIALOG_BG, true);
        }
            break;
        case DialogType_FirstUpdateConfirm:
        {
            //まずは更新日時をUpdateしておく。
            UserDataDefault::sharedInstance()->modifyUpdateConfirmDate();
            UserDataDefault::sharedInstance()->modifyUpdateDate();
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            sprite = CCSprite::create("Charactor_h_350_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0-70, bgSize.height/2.0-80.0));
            sprite->setRotationY(180);
            bg->addChild(sprite);
            
            sprite = CCSprite::create("Sub_h_90_shadow.png");
            sprite->setPosition(ccp(bgSize.width/2.0+100, bgSize.height/2.0-50));
            bg->addChild(sprite);
            
            sprite = CCSprite::createWithSpriteFrameName("nc71060.png");
            sprite->setPosition(ccp(bgSize.width/2.0+150, bgSize.height/2.0+20));
            sprite->setScale(0.5);
            bg->addChild(sprite);
            
            sprite = CCSprite::create("text_firstupdate.png");
            sprite->setPosition(ccp( bgSize.width / 2.0, bgSize.height-sprite->getContentSize().height/2.0));
            bg->addChild(sprite);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_yes.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_AddBookMark:
        {
            label = CCLabelTTF::create("★ブックマークに追加★", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            label = CCLabelTTF::create("\nブックマークに追加しました。\n\nまた、発売前の作品は、カレンダーとの連携が許可されている場合のみカレンダーにも登録されます。", FONT_W6, 24, LabelDetailSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop );
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            //    label->setFontFillColor(ccc3(0, 0, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_AlreadyBookMark:
        case DialogType_BookMarkCountMax:
        {
            label = CCLabelTTF::create("★ブックマークに追加★", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            CCString* str = CCString::create("\n既に追加されています。\nブックマークに追加したアイテムはブックマークリストで確認できます。");
            if (type==DialogType_BookMarkCountMax) {
                str = CCString::create("\n登録できる上限数を超えています。\n\nブックマークに追加したアイテムはブックマークリストで確認できます。");
            }
            
            label = CCLabelTTF::create(str->getCString(), FONT_W6, 24, LabelDetailSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop );
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            //    label->setFontFillColor(ccc3(0, 0, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_DeleteBookMark:
        case DialogType_DeleteBookMarkWithoutRemove:
        {
            label = CCLabelTTF::create("★ブックマークを削除★", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            
            CCString* message = CCString::createWithFormat("\nブックマークから削除しました。\n\nただし、カレンダーとの連動データは削除されませんのでご注意ください。");
            
            label = CCLabelTTF::create(message->getCString(), FONT_W6, 24, LabelDetailSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop );
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            //    label->setFontFillColor(ccc3(0, 0, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            if ( type==DialogType_DeleteBookMark ) {
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallback));
            } else {
                button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallbackWithoutRemoveParent));
            }
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_ReturnList:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//            CCSprite* sp = CCSprite::create("Default.png");
//            sp->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
//            addChild(sp);

            removeChildByTag(TAG_DIALOG_BG, true);
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_mini.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            label = CCLabelTTF::create("リストに戻します！", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0-45, bgSize.height/2.0-6));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            sprite = CCSprite::create("rect3015_shadow.png");
            sprite->setPosition(ccp(bgSize.width-60, bgSize.height/2.0+10));
            sprite->setScale(1.0);

            bg->addChild(sprite);
            
            bg->runAction(CCSequence::create(CCDelayTime::create(0.7), CCScaleTo::create(0.3, 1.0, 0.0), NULL));
            
            
        }
            break;
        case DialogType_ChangeListType:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
        }
            break;
        case DialogType_Welcome:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_mini.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            label = CCLabelTTF::create("ようこそ！『らのたん』へ", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0-30, bgSize.height/2.0-6));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);

            sprite = CCSprite::create("rect3015_shadow.png");
            sprite->setPosition(ccp(bgSize.width-60, bgSize.height/2.0+10));
            sprite->setScale(1.0);
            bg->addChild(sprite);
            
            bg->addChild(label);
            bg->runAction(CCSequence::create(CCDelayTime::create(0.7), CCScaleTo::create(0.3, 1.0, 0.0), NULL));
        }
            break;
        case DialogType_Comeback:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);

            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_mini.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+560));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            label = CCLabelTTF::create("おかえりなさい！", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0-30, bgSize.height/2.0-6.0));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
 //           bg->runAction(CCSequence::create(CCDelayTime::create(2.0), CCScaleTo::create(0.3, 1.0, 0.0), NULL));
            sprite = CCSprite::create("rect3015_shadow.png");
            sprite->setPosition(ccp(bgSize.width-60, bgSize.height/2.0+10));
            sprite->setScale(1.0);
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCFloat::create(winSize.width/2.0), "move_posx");
            dic->setObject(CCFloat::create(winSize.height/2.0), "move_posy");
            bg->runAction(AnimationFactory::sharedInstance()->getActions("commonalertdlg2-in", dic));

            bg->addChild(sprite);

        }
            break;
        case DialogType_BuyItem:
        {
            label = CCLabelTTF::create("★購入★", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
        
            label = CCLabelTTF::create("近日実装予定！！", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+130.0));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            sprite = CCSprite::create("zannen.png");
            sprite->setPosition(ccp(bgSize.width/2.0+10.0, bgSize.height/2.0));
            bg->addChild(sprite);

            sprite = CCSprite::create("zannen_text1.png");
            sprite->setPosition(ccp(bgSize.width/2.0+160.0, bgSize.height/2.0+20));
            bg->addChild(sprite);

            sprite = CCSprite::create("zannen_text2.png");
            sprite->setPosition(ccp(bgSize.width/2.0-160.0, bgSize.height/2.0+20));
            bg->addChild(sprite);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_Search:
        {
            label = CCLabelTTF::create("★検索★", FONT_W6, 28, LabelTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            
            //作家検索ボタン　ここから
            CCString* value = (CCString*)params->objectForKey("author");
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCString::create("author"), "type");
            dic->setObject(value, "value");
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_search_author.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::search));
            button->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+110.0));
            button->setParams(dic);
            button->setTouchPriority(0);
            bg->addChild(button);
            
            label = CCLabelTTF::create(value->getCString(), FONT_W6, 20, LabelSearchSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+100));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);
            //作家検索ボタン　ここまで
            
            //絵師検索ボタン   ここから
            value = (CCString*)params->objectForKey("creator");
            dic = CCDictionary::create();
            dic->setObject(CCString::create("creator"), "type");
            dic->setObject(value, "value");
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_search_creator.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::search));
            button->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+30.0));
            button->setParams(dic);
            button->setTouchPriority(0);
            bg->addChild(button);
            
            
            label = CCLabelTTF::create(value->getCString(), FONT_W6, 20, LabelSearchSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0+20));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);

            //絵師検索ボタン   ここまで
            
            //キーワード検索　ここから
            CCSize editBoxSize = CCSizeMake(button->getContentSize().width-100, button->getContentSize().height);
            editBox = CCEditBox::create(editBoxSize, CCScale9Sprite::createWithSpriteFrameName("btn_search_keyword.png"));
            editBox->setPosition(ccp(bgSize.width/2.0-50, bgSize.height/2.0-50));
            editBox->setFontColor(ccYELLOW);
            editBox->setPlaceholderFontSize(5);
            editBox->setMaxLength(10);
            editBox->setPlaceHolder("キーワード");
            editBox->setTouchPriority(0);
            bg->addChild(editBox);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_search_keyword_go.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::searchKeyword));
            button->setPosition(ccp(bgSize.width/2.0+130, bgSize.height/2.0-50.0));
            button->setTouchPriority(0);
            bg->addChild(button);

            label = CCLabelTTF::create("※キーワード検索はタイトル名も含みます。", FONT_W6, 16, LabelTitleSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            label->setPosition(ccp(bgSize.width/2.0+20, bgSize.height/2.0-110.0));
            bg->addChild(label);

            
            //キーワード検索　ここまで
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
            
        }
            break;
        case DialogType_BookmarkAuthor:
        case DialogType_BookmarkCreator:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            CCString* title = NULL;
            CCString* name = (CCString*)params->objectForKey("name");
            CCString* itemid = (CCString*)params->objectForKey("itemid");
            CCString* buttonName = NULL;
            CCString* closeButtonName = NULL;
            CCArray* list = (CCArray*)params->objectForKey("list");
            
            if ( type==DialogType_BookmarkAuthor ) {
                bg = CCSprite::create("bg_common_dialog_bookmark_author.png");
                title = CCString::createWithFormat("%s（作家）", name->getCString());
                buttonName = CCString::create("bg_bookmark_label_author_2.png");
                closeButtonName = CCString::create("btn_common_close_purple.png");
            } else {
                bg = CCSprite::create("bg_common_dialog_bookmark_creator.png");
                title = CCString::createWithFormat("%s（イラスト）", name->getCString());
                buttonName = CCString::create("bg_bookmark_label_creator_2.png");
                closeButtonName = CCString::create("btn_common_close_green.png");
            }
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();

            CCImage *image = new CCImage();
            std::string imagePath =UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "L_" +itemid->getCString();
            bool flg = image->initWithImageFile(imagePath.c_str());
            
            image->autorelease();
            // 3. CCTexture2DからCCSpriteへ
            
            CCTexture2D *texture = new CCTexture2D();
            texture->initWithImage(image);
            texture->autorelease();
            if ( flg ) {
                CCSize size = bg->getContentSize();
                sprite = CCSprite::createWithTexture(texture);
                sprite -> setPosition(ccp(size.width/2.0, size.height/2.0-10));
                CCSize contentSize = sprite->getContentSize();
                float scaleX = 370.0 / contentSize.height;
                sprite -> setScale(scaleX);
                sprite->setOpacity(176);
                bg->addChild(sprite);
            }

            
            label = CCLabelTTF::create(title->getCString(), FONT_W6, 28, LabelBookmarkTitleSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-50));
            label->setColor(ccc3(255, 255, 0));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);

            CCString* info = CCString::create("発売予定に情報はありません。");
            if ( list->count()!=0 ) {
                info = CCString::createWithFormat("発売予定に%d件の情報があります。", list->count());
            }
            
            label = CCLabelTTF::create(info->getCString(), FONT_W6, 26, LabelBookmarkInfoSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height-100));
            label->setColor(ccc3(255, 255, 255));
            label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
            bg->addChild(label);

            for (int i=0; i<list->count(); i++) {
                if ( i==3 ) {
                    break;
                }

                button = CommonButton::create(CCScale9Sprite::create(buttonName->getCString()), CCSizeZero, this, callfuncO_selector(DialogLayer::changeDetail));
                button->setPosition(ccp(bgSize.width/2.0, bgSize.height-185-(122.0*i)));
                button->setTouchPriority(0);
                bg->addChild(button);

                CCDictionary* dic =(CCDictionary*)list->objectAtIndex(i);
                ViewItem* item = (ViewItem*)dic->objectForKey("item");
                CCInteger* idx = (CCInteger*)dic->objectForKey("idx");
                
                CCDictionary* params = CCDictionary::create();
                params->setObject(idx, "idx");
                button->setParams(params);
                
                label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 22, LabelBookmarkSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
                label->setPosition(ccp(bgSize.width/2.0, bgSize.height-175-(122.0*i)));
                label->setColor(ccc3(255, 255, 255));
                label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
                bg->addChild(label);
                label = CCLabelTTF::create("＞＞発売予定表へ", FONT_W6, 18, LabelBookmarkSize, kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
                label->setPosition(ccp(bgSize.width/2.0, bgSize.height-193-(122.0*i)));
                label->setColor(ccc3(255, 255, 0));
                label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
                bg->addChild(label);
                
                
            }
            
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName(closeButtonName->getCString()), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);

        }
            break;
        case DialogType_Content_All:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            CCString* content = (CCString*)params->objectForKey("content");
            bg = CCSprite::create("bg_common_dialog_bookmark_creator.png");
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            CCSize labelSize = CCSizeMake(bgSize.width-40, bgSize.height-60);
            
            CCLabelTTF *label = CCLabelTTF::create(content->getCString(), FONT_W6, 22, labelSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
            
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));

            bg->addChild(label);
            
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close_green.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::closeButtonCallback));
            button->setPosition(ccp( bgSize.width/2.0, 60 ));
            button->setTouchPriority(0);
            bg->addChild(button);
        }
            break;
        case DialogType_Bud_Data:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            removeChildByTag(TAG_DIALOG_BG, true);
            
            bg = CCSprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
            button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_close.png"), CCSizeZero, this, callfuncO_selector(DialogLayer::doneBtnCallbackWithoutRemoveParent));
            
            bg->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
            addChild(bg, 0, TAG_DIALOG_BG);
            bgSize=bg->getContentSize();
            
            
            button->setPosition(ccp( bgSize.width / 2.0, 45 ));
            button->setTouchPriority(0);
            bg->addChild(button);

            bgSize=bg->getContentSize();

            
            CCLabelTTF *label = CCLabelTTF::create("　サーバから取得したデータが不完全の可能性があります。２時間ほどお時間をおいて、もう一度更新をお試しください。", FONT_W6, 24, CCSizeMake(bgSize.width-50, bgSize.height-50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            bg->addChild(label);
            
            label = CCLabelTTF::create("データ更新に関するお知らせ", FONT_W6, 32, CCSizeMake(bgSize.width-50, bgSize.height-50), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
            label->setPosition(ccp(bgSize.width/2.0, bgSize.height/2.0));
            label->setColor(ccc3(255, 255, 0));
            bg->addChild(label);
        }
            break;
        default:
            break;
    }
    
}

void DialogLayer::initAction(DialogLayer::DialogType type)
{
    switch (type) {
        case DialogType_Color_White_fadeinout:
        {
            CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::closeDialog));
            runAction(CCSequence::create(CCDelayTime::create(0.05),CCFadeOut::create(0.2),func, NULL));
        }
            break;
        case DialogType_Color_White_fadeout:
        {
            CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::closeDialog));
            runAction(CCSequence::create(CCDelayTime::create(0.2),CCFadeOut::create(0.4),func, NULL));
        }
            break;
        case DialogType_ChangeListType:
        {
            CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::closeDialog));
            runAction(CCSequence::create(CCDelayTime::create(0.5),CCFadeOut::create(0.5),func, NULL));
        }
            break;
        case DialogType_ReturnList:
        case DialogType_Welcome:
        case DialogType_Color_Black_fadeout:
        {
            CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::closeDialog));
            runAction(CCSequence::create(CCDelayTime::create(0.5),CCFadeOut::create(1.0),func, NULL));

        }
            break;
        case DialogType_Comeback:
        {
            CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::doneBtnCallback));
            runAction(CCSequence::create(CCDelayTime::create(3.0),func, NULL));
            
        }
            break;
        default:
        {
            CCSprite* bg = dynamic_cast<CCSprite*>(getChildByTag(TAG_DIALOG_BG));
            if ( bg==NULL ) {
                return;
            }
            bg->setScaleY(0.0);
            bg->runAction(CCEaseIn::create(CCScaleTo::create(0.3, 1.0, 1.0),1.0));
        }
            break;
    }
    

}

void DialogLayer::doneBtnCallback()
{
    if (p_target && m_doneSelector)
    {
        (p_target->*m_doneSelector)();
    }
    if ( this==NULL ) {
        return;
    }
    removeFromParentAndCleanup(true);

}

void DialogLayer::doneBtnCallbackWithoutRemoveParent()
{
    if (p_target && m_doneSelector)
    {
        (p_target->*m_doneSelector)();
    }
}

void DialogLayer::closeButtonCallback()
{
    CCCallFunc* func = CCCallFunc::create(this,callfunc_selector(DialogLayer::closeDialog));
    CCSprite* bg = dynamic_cast<CCSprite*>(getChildByTag(TAG_DIALOG_BG));
    if ( bg==NULL ) {
        return;
    }
    bg->runAction(CCSequence::create(CCEaseOut::create(CCScaleTo::create(0.3, 1.0, 0.0),1.0), func, NULL));

}

void DialogLayer::closeDialog()
{
    removeFromParentAndCleanup(true);
}

void DialogLayer::searchKeyword()
{
    CCString* value = CCString::create(editBox->getText());
    if ( value->length()==0 ) {
        CCLog("none Search word");
        return;
    }
    

    CCString* type =CCString::create("keywords");
    
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(type, "type");
    dic->setObject(value, "value");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH);
    
    
    getParent()->addChild(AsyncSearchItem::create(p_target, m_doneSelector, dic));
    
    closeDialog();

}

void DialogLayer::search(CCObject* obj)
{
    CommonButton* button = dynamic_cast<CommonButton*>(obj);
    if ( button==NULL ) {
        return;
    }
    CCDictionary* dic = button->getParams();
    CCString* type =((CCString*)dic->objectForKey("type"));
    CCString* value =((CCString*)dic->objectForKey("value"));

    dic->setObject(type, "type");
    dic->setObject(value, "value");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH);

    
    getParent()->addChild(AsyncSearchItem::create(p_target, m_doneSelector, dic));
    
    closeDialog();
    
}

std::string DialogLayer::UrlEncode( const std::string & str )
{
    std::string retStr;
    
    std::string::size_type length = str.size();
    for ( std::string::size_type i = 0 ; i < length ; i++ )
    {
        if (( '0' <= str[ i ] && str[ i ] <= '9' ) ||
            ( 'a' <= str[ i ] && str[ i ] <= 'z' ) ||
            ( 'A' <= str[ i ] && str[ i ] <= 'Z' ) ||
            str[ i ] == '-'   || str[ i ] == '.'   ||
            str[ i ] == '_'   || str[ i ] == '~' )
        {
            // 数字/アルファベット/[-][.][_][~]はそのまま
            retStr += str[ i ];
        }
        else
        {
            retStr += '%';
            char tmp[ 3 ];
            snprintf( tmp, 3, "%X", static_cast< unsigned char >( str[ i ] ));
            retStr += tmp;  
        }  
    }  
    
    return retStr;  
}


void DialogLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool DialogLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    return true;
}

void DialogLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}

void DialogLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}

void DialogLayer::changeDetail(CCObject* index)
{
    CommonButton* button = (CommonButton*)index;
    
    CCDictionary* dic = button->getParams();
    CCInteger* idx =((CCInteger*)dic->objectForKey("idx"));
    int furcs = idx->getValue();
    
    CreatorsDataFactory::sharedInstance()->setListIndex(furcs);
    
    doneBtnCallback();
}