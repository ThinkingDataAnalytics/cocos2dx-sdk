#include <string>
#include <iostream>
#include <sstream>
#include "TDJSONObject.h"
const string LIB_NAME = "Cocos2d-x";
const string LIB_VERSION = "2.0.0-beta.1";

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
    
    
    class TDAnalytics {
    public:
        
        static void init(string appid,string server);
        
        static void init(TDConfig config);
        
        static void track(string eventName,string appid = "");
        
        static void track(string eventName, const TDJSONObject &properties,string appid = "");
        
        static void track(TDFirstEventModel* firstEvent,string appid = "");
        
        static void track(TDUpdatableEventModel* updatableEvent,string appid = "");
        
        static void track(TDOverwritableEventModel* overWritableEvent,string appid = "");
        
        static void timeEvent(string eventName,string appid = "");
        
        static void login(string accountID,string appid = "");
        
        static void logout(string appid = "");
        
        static void setDistinctId(string distinctId,string appid = "");
        
        static string getDistinctId(string appid = "");
        
        
        static  void userSet(const TDJSONObject &properties,string appid = "");
        
        static  void userSetOnce(const TDJSONObject &properties,string appid = "");
        
        static  void userAdd(const TDJSONObject &properties,string appid = "");
        
        static  void userAppend(const TDJSONObject &properties,string appid = "");
        
        static  void userUniqAppend(const TDJSONObject &properties,string appid = "");
        
        static  void userDelete(string appid = "");
        
        static  void userUnset(string propertyName,string appid = "");
        
        
        static  void setSuperProperties(const TDJSONObject &properties,string appid = "");
        
        static  TDJSONObject getSuperProperties(string appid = "");
        
        static void  clearSuperProperties(string appid = "");
        
        static void  unsetSuperProperty(string superPropertyName,string appid = "");
        
        static void  setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties,string appid = "");
        
        static TDPresetProperties* getPresetProperties(string appid = "");
        
        static  void enableAutoTrack(string appid = "");
        
        static  void enableAutoTrack(const TDJSONObject &properties, TDAutoTrackEventType eventType = (TDAutoTrackEventType)15, string appid = "");
        
        static  void flush(string appid = "");
        
        static  string createLightInstance(string appid = "");
        
        static  void enableTracking(bool enabled,string appid = "");
        
        static  void optOutTrackingAndDeleteUser(string appid = "");
        
        static  void optOutTracking(string appid = "");
        
        static  void optInTracking(string appid = "");
        
        static  void setTrackStatus(TDTrackStatus status,string appid = "");
        
        static  void enableTrackLog(bool enableLog);
        
        static  string getDeviceId();
        
        static void calibrateTime(long long timestamp);
        
        static void calibrateTimeWithNtp(string ntpServer);
        
        static string currentAppId(string appId);
        
        static string getLocalRegion();
        
        static void enableThirdPartySharing(TDThirdPartyType type, string appId = "");
        
        static void enableThirdPartySharing(TDThirdPartyType type, const TDJSONObject &properties, string appId = "");
        
    };
    }
}
