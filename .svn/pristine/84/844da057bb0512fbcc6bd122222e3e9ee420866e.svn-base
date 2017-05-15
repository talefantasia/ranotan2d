//
//  BookMarkTableLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/19.
//
//

#ifndef __Ranotan2D__BookMarkTableLayer__
#define __Ranotan2D__BookMarkTableLayer__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "ItemViewScene.h"


USING_NS_CC;
USING_NS_CC_EXT;


class BookMarkTableLayer :  public CCLayerColor,
                            public CCTableViewDataSource,
                            public CCTableViewDelegate
{
public:
    CC_SYNTHESIZE_RETAIN(CCArray*, itemList, ItemList);
    CC_SYNTHESIZE_RETAIN(CCArray*, planList, PlanList);
    CC_SYNTHESIZE_RETAIN(CCArray*, bookmarkList, BookmarkList);
    CC_SYNTHESIZE_RETAIN(CCTableView*, table, Table);
    CC_SYNTHESIZE(BookmarkType, bookmarkType, BookmarkType);
    CC_SYNTHESIZE(int, listIndex, ListIndex);
    CC_SYNTHESIZE(int, deleteIndex, DeleteIndex);
    
    unsigned int itemCnt;
    
    BookMarkTableLayer();
    virtual ~BookMarkTableLayer();
    virtual bool init(BookmarkType type);
    static BookMarkTableLayer* create(BookmarkType type);
    
    //CCTableViewDataSource Virtual Method
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    //CCTableViewDelegate Virtual Method
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    void moveCallback();

private:
    void initData();
    void createTable();

    int includeKeyWord(const char* name, ListType type);
    CCArray* createIncludePlans(const char* name);
};
#endif /* defined(__Ranotan2D__BookMarkTableLayer__) */
