//
//  CommonDefine.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/11.
//
//

#ifndef Ranotan2D_CommonDefine_h
#define Ranotan2D_CommonDefine_h

#import "cocos2d.h"
#import "UserDataDefault.h"

// リリースビルドを作る時はコメントアウトする
// 本当にリリースする時は、iOS版はReleaseビルドをしてプログラムを最適化する
//#define DEBUG

#ifdef DEBUG
#define COCOS2D_DEBUG 1
#else
#define NDEBUG
#endif

//Layer操作
#define NOTIFICATION_LAYER_CONTROLL_DETAIL_CAMERA "NOTIFICATION_LAYER_CONTROLL_DETAIL_CAMERA"

//キャラクターレイヤー
#define NOTIFICATION_CHARACTORLAYER_EDIT_MAGICAL_CIRCLE "NOTI_CHARACTORLAYER_EDIT_MAGICAL_CIRCLE"
#define NOTIFICATION_CHARACTORLAYER_RESET_TOTAL_DELTA "NOTI_CHARACTORLAYER_RESET_TOTAL_DELTA"

//メインキャラクター
#define NOTIFICATION_CHARACTOR_SUBEFFECT_NONE "NOTI_CHARACTOR_SUBEFFECT_NONE"
#define NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH "NOTI_CHARACTOR_SUBEFFECT_SEARCH"
#define NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_IMAGE "NOTI_CHARACTOR_SUBEFFECT_SEARCH_IMAGE"
#define NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_COMPLETE "NOTI_CHARACTOR_SUBEFFECT_SEARCH_COMPLETE"
#define NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_CANCEL "NOTI_CHARACTOR_SUBEFFECT_SEARCH_CANCEL"

//サブキャラクター
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_ROLLING "NOTI_SUBCHARACTOR_ANIMATION_ROLLING"
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP "NOTI_SUBCHARACTOR_ANIMATION_JUMP"
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP_CHECK "NOTI_SUBCHARACTOR_ANIMATION_JUMP_CHECK"
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMPDANCE "NOTI_SUBCHARACTOR_ANIMATION_JUMPDANCE"
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_STARLIGHT "NOTI_SUBCHARACTOR_ANIMATION_STARLIGHT"
#define NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE "NOTI_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define FONT_W3 "HiraKakuProN-W3"
#define FONT_W6 "HiraKakuProN-W6"
#else
#define FONT_W3 "DroidSans"
#define FONT_W6 "DroidSans"
#endif

#define MAX_COUNT_USER_ITEM (100)

#define winWidth 640
#define winHeightMin 960
extern double winHeight;
extern double displayScale;
extern int displayHorizontalMargin;
extern bool isParticleDisp;
extern bool isHighResolution;



#endif
