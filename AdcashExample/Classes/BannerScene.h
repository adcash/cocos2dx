//
//  BannerScene.h
//  AdcashExample
//
//  Created by Martin on 7/3/15.
//
//

#ifndef __AdcashExample__BannerScene__
#define __AdcashExample__BannerScene__

#include "cocos2d.h"
#include "ProtocolAds.h"

class BannerScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(BannerScene);
    
private:
    cocos2d::plugin::ProtocolAds *_adcashSDK = NULL;
    
    void showBanner(int adSize, cocos2d::plugin::ProtocolAds::AdsPos pos);
    void hideBanner();
    void onAdsResult(int code, std::string& msg);
};


#endif /* defined(__AdcashExample__BannerScene__) */
