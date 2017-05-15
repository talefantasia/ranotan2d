
//
//  File.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/18.
//
//

#ifndef __Ranotan2D__File__
#define __Ranotan2D__File__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "ItemViewScene.h"


USING_NS_CC;
USING_NS_CC_EXT;

class ItemListTableLayer :  public CCLayerColor,
                            public CCTableViewDataSource,
                            public CCTableViewDelegate
{
public:
    
    CC_SYNTHESIZE_RETAIN(CCArray*, itemList, ItemList);
    CC_SYNTHESIZE_RETAIN(CCArray*, bookmarkList, BookmarkList);
    CC_SYNTHESIZE_RETAIN(CCTableView*, table, Table);
    CC_SYNTHESIZE(ListType, listType, ListType);
    CC_SYNTHESIZE(PlanType, planType, PlanType);
    CC_SYNTHESIZE(int, listIndex, ListIndex);
  
    unsigned int itemCnt;
    
    ItemListTableLayer();
    virtual ~ItemListTableLayer();
    virtual bool init(int index, ListType type, PlanType planType);
    static ItemListTableLayer* create(int index, ListType type, PlanType planType);
    
    //CCTableViewDataSource Virtual Method
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    //CCTableViewDelegate Virtual Method
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);

private:
    void initData();
    void createTable();
    CCPoint mathOffsetFromForcusItem(int index);
    void mathFoucsItemFromOffset();
    void update(float delta);
    bool isContain(CCString* value, int checkType);
};

#endif /* defined(__Ranotan2D__File__) */
