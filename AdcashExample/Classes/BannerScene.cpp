//
//  BannerScene.cpp
//  AdcashExample
//


#include "BannerScene.h"
#include "PluginManager.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d::plugin;

Scene* BannerScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = BannerScene::create();
    
    scene->addChild(layer);

    return scene;
}

bool BannerScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    // CREATE THE EMPTY MENU
    Menu* pMenu = Menu::create();
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, 1);
    
    Point beginPos = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50);
    float step = 60.0f;
    
    // ADD ITEMS TO MENU
    
    std::string menuItems[] = {"Show Banner", "Hide banner", "Back"};
    
    int nCaseCount = sizeof(menuItems) / sizeof(std::string);
    for (int i = 0; i < nCaseCount; ++i) {
        std::string caseName = menuItems[i];
        MenuItemFont *pItem = MenuItemFont::create(caseName.c_str(), CC_CALLBACK_1(BannerScene::menuCallback, this));
        pItem->setTag(i);
        pItem->setPosition(Point(beginPos.x, beginPos.y - i * step));
        pMenu->addChild(pItem);
    }
    
    std::function<void(int, std::string&)> a = std::bind(&BannerScene::onAdsResult, this, std::placeholders::_1, std::placeholders::_2);
    _adcashSDK = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdcashSDK_plugin"));
    _adcashSDK->setCallback(a);
    TAdsDeveloperInfo devInfo = TAdsDeveloperInfo();
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    devInfo["bannerZoneId"] = "409358";
#endif
     */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    devInfo["bannerZoneId"] = "1253052";
#endif
    _adcashSDK->configDeveloperInfo(devInfo);
    
    return true;
}

void BannerScene::menuCallback(Ref* pSender)
{
    MenuItemFont *pItem = (MenuItemFont*) pSender;
    switch (pItem->getTag()) {
        case 0:
            // Show banner at the bottom
            BannerScene::showBanner(0, ProtocolAds::AdsPos::kPosBottom);
            return;
        case 1:
            // Hide banner
            BannerScene::hideBanner();
            return;
        default:
            // Back
            Director::getInstance()->replaceScene(HelloWorld::createScene());
            break;
    }
}

void BannerScene::showBanner(int adSize, ProtocolAds::AdsPos pos)
{
    TAdsInfo info = TAdsInfo();
    info["adType"] = "1";
    info["adSize"] = std::to_string(adSize);
    _adcashSDK->showAds(info, pos);
}

void BannerScene::hideBanner()
{
    TAdsInfo info = TAdsInfo();
    info["adType"] = "1";
    _adcashSDK->hideAds(info);
}

void BannerScene::onAdsResult(int code, std::string& msg)
{
    log("OnAdsResult, code : %d, msg : %s", code, msg.c_str());
}
