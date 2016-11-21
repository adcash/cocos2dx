//
//  InterstitialScene.cpp
//  AdcashExample
//
//
//

#include "InterstitialScene.h"
#include "PluginManager.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d::plugin;

Scene* InterstitialScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InterstitialScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool InterstitialScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create();
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, 1);
    
    Point beginPos = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50);
    float step = 60.0f;
    
    std::string menuItems[] = {"Show Interstitial", "Back"};
    
    int nCaseCount = sizeof(menuItems) / sizeof(std::string);
    for (int i = 0; i < nCaseCount; ++i) {
        std::string caseName = menuItems[i];
        MenuItemFont *pItem = MenuItemFont::create(caseName.c_str(), CC_CALLBACK_1(InterstitialScene::menuCallback, this));
        pItem->setTag(i);
        pItem->setPosition(Point(beginPos.x, beginPos.y - i * step));
        pMenu->addChild(pItem);
    }
    
   _adcashSDK = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdcashSDK_plugin"));
   TAdsDeveloperInfo devInfo = TAdsDeveloperInfo();
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   devInfo["interstitialZoneId"] = "409357";
#endif
     */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   devInfo["interstitialZoneId"] = "1396575";
#endif
   _adcashSDK->configDeveloperInfo(devInfo);
   
    return true;
}

void InterstitialScene::menuCallback(Ref* pSender)
{
    MenuItemFont *pItem = (MenuItemFont*) pSender;
    switch (pItem->getTag()) {
        case 0:
            // Show interstitial
            InterstitialScene::showInterstitial();
            return;
        default:
            break;
    }
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void InterstitialScene::showInterstitial()
{
    TAdsInfo info = TAdsInfo();
    info["adType"] = "2";
    _adcashSDK->showAds(info, ProtocolAds::AdsPos::kPosBottom);
}



