/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.plugin;

import java.util.Hashtable;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.WindowManager;

import com.adcash.mobileads.AdcashAdRequestFailedError;
import com.adcash.mobileads.AdcashBannerView;
import com.adcash.mobileads.AdcashInterstitial;
import com.adcash.mobileads.AdcashView;
import com.adcash.mobileads.AdcashConversionTracker;

public class AdcashSDK implements InterfaceAds {

	private static final String LOG_TAG = "AdcashSDK";

	private static final String KEY_TYPE = "adType";
	private static final String KEY_BANNER_ZONE_ID = "bannerZoneId";
	private static final String KEY_INTERSTITIAL_ZONE_ID = "interstitialZoneId";

	private static Activity mContext = null;
	private static boolean bDebug = false;
	private static AdcashSDK mAdapter = null;

	private AdcashBannerView adView = null;
	private AdcashInterstitial interstitial = null;
	private AdcashConversionTracker tracker = null;

	private WindowManager mWm = null;

	private static final int AD_TYPE_BANNER = 1;
	private static final int AD_TYPE_FULLSCREEN = 2; // Interstitial
	private static final int AD_TYPE_MOREAPP = 3; // Conversion tracker

	private String bannerZoneId = "";
	private String interstitialZoneId = "";

	public AdcashSDK(Context context) {
		mContext = (Activity) context;
		mAdapter = this;
	}

	@Override
	public void showAds(Hashtable<String, String> info, int pos) {
		try {
			String strType = info.get(KEY_TYPE);
			int adsType = Integer.parseInt(strType);

			switch (adsType) {
			case AD_TYPE_BANNER:
				showBannerAd(bannerZoneId, pos);
				break;
			case AD_TYPE_FULLSCREEN: // if interstitial
				loadInterstitialAd(interstitialZoneId);
				break;
			case AD_TYPE_MOREAPP: // if conversion tracker

				break;
			default:
				LogD("The value of 'AdmobType' is wrong (should be Banner(1) or Interstitial(2)");
				break;
			}
		} catch (Exception e) {
			LogE("Error when show Ads ( " + info.toString() + " )", e);
		}
	}

	@Override
	public void hideAds(Hashtable<String, String> info) {
		try {
			String strType = info.get(KEY_TYPE);
			int adsType = Integer.parseInt(strType);

			switch (adsType) {
			case AD_TYPE_BANNER:
				hideBannerAd();
				break;
			case AD_TYPE_FULLSCREEN:
				hideInterstitialAd();
				break;
			default:
				LogD("The value of 'AdmobType' is wrong (should be Banner(1) or Interstitial(2)");
				break;
			}
		} catch (Exception e) {
			LogE("Error when hide Ads ( " + info.toString() + " )", e);
		}
	}

	private void showBannerAd(final String adUnitId, int pos) {// position = 1
																// => TOP;
																// position = 4
																// => BOTTOM
		final int curPos = pos;

		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				// destory the ad view before
				if (null != adView) {
					if (null != mWm) {
						mWm.removeView(adView);
					}
					adView = null;
				}

				adView = new AdcashBannerView(mContext, null);
				adView.setBackgroundColor(Color.TRANSPARENT);
				adView.setAdUnitId(adUnitId);
				// adView.setAdSize(adSize);

				if (null == mWm) {
					mWm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
				}
				AdsWrapper.addAdView(mWm, adView, curPos);

				adView.setAdListener(new AdcashAdsListener());
				adView.loadAd();
			}
		});
	}

	private void hideBannerAd() {
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (null != adView) {
					if (null != mWm) {
						mWm.removeView(adView);
					}

					adView = null;
				}
			}
		});
	}

	// Interstitials
	private void loadInterstitialAd(final String adUnitId) {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				interstitial = new AdcashInterstitial(adUnitId, mContext);
				AdcashAdsListener listener = new AdcashAdsListener();
				listener.setUpdater(new UpdateListener() {
					
					@Override
					public void updateOnAdLoaded() {
						interstitial.showAd();
					}
				});
				interstitial.setAdListener(listener);
				interstitial.loadAd();
			}
		});
	}

	private void hideInterstitialAd() {
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				LogD("Cannot hide interstitial programatically");
			}
		});
	}

	public void showInterstitialAd() {
		if (interstitial.isReady) {
			interstitial.showAd();
		}
	}

	interface UpdateListener {

		public void updateOnAdLoaded();
	}

	// Listener
	private class AdcashAdsListener extends AdcashView.AdListener {
		private UpdateListener mUpdater;

		public void setUpdater(UpdateListener listener) {
			mUpdater = listener;
		}

		@Override
		public void onAdClosed() {
			super.onAdClosed();
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsDismissed, "Ads view dismissed!");
		}

		@Override
		public void onAdFailedToLoad(int errorCode) {
			super.onAdFailedToLoad(errorCode);
			int errorNo = AdsWrapper.RESULT_CODE_UnknownError;
			String message = "Unknow error";
			switch (errorCode) {
			case AdcashAdRequestFailedError.NO_NETWORK:
				errorNo = AdsWrapper.RESULT_CODE_NetworkError;
				message = "No internet connection";
				break;
			case AdcashAdRequestFailedError.REQUEST_FAILED:
				errorNo = AdsWrapper.RESULT_CODE_NetworkError;
				message = "Request failed";
				break;
			case AdcashAdRequestFailedError.NETWORK_FAILURE:
				errorNo = AdsWrapper.RESULT_CODE_NetworkError;
				message = "Network failure";
				break;
			case AdcashAdRequestFailedError.NO_AD:
				message = "There is no ad";
				break;
			default:
				message = "Unknow error";
			}

			AdsWrapper.onAdsResult(mAdapter, errorNo, message);
		}

		@Override
		public void onAdLeftApplication() {
			super.onAdLeftApplication();
			LogD("onLeaveApplication invoked");
		}

		@Override
		public void onAdLoaded() {
			super.onAdLoaded();
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsReceived, "Ads request received success!");

			if (mUpdater != null) {
				mUpdater.updateOnAdLoaded();
			}
		}

		@Override
		public void onAdOpened() {
			super.onAdOpened();
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsShown, "Ads view shown!");
		}

	}

	// ConversionTracker

	public void reportAppOpen() {

		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (tracker == null) {
					tracker = new AdcashConversionTracker(mContext);
				}
				tracker.reportAppOpen();
			}
		});
	}

	public void reportConversion(final String campaign, final String payout, final String tid) {

		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (tracker == null) {
					tracker = new AdcashConversionTracker(mContext);
				}
				tracker.reportConversion(campaign, payout, tid);
			}
		});

	}
	// Other interface functions

	@Override
	public void setDebugMode(boolean debug) {
		bDebug = debug;
	}

	@Override
	public String getSDKVersion() {
		return "1.2.1";
	}

	@Override
	public String getPluginVersion() {
		return "1.0.0";
	}

	@Override
	public void spendPoints(int points) {
		LogD("Adcash not support spend points!");
	}

	@Override
	public void queryPoints() {
		LogD("Adcash not support query points!");
	}

	protected static void LogE(String msg, Exception e) {
		Log.e(LOG_TAG, msg, e);
		e.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (bDebug) {
			Log.d(LOG_TAG, msg);
		}
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		try {
			if (devInfo.containsKey(KEY_INTERSTITIAL_ZONE_ID)) {
				interstitialZoneId = devInfo.get(KEY_INTERSTITIAL_ZONE_ID);
			}
			if (devInfo.containsKey(KEY_BANNER_ZONE_ID)) {
				bannerZoneId = devInfo.get(KEY_BANNER_ZONE_ID);
			}
		} catch (Exception e) {
			LogE("initAppInfo, The format of appInfo is wrong", e);
		}
	}
}
