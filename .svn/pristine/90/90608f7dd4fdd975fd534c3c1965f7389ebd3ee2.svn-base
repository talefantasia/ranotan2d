//
//  UserDataDefault.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/10.
//
//

#ifndef __Ranotan2D__UserDataDefault__
#define __Ranotan2D__UserDataDefault__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

class UserDataDefault : public CCObject
{
public:
    UserDataDefault();
    virtual ~UserDataDefault();
    virtual bool init();
    CREATE_FUNC(UserDataDefault);

    static UserDataDefault* sharedInstance();

    //発売予定の格納スペース
    int getTargetProcess();
    int getNextTargetProcess();
    CCString* getNowTarget();
    std::string getNowTargetDirPath();
    std::string getNowTargetSaveFilePath();
    std::string getNowTargetImgDirPath();
    CCString* getNextTarget();
    std::string getNextTargetDirPath();
    std::string getNextTargetSaveFilePath();

    //ブックマークの格納スペース
    std::string getBookMarkDirPath();
    std::string getBookMarkSaveFilePath();

    //検索用のテンポラリスペース
    std::string getSearchDirPath();

    //データ更新が成功したかどうか
    bool getUpdateResult();
    void modifyUpdateResult(bool result);
  
    //データ更新日時の更新
    void modifyUpdateDate();
    CCString* getUpdateDate();
    bool isUpdate();
    
    //データ更新確認日
    void modifyUpdateConfirmDate();
    CCString* getUpdateConfirmDate();
    bool isUpdateConfirm();
    
    //作家・イラストの格納場所
    std::string getCreatorDirPath();
    std::string getCreatorSaveFilePath();
    
    //詳細の説明を開いているか
    bool getSettingContent();
    void setSettingContent(bool setting);
    
    //詳細の詳細を開いているか
    bool getSettingInfo();
    void setSettingInfo(bool setting);

    //詳細のナビを開いているか
    bool getSettingNavi();
    void setSettingNavi(bool setting);

};

#endif /* defined(__Ranotan2D__UserDataDefault__) */
