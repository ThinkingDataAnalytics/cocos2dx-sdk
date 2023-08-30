//
//  ThinkingSDKObject.h
//  ThinkingEngine
//
//  Created by wwango on 2021/12/20.
//

//#ifndef ThinkingSDKObject_h
//#define ThinkingSDKObject_h

#include "../Common/TDAnalytics.h"
#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"
#include <sys/timeb.h>
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include <IOKit/IOKitLib.h>
#endif

USING_NS_CC;

using namespace thinkingdata::analytics;

namespace thinkingdata {
    namespace analytics {
    
    class ThinkingSDKObject
    {
    public:
        
        TDJSONObject m_superProperties;
        
        ThinkingSDKObject();
        ThinkingSDKObject(string appid, string server);
        ThinkingSDKObject(string appid, string server, string name);
        ~ThinkingSDKObject();
        
        bool m_optOutTracking;
        bool m_enable;
        double zoneOffset;
        
        string getName();
        string getAppId();
        string getServer();
        string getDistinctId();
        string getAccountId();
        string getDeviceId();
        
        void setName(string name);
        void setAppId(string appId);
        void setServer(string server);
        void setDistinctId(string distinctId);
        void setAccountId(string accountId);
        void clearAccountId();
        
        void enableTracking(bool enabled);
        void optOutTrackingAndDeleteUser();
        void optOutTracking();
        void optInTracking();
        
        void track(string eventName);
        void track(string eventName,const TDJSONObject& node);
        void trackFirst(TDFirstEventModel *event);
        void trackUpdate(TDUpdatableEventModel *event);
        void trackOverWrite(TDOverwritableEventModel *event);
        void login(string accountID);
        void logout();
        
        void user_set(const TDJSONObject &properties);
        void user_setOnce(const TDJSONObject &properties);
        void user_append(const TDJSONObject &properties);
        void user_add(const TDJSONObject &properties);
        void user_delete();
        void user_unset(string propertyName);
        void user_uniqAppend(const TDJSONObject &properties);
        void clearSuperProperties();
        void unsetSuperProperty(string superPropertyName);
        void setSuperProperties(const TDJSONObject &properties);
        void setDynamicSuperProperties(GetDynamicSuperProperties dynamicSuperProperties);
        
        void flush(string eventName);
        void flush(string eventName, const TDJSONObject &properties);
        void flush(const TDEventModel &event);
        void flushUser(string eventType, const TDJSONObject &properties);
        void flush(string eventName, string eventType, const TDJSONObject &properties, string eventID, string firstCheckID);
        void complete(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response);
        
        void saveNode(const TDJSONObject &node);
        TDJSONObject getNode();
        
        string getAccountIdKey();
        string getDistinctIdKey();
        string getSuperPropertyKey();
        string getDeviceIdKey();
        string getDisEnableKey();
        string getoptOutKey();
        
    private:
        string m_appid;
        string m_server;
        string m_name;
        string m_distinctId;
        string m_accountId;
        string m_deviceId;
    };
    }
}

//#endif /* ThinkingSDKObject_h */

