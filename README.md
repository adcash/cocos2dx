Adcash Plugin for Cocos2D-X

### Supported Platforms
 * iOS

### Plugin features include :

* Support for Banner Ads
* Support for Interstitial
* Banner positions(top/bottom)
* Ad event listeners

### And requirements are :
* Cocos2D-X
* An ad zone ID
* Target of 7.0 or higher

### Integration

1. Download or clone the repository.
2. Find your project from ios/mac folder and launch xcodeproject.
3. Drag and drop **Adcash.framework**, **Adcash.bundle** and **AdcashCocos2dXPlugin.framework** to your project and make sure you select **Copy files if needed**.

### Implementation & Configuration
Add this to your HelloWorldScene.h:

```cpp
#include "ProtocolAds.h"
...
private:
    cocos2d::plugin::ProtocolAds *_adcashSDK = NULL;
```

and add this to your HelloWorldScene.cpp:

```cpp
#include "PluginManager.h"
using namespace cocos2d::plugin;
...
bool HelloWorld::init()
{
...
_adcashSDK = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdcashSDK_plugin"));

// Configure plugin
TAdsDeveloperInfo devInfo = TAdsDeveloperInfo();
devInfo["bannerZoneId"] = "YOUR_BANNER_ZONE_ID_HERE";
devInfo["interstitialZoneId"] = "YOUR_INTERSTITIAL_ZONE_ID_HERE";
_adcashSDK->configureDeveloperInfo(devInfo);
...
```
#### Banner
Add this snippet to your HelloWorldScene.cpp:
```cpp
TAdsInfo info = TAdsInfo();
info["adType"] = "1"; // 1 for banner
info["adSize"] = "0"; // is Smart banner
_adcashSDK->showAds(info, ProtocolAds::AdsPos::kPosBottom);
```

#### Interstitial
Add this snippet to your HelloWorldScene.cpp:
```cpp
TAdsInfo info = TAdsInfo();
info["adType"] = "2" // for interstitial
_adcashSDK->showAds(info, ProtocolAds::AdsPos::kPosBottom);
// position for interstitial is ignored
```
#### Delegates
If you need to listen delegates for the ad request, do these additions;
In HelloWorldScene.h, add:
```cpp
private:
    void onAdsResult(int code, std::string& msg);
```
And in HelloWorldScene.cpp, add:
```cpp
bool HelloWorld::init()
{
...
    std::function<void(int, std::string&)> callback = std::bind(&HelloWorld::onAdsResult, this, std::placeholders::_1, std::placeholders::_2);
    _adcashSDK->setCallback(a);

...

void HelloWorld::onAdsResult(int code, std::string& msg)
{
    log("OnAdsResult, code: %d, msg: %s", code, msg.c_str());
}
```

## Support
If you need any support or assistance, you can contact us by sending email to [mobile@adcash.com](mailto:mobile@adcash.com) .