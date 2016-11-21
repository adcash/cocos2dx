//
//  AdcashSDK_plugin.m
//  adcash-cocos2dx-plugin
//
//  Copyright (c) 2015 Adcash. All rights reserved.
//

#import "AdcashSDK_plugin.h"
#import "AdsWrapper.h"

#define DLog(...) NSLog(__VA_ARGS__);

@interface AdcashSDK_plugin () <ADCBannerViewDelegate, ADCInterstitialDelegate>

@property (nonatomic, strong) ADCBannerView * bannerView;
@property (nonatomic, strong) ADCInterstitial * interstitial;
@property (nonatomic, copy) NSString * bannerZoneId;
@property (nonatomic, copy) NSString * interstitialZoneId;
@end

@implementation AdcashSDK_plugin

- (void)configDeveloperInfo:(NSMutableDictionary *)devInfo
{
    DLog(@"Dev info: %@", devInfo);
    self.bannerZoneId = devInfo[@"bannerZoneId"];
    self.interstitialZoneId = devInfo[@"interstitialZoneId"];
}

- (void) showAds: (NSMutableDictionary*) info position:(int) pos
{
    NSString* strType = [info objectForKey:@"adType"];
    AdcashAdType adType = [strType intValue];
    
    switch (adType) {
        case AdcashAdTypeBanner:
        {
            NSString* strSize = [info objectForKey:@"adSize"];
            ADCAdSize adSize = [strSize intValue];
            [self showBanner:adSize atPos:pos];
            break;
        }
        case AdcashAdTypeInterstitial:
            [self showInterstitial];
            break;
        default:
            DLog(@"The value of 'adType' is wrong (should be 1(Banner) or 2(Interstitial))");
            break;
    }
}

- (void)hideAds:(NSMutableDictionary *)info
{
    NSString* strType = [info objectForKey:@"adType"];
    AdcashAdType type = [strType intValue];
    NSAssert(type == AdcashAdTypeBanner || type == AdcashAdTypeInterstitial,
             @"<Adcash> The value of 'adType' is wrong (should be Banner(1) or Interstitial(2)");
    switch (type) {
        case AdcashAdTypeBanner:
            if (nil != self.bannerView) {
                [self.bannerView removeFromSuperview];
                self.bannerView = nil;
            }
            break;
        case AdcashAdTypeInterstitial:
            NSAssert(false, @"<Adcash> You can not hide interstitial programatically.");
            DLog(@"<Adcash> You can not hide interstitial programatically.");
            break;
        default:
            DLog(@"<Adcash> The value of 'AdmobType' is wrong (should be Banner(1) or Interstitial(2)");
            break;
    }
}

- (void)queryPoints
{
    DLog(@"Query points");
}

- (void) spendPoints: (int) points
{
    DLog(@"Spend points");
}

- (void)setDebugMode:(BOOL)debug
{
    DLog(@"setDebugMode:");
}

- (NSString *)getSDKVersion
{
    return @"2.3.0";
}

- (NSString *)getPluginVersion
{
    return @"1.0.0";
}

#pragma mark - BannerView

- (void) showBanner: (ADCAdSize) sizeEnum atPos:(int) pos
{
    ADCAdSize size = ADCAdSizeSmartBanner;
    switch (sizeEnum) {
        case ADCAdSizeSmartBanner:
            size = ADCAdSizeSmartBanner;
            break;
        // Add more when more sizes available
        default:
            break;
    }
    if (nil != self.bannerView) {
        [self.bannerView removeFromSuperview];
        self.bannerView = nil;
    }
    
    UIViewController *rootViewController = [AdsWrapper getCurrentRootViewController];
    self.bannerView = [[ADCBannerView alloc] initWithAdSize:size
                                                  zoneID:self.bannerZoneId
                                        rootViewController:rootViewController];
    self.bannerView.delegate = self;

    UIViewAutoresizing mask = UIViewAutoresizingFlexibleLeftMargin |
    UIViewAutoresizingFlexibleRightMargin |
    UIViewAutoresizingFlexibleWidth;
    CGPoint origin = CGPointMake(0, 0);
    switch (pos) {
        case kPosTop:
            mask = mask | UIViewAutoresizingFlexibleBottomMargin;
            origin = CGPointMake(0, 0);
            break;
        case kPosBottom:
            mask = mask | UIViewAutoresizingFlexibleTopMargin;
            origin = CGPointMake(0, CGRectGetHeight(rootViewController.view.frame) - CGRectGetHeight(self.bannerView.frame));
            break;
        default:
            DLog(@"<Adcash> Other banner positions than Top(1) and Bottom(4) are not supported!");
            [AdsWrapper onAdsResult:self
                            withRet:kUnknownError
                            withMsg:@"<Adcash> Other banner positions than Top(1) and Bottom(4) are not supported!"];
            return;
    }
    
    self.bannerView.autoresizingMask = mask;
    
    CGRect aRect = self.bannerView.frame;
    aRect.origin = origin;
    self.bannerView.frame = aRect;
    
    [rootViewController.view addSubview:self.bannerView];
    
    [self.bannerView load];
}

- (void)bannerView:(ADCBannerView *)bannerView didFailToReceiveAdWithError:(NSError *)error
{
    NSString *msg = [NSString stringWithFormat:@"<Adcash> Banner failed to load. Reason: %@", [error localizedDescription]];
    [AdsWrapper onAdsResult:self withRet:kUnknownError withMsg:msg];
}

#pragma mark - Interstitial

- (void) showInterstitial
{
    if (self.interstitial) {
        DLog(@"<Adcash> You can not show another interstitial before closing the current one.");
        [AdsWrapper onAdsResult:self
                        withRet:kUnknownError
                        withMsg:@"<Adcash> You can not show another interstitial before closing the current one."];
        return;
    }
    
    if (!self.interstitialZoneId || [self.interstitialZoneId length] == 0) {
        DLog(@"<Adcash> Interstitial zone id has not been set in `configDeveloperInfo` method");
        [AdsWrapper onAdsResult:self
                        withRet:kUnknownError
                        withMsg:@"<Adcash> Interstitial zone id has not been set in `configDeveloperInfo` method"];
        return;
    }
    
    self.interstitial = [[ADCInterstitial alloc] initWithZoneID:self.interstitialZoneId];
    self.interstitial.delegate = self;
    [self.interstitial load];
}

- (void)interstitialDidReceiveAd:(ADCInterstitial *)interstitial
{
    [interstitial presentFromRootViewController:[AdsWrapper getCurrentRootViewController]];
    
    [AdsWrapper onAdsResult:self withRet:kAdsReceived withMsg:@"<Adcash> Interstitial loaded OK."];
}

- (void)interstitial:(ADCInterstitial *)interstitial didFailToReceiveAdWithError:(NSError *)error
{
    self.interstitial = nil;
    NSString *msg = [NSString stringWithFormat:@"<Adcash> Interstitial failed to load. Reason: %@", [error localizedDescription]];
    [AdsWrapper onAdsResult:self withRet:kUnknownError withMsg:msg];
}

- (void)interstitialWillDismissScreen:(ADCInterstitial *)interstitial
{
    self.interstitial = nil;
    [AdsWrapper onAdsResult:self withRet:kAdsDismissed withMsg:@"<Adcash> Interstitial has been dismissed"];
}

@end
