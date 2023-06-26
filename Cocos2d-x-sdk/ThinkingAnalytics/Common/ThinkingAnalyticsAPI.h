#include <string>
#include <iostream>
#include <sstream>
#include "TDJSONObject.h"
const string LIB_NAME = "Cocos2d-x";
const string LIB_VERSION = "1.3.4.1";

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
    
    typedef TDJSONObject(*GetDynamicSuperProperties)();
    typedef TDJSONObject(*GetAutoTrackCallBack)(int eventType, TDJSONObject properties);
    static std::map<string,GetDynamicSuperProperties> dynamicPropertiesMap;
    static std::map<string,GetAutoTrackCallBack>      autoTrackCallBackMap;
   
    
    class ThinkingAnalyticsAPI {
    public:
        
        static void init(string appid,string server);
        
        static void init(Config config);
        
        static void track(string eventName,string appid = "");
        
        static void track(string eventName, const TDJSONObject &properties,string appid = "");
        
        static void track(TDFirstEvent* firstEvent,string appid = "");
        
        static void track(TDUpdatableEvent* updatableEvent,string appid = "");
        
        static void track(TDOverWritableEvent* overWritableEvent,string appid = "");
        
        static void timeEvent(string eventName,string appid = "");
        
        static void login(string accountID,string appid = "");
        
        static void logout(string appid = "");
        
        static void identify(string distinctId,string appid = "");
        
        static string getDistinctId(string appid = "");
        
    
        static  void user_set(const TDJSONObject &properties,string appid = "");
        
        static  void user_setOnce(const TDJSONObject &properties,string appid = "");
        
        static  void user_add(const TDJSONObject &properties,string appid = "");
        
        static  void user_append(const TDJSONObject &properties,string appid = "");
        
        static  void user_uniqAppend(const TDJSONObject &properties,string appid = "");
        
        static  void user_delete(string appid = "");
        
        static  void user_unset(string propertyName,string appid = "");

  
        static  void setSuperProperties(const TDJSONObject &properties,string appid = "");
       
        static  TDJSONObject getSuperProperties(string appid = "");
        
        static void  clearSuperProperties(string appid = "");
      
        static void  unsetSuperProperty(string superPropertyName,string appid = "");
        
        static void  setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties,string appid = "");
        
        static PresetProperties* getPresetProperties(string appid = "");
        
        static  void enableAutoTrack(string appid = "");
        
        static  void enableAutoTrack(const TDJSONObject &properties, TAAutoTrackType eventType = (TAAutoTrackType)15, string appid = "");
       
        static  void flush(string appid = "");

        static  string createLightInstance(string appid = "");
        
        static  void enableTracking(bool enabled,string appid = "");
        
        static  void optOutTrackingAndDeleteUser(string appid = "");
        
        static  void optOutTracking(string appid = "");
       
        static  void optInTracking(string appid = "");

        static  void setTrackStatus(TATrackType status,string appid = "");
      
        static  void enableTrackLog(bool enableLog);
       
        static  string getDeviceId();
        
        static void calibrateTime(long long timestamp);

        static void calibrateTimeWithNtp(string ntpServer);
        
        static string currentAppId(string appId);
        
        static string getLocalRegion();
        
        static void enableThirdPartySharing(TAThirdPartyType type, string appId = "");
        
        static void enableThirdPartySharing(TAThirdPartyType type, const TDJSONObject &properties, string appId = "");

    };

}
