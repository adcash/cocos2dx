//
//  AdcashSDK_plugin.h
//  adcash-cocos2dx-plugin
//
//  Copyright (c) 2015 Adcash. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InterfaceAds.h"
#import <Adcash/AdcashSDK.h>

typedef NS_ENUM(NSUInteger, AdcashAdType)
{
    AdcashAdTypeBanner = 1,
    AdcashAdTypeInterstitial
};

@interface AdcashSDK_plugin : NSObject <InterfaceAds>

// InterfaceAds protocol
- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo;
- (void) showAds: (NSMutableDictionary*) info position:(int) pos;
- (void) hideAds: (NSMutableDictionary*) info;
- (void) queryPoints;
- (void) spendPoints: (int) points;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

@end
