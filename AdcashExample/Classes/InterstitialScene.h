//
//  InterstitialScene.h
//  AdcashExample
//

#ifndef __AdcashExample__InterstitialScene__
#define __AdcashExample__InterstitialScene__

#include "cocos2d.h"
#include "ProtocolAds.h"

class InterstitialScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(InterstitialScene);
    
private:
    cocos2d::plugin::ProtocolAds *_adcashSDK = NULL;
    
    void showInterstitial();
};

#endif /* defined(__AdcashExample__InterstitialScene__) */
