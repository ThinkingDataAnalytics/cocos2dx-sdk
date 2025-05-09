#include <string>
#include <iostream>
#include <sstream>
#include "TDJSONObject.h"

/**
 * \mainpage
 * \section main_introduction Introduction
 * # ThinkingData SDK for Cocos2d-x
 * ![output](http://image.thinkingdata.cn/images/common/logo-en.png)
 * 
 * This is the [ThinkingData](https://www.thinkingdata.cn)™ SDK for Cocos2d-x. Documentation is available on our help center in the following languages:
 * 
 * - [English](https://docs.thinkingdata.cn/ta-manual/latest/en/99oQ5UeGzK09DWfPCaQwCg/installation/client_sdk/game_engine_installation/cocos2d-x_sdk_installation/cocos2d-x_sdk_installation.html)
 * - [中文](https://docs.thinkingdata.cn/ta-manual/latest/installation/installation_menu/client_sdk/game_engine_sdk_installation/cocos2d-x_sdk_installation/cocos2d-x_sdk_installation.html)
 * - [日本語](https://docs.thinkingdata.io/ta-manual/v4.0/ja/installation/installation_menu/client_sdk/game_engine_sdk_installation/cocos2d-x_sdk_installation/cocos2d-x_sdk_installation.html)
 * 
 */
const string LIB_NAME = "Cocos2d-x";
const string LIB_VERSION = "2.0.2";

using namespace std;
template < typename TDType >
static void addValue(vector<TDType> &list,TDType &value)
{
  typename vector<TDType>::iterator ret = std::find(list.begin(),list.end(), value);
  if(ret == list.end())
  {
      list.push_back(value);
  }
}

namespace thinkingdata {
    namespace analytics {
    
    typedef TDJSONObject(*GetDynamicSuperProperties)();
    typedef TDJSONObject(*GetAutoTrackEventProperties)(int eventType, TDJSONObject properties);
    static std::map<string,GetDynamicSuperProperties>   dynamicPropertiesMap;
    static std::map<string,GetAutoTrackEventProperties> autoTrackCallBackMap;
    
    
    /** 
     * @brief ThinkingData Analytics SDK for Cocos2d-x
     * 
     */
    class TDAnalytics {
    public:
        
        /**
         * @brief Init SDK
         * 
         * @param appid Project ID
         * @param server Server URL
         */
        static void init(string appid,string server);
        
        /**
         * @brief Init SDK
         * 
         * @param config SDK Config, include Project ID、Server URL... see <TDConfig>
         */
        static void init(TDConfig config);
        
        /**
         * @brief Track a Event
         * 
         * @param eventName Event Name
         * @param appid Project ID, optional
         */
        static void track(string eventName,string appid = "");
        
        /**
         * @brief Track a Event with Custom Properties
         * 
         * @param eventName Event Name
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static void track(string eventName, const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Track a First Event
         * 
         * @param firstEvent First Event
         * @param appid Project ID, optional
         */
        static void track(TDFirstEventModel* firstEvent,string appid = "");

        /**
         * @brief Track a Updatable Event
         * 
         * @param updatableEvent Updatable Event
         * @param appid Project ID, optional
         */
        static void track(TDUpdatableEventModel* updatableEvent,string appid = "");
        
        /**
         * @brief Track a Overwritable Event
         * 
         * @param overWritableEvent Overwritable Event
         * @param appid Project ID, optional
         */
        static void track(TDOverwritableEventModel* overWritableEvent,string appid = "");
        
        /**
         * @brief Start a Event Timer
         * 
         * @param eventName Event Name
         * @param appid Project ID, optional
         */
        static void timeEvent(string eventName,string appid = "");
        
        /**
         * @brief Login Account ID
         * 
         * @param accountID Account ID
         * @param appid Project ID, optional
         */
        static void login(string accountID,string appid = "");
        
        /**
         * @brief Logout Account ID
         * 
         * @param appid Project ID, optional
         */
        static void logout(string appid = "");
        
        /**
         * @brief Set the Distinct Id object
         * 
         * @param distinctId 
         * @param appid 
         */
        static void setDistinctId(string distinctId,string appid = "");
        
        /**
         * @brief Get the Distinct ID object
         * 
         * @param appid Project ID, optional
         * @return string Distinct ID
         */
        static string getDistinctId(string appid = "");
        
        /**
         * @brief Track a User Properties
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static  void userSet(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Track a User Properties, only the first data will be recorded
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static  void userSetOnce(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Track a User Properties, number data will bo added
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */        
        static  void userAdd(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Track a User Properties, list data will be appended
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static  void userAppend(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Track a User Properties, list data will be appended uniquely
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static  void userUniqAppend(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Delete a User Properties
         * 
         * @param appid Project ID, optional
         */
        static  void userDelete(string appid = "");
        
        /**
         * @brief Unset a User Properties
         * 
         * @param properties Custom Properties
         * @param appid Project ID, optional
         */
        static  void userUnset(string propertyName,string appid = "");
        
        /**
         * @brief Set the Super Properties object
         * 
         * @param properties Super Properties
         * @param appid Project ID, optional
         */
        static  void setSuperProperties(const TDJSONObject &properties,string appid = "");
        
        /**
         * @brief Get the Super Properties object
         * 
         * @param appid Project ID, optional
         * @return TDJSONObject Super Properties
         */
        static  TDJSONObject getSuperProperties(string appid = "");
        
        /**
         * @brief Clear the Super Properties
         * 
         * @param appid Project ID, optional
         */
        static void  clearSuperProperties(string appid = "");
        
        /**
         * @brief Unset the Super Properties
         * 
         * @param superPropertyName Super Property Name
         * @param appid Project ID, optional
         */
        static void  unsetSuperProperty(string superPropertyName,string appid = "");
        
        /**
         * @brief Set the Dynamic Super Properties object
         * 
         * @param getDynamicSuperProperties Dynamic Super Properties
         * @param appid Project ID, optional
         */
        static void  setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties,string appid = "");
        
        /**
         * @brief Get the Preset Properties object
         * 
         * @param appid Project ID, optional
         * @return TDPresetProperties* Preset Properties
         */
        static TDPresetProperties* getPresetProperties(string appid = "");
        
        /**
         * @brief Enable Auto Track Event
         * 
         * @param appid Project ID, optional
         */
        static  void enableAutoTrack(string appid = "");
        
        /**
         * @brief Enable Auto Track Event
         * 
         * @param properties Custom Event Propertiers
         * @param eventType Auto-tracking Event Type, see <TDAutoTrackEventType>
         * @param appid Project ID, optional
         */
        static  void enableAutoTrack(const TDJSONObject &properties, TDAutoTrackEventType eventType = (TDAutoTrackEventType)15, string appid = "");
        
        /**
         * @brief Flush Event Data
         * 
         * @param appid Project ID, optional
         */
        static  void flush(string appid = "");
        
        /**
         * @brief Create a Light Instance object
         * 
         * @param appid Project ID, optional
         * @return string Light Instance ID
         */
        static  string createLightInstance(string appid = "");
        
        [[deprecated("Replaced by setTrackStatus")]]
        static  void enableTracking(bool enabled,string appid = "");
        
        [[deprecated("Replaced by setTrackStatus")]]
        static  void optOutTrackingAndDeleteUser(string appid = "");
        
        [[deprecated("Replaced by setTrackStatus")]]
        static  void optOutTracking(string appid = "");
        
        [[deprecated("Replaced by setTrackStatus")]]
        static  void optInTracking(string appid = "");
        
        /**
         * @brief Set the Track Status object
         * 
         * @param status Track Status, see <TDTrackStatus>
         * @param appid Project ID, optional
         */
        static  void setTrackStatus(TDTrackStatus status,string appid = "");
        
        /**
         * @brief Enable Print SDK Logs
         * 
         * @param enableLog Enable Printing
         */
        static  void enableTrackLog(bool enableLog);
        
        /**
         * @brief Get the Device Id object
         * 
         * @return string Device ID
         */
        static  string getDeviceId();
        
        /**
         * @brief Calibrate Event Time with Timestamp
         * 
         * @param timestamp Timestamp
         */
        static void calibrateTime(long long timestamp);
        
        /**
         * @brief Calibrate Event Time with NTP Server
         * 
         * @param ntpServer NTP Server
         */
        static void calibrateTimeWithNtp(string ntpServer);
        
        static string currentAppId(string appId);
        
        /**
         * @brief Get the Local Region object
         * 
         * @return string Local Region Code
         */
        static string getLocalRegion();
        
        /**
         * @brief Enable Third Party Sharing
         * 
         * @param type Third party Type, see <TDThirdPartyType>
         * @param appId Project ID, optional
         */
        static void enableThirdPartySharing(TDThirdPartyType type, string appId = "");
        
        /**
         * @brief Enable Third Party Sharing
         * 
         * @param type Third party Type, see <TDThirdPartyType>
         * @param properties Custom Properties
         * @param appId Project ID, optional
         */
        static void enableThirdPartySharing(TDThirdPartyType type, const TDJSONObject &properties, string appId = "");
        
    };
    }
}
