//
//  LabelNoticeLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/08/14.
//
//

#ifndef __Ranotan2D__LabelNoticeLayer__
#define __Ranotan2D__LabelNoticeLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define TAG_DIALOG_BG   (10001)

class LabelNoticeLayer : public CCLayerColor
{
public:
    LabelNoticeLayer();
    virtual ~LabelNoticeLayer();
    virtual bool init();
    static LabelNoticeLayer* create();
    
    
    
    //Touch
    void registerWithTouchDispatcher();
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    //CCTableViewDataSource Virtual Method
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    //CCTableViewDelegate Virtual Method
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
private:
    
    void initLayout();
    
    void closeButtonCallback();
    void closeDialog();
        
};

#endif /* defined(__Ranotan2D__LabelNoticeLayer__) */
