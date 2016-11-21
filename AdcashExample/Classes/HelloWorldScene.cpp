#include "HelloWorldScene.h"
#include "ProtocolAds.h"
#include "PluginManager.h"
#include "BannerScene.h"
#include "InterstitialScene.h"

USING_NS_CC;
using namespace cocos2d::plugin;

ProtocolAds *_adcashSDK = NULL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    _adcashSDK = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdcashSDK_plugin"));
    TAdsDeveloperInfo devInfo = TAdsDeveloperInfo();
    devInfo["bannerZoneId"] = "1253052";
    devInfo["interstitialZoneId"] = "1396575";
    _adcashSDK->configDeveloperInfo(devInfo);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create();
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, 1);
    
    Point beginPos = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50);
    float step = 60.0f;
    
    std::string menuItems[] = {"Banner", "Interstitial"};
    
    int nCaseCount = sizeof(menuItems) / sizeof(std::string);
    for (int i = 0; i < nCaseCount; ++i) {
        std::string caseName = menuItems[i];
        MenuItemFont *pItem = MenuItemFont::create(caseName.c_str(), CC_CALLBACK_1(HelloWorld::menuCallback, this));
        pItem->setTag(i);
        pItem->setPosition(Point(beginPos.x, beginPos.y - i * step));
        pMenu->addChild(pItem);
    }
    return true;
}

void HelloWorld::menuCallback(Ref* pSender)
{
    MenuItemFont *pItem = (MenuItemFont*) pSender;
    Scene* newScene = NULL;
    switch (pItem->getTag()) {
        case 0:
            newScene = BannerScene::createScene();
            break;
        case 1:
            newScene = InterstitialScene::createScene();
            break;
        default:
            break;
    }
    
    if (newScene) {
        Director::getInstance()->replaceScene(newScene);
    } else {
        HelloWorld::menuCloseCallback(NULL);
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
