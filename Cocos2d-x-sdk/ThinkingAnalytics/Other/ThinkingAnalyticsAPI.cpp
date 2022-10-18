
#include "ThinkingSDKObject.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "json/rapidjson.h"
#include "json/document.h"

using namespace thinkingdata;
using namespace cocos2d::network;
using namespace rapidjson;

const static string TD_EVENT_TYPE                 = "track";
const static string TD_EVENT_TYPE_TRACK_FIRST     = "track_first";
const static string TD_EVENT_TYPE_TRACK_UPDATE    = "track_update";
const static string TD_EVENT_TYPE_TRACK_OVERWRITE = "track_overwrite";

const static string TD_EVENT_TYPE_USER_DEL        = "user_del";
const static string TD_EVENT_TYPE_USER_ADD        = "user_add";
const static string TD_EVENT_TYPE_USER_SET        = "user_set";
const static string TD_EVENT_TYPE_USER_SETONCE    = "user_setOnce";
const static string TD_EVENT_TYPE_USER_UNSET      = "user_unset";
const static string TD_EVENT_TYPE_USER_APPEND     = "user_append";

typedef std::map<string , ThinkingSDKObject*> MAP_THINKING_OBJECT;
typedef std::map<string , GetDynamicSuperProperties> MAP_THINKING_DYNAMICSUPERPROPERTIES;
typedef std::map<string , TDJSONObject> MAP_THINKING_SUPERPROPERTIES;
typedef std::map<string, thinkingdata::TDJSONObject::ValueNode> TD_MAP_TYPE;
typedef std::map<string, long long> TD_MAP_TIME_EVENT;
typedef std::map<string, TD_MAP_TIME_EVENT> TD_APPID_MAP_TIME_EVENT;

MAP_THINKING_DYNAMICSUPERPROPERTIES mapDyldProperties;
MAP_THINKING_SUPERPROPERTIES mapSuperProperties;
MAP_THINKING_OBJECT mapInstances;
TD_APPID_MAP_TIME_EVENT maptimeEvents;
string defaultInstanceAppid;

string td_getUUID() {
    char str_uuid[80];
    static bool hassrand;
    if (hassrand != true) {
        srand(time(NULL));
        hassrand = true;
    }
    snprintf(str_uuid, sizeof(str_uuid),
             "%x%x-%x-%x-%x-%x%x%x",
             rand(),
             rand(),
             rand(),
             ((rand() & 0x0fff) | 0x4000),
             rand() % 0x3fff + 0x8000,
             rand(),
             rand(),
             rand());
    return string(str_uuid);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

char td_uuid_buf[512] = "";
char *td_getMacUUID()
{
    if (strcmp(td_uuid_buf, "") == 0) {
        int bufSize = sizeof(td_uuid_buf);
        io_registry_entry_t ioRegistryRoot = IORegistryEntryFromPath(kIOMasterPortDefault, "IOService:/");
        CFStringRef uuidCf = (CFStringRef) IORegistryEntryCreateCFProperty(ioRegistryRoot, CFSTR(kIOPlatformUUIDKey), kCFAllocatorDefault, 0);
        IOObjectRelease(ioRegistryRoot);
        CFStringGetCString(uuidCf, td_uuid_buf, bufSize, kCFStringEncodingMacRoman);
        CFRelease(uuidCf);
    }
    return td_uuid_buf;
}
#endif


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
string td_getWindowsUUID()
{
    char szuuid[64] = { 0 };
    GUID guid;
    CoCreateGuid(&guid);
    _snprintf_s(
        szuuid,
        sizeof(szuuid),
        "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);

    return std::string(szuuid);
}
#endif

bool showLog;

void ThinkingAnalyticsAPI::track(string eventName,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->track(eventName, TDJSONObject());
}
void ThinkingAnalyticsAPI::track(string eventName,const TDJSONObject& node,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->track(eventName, node);
}
void ThinkingAnalyticsAPI::track(TDFirstEvent* firstEvent,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->trackFirst(firstEvent);
}
void ThinkingAnalyticsAPI::track(TDUpdatableEvent* updatableEvent,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->trackUpdate(updatableEvent);
}
void ThinkingAnalyticsAPI::track(TDOverWritableEvent* overWritableEvent,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->trackOverWrite(overWritableEvent);
}
void ThinkingAnalyticsAPI::timeEvent(string eventName,string appId)
{
    timeb t;
    ftime(&t);
    maptimeEvents[appId][eventName] = t.time*1000 + t.millitm;
}
void ThinkingAnalyticsAPI::login(string accountID,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->login(accountID);
}
void ThinkingAnalyticsAPI::logout(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->logout();
}
void ThinkingAnalyticsAPI::identify(string distinctId,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->setDistinctId(distinctId);
}
void ThinkingAnalyticsAPI::user_set(const TDJSONObject &properties,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_set(properties);
}
void ThinkingAnalyticsAPI::user_setOnce(const TDJSONObject &properties,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_setOnce(properties);
}
void ThinkingAnalyticsAPI::user_append(const TDJSONObject &properties,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_append(properties);
}
void ThinkingAnalyticsAPI::user_add(const TDJSONObject &properties,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_add(properties);
}
void ThinkingAnalyticsAPI::user_delete(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_delete();
}
void ThinkingAnalyticsAPI::user_unset(string propertyName,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->user_unset(propertyName);
}
void ThinkingAnalyticsAPI::enableAutoTrack(string appId)
{
    printf("ThinkingAnalytics SDK does not support this platform \n");
}
void ThinkingAnalyticsAPI::flush(string appId)
{
    printf("ThinkingAnalytics SDK does not support this platform \n");
}
string ThinkingAnalyticsAPI::getDeviceId()
{
    return mapInstances[defaultInstanceAppid]->getDeviceId();
}
void ThinkingAnalyticsAPI::setSuperProperties(const TDJSONObject &properties,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    
    TDJSONObject node = obj->getNode();
    node.mergeFrom(properties);
    
    obj->setSuperProperties(node);
}
void ThinkingAnalyticsAPI::clearSuperProperties(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->clearSuperProperties();
}
void ThinkingAnalyticsAPI::unsetSuperProperty(string superPropertyName,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->unsetSuperProperty(superPropertyName);
}
void ThinkingAnalyticsAPI::optOutTrackingAndDeleteUser(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->optOutTrackingAndDeleteUser();
}
void ThinkingAnalyticsAPI::optOutTracking(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->optOutTracking();
}
void ThinkingAnalyticsAPI::optInTracking(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->optInTracking();
}
void ThinkingAnalyticsAPI::enableTracking(bool enabled,string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return;}
    obj->enableTracking(enabled);
}
void ThinkingAnalyticsAPI::enableTrackLog(bool enabled)
{
    showLog = enabled;
}
string ThinkingAnalyticsAPI::getDistinctId(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {return "";}
    return obj->getDistinctId();
}
PresetProperties* ThinkingAnalyticsAPI::getPresetProperties(string appId)
{
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    cocos2d::Application* app = cocos2d::Application::getInstance();
    cocos2d::Application::Platform platform = app->getTargetPlatform();
    string get_version = app->getVersion();
    string platformString = platform==cocos2d::Application::Platform::OS_MAC ? "Mac":"Windows";
    string system_language = app->getCurrentLanguageCode();
    
    PresetProperties *presetProperties = new PresetProperties();
    presetProperties->deviceId = getDeviceId();
    presetProperties->os = platformString;
    presetProperties->appVersion = get_version;
    presetProperties->systemLanguage = system_language;
    presetProperties->screenWidth = cocos2d::Director::getInstance()->getWinSize().width;
    presetProperties->screenHeight = cocos2d::Director::getInstance()->getWinSize().height;
    presetProperties->zoneOffset = obj->zoneOffset;
    presetProperties->libName = LIB_NAME;
    presetProperties->libVersion = LIB_VERSION;
    
    return presetProperties;
}
void ThinkingAnalyticsAPI::calibrateTime(long long timestamp) {
    printf("ThinkingAnalytics SDK does not support this platform \n");
}
void ThinkingAnalyticsAPI::calibrateTimeWithNtp(string ntpServer) {
    printf("ThinkingAnalytics SDK does not support this platform \n");
}
string ThinkingAnalyticsAPI::getLocalRegion() {
    printf("ThinkingAnalytics SDK does not support this platform \n");
    return "";
}

TDJSONObject ThinkingAnalyticsAPI::getSuperProperties(string appId) {
    ThinkingSDKObject *obj = mapInstances[currentAppId(appId)];
    if (obj == NULL) {
        return TDJSONObject();
    }
    return obj->m_superProperties;
}
string ThinkingAnalyticsAPI::currentAppId(string appId) {
    if (appId == "") {
        return defaultInstanceAppid;
    }
    return appId;
}

void ThinkingAnalyticsAPI::setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties, string appId) {
    if (currentAppId(appId) == "") {return;}
    mapDyldProperties[currentAppId(appId)] = getDynamicSuperProperties;
}
string ThinkingAnalyticsAPI::createLightInstance(string appId) {
    printf("ThinkingAnalytics SDK does not support this platform \n");
    return "";
}
void ThinkingAnalyticsAPI::init(Config config) {
    string appid = config.getAppId();
    string server = config.getServer();
    ThinkingSDKObject *obj = mapInstances[appid];
    
    if (appid.size() != 0 && obj == NULL) {

        ThinkingSDKObject* obj = new ThinkingSDKObject(config.getAppId(), config.getServer());
        obj->setAppId(config.getAppId());
        obj->setServer(server);

        string deviceId = cocos2d::UserDefault::getInstance()->getStringForKey(obj->getDeviceIdKey().c_str());
        if (deviceId.size() == 0) {
            deviceId = obj->getDeviceId();
            if (deviceId.size() == 0) {
                deviceId = td_getUUID();
            }
        }

        cocos2d::UserDefault::getInstance()->setStringForKey(obj->getDeviceIdKey().c_str(), deviceId);
        cocos2d::UserDefault::getInstance()->flush();


        string distinctId = cocos2d::UserDefault::getInstance()->getStringForKey(obj->getDistinctIdKey().c_str());
        string defaultDistinctId;
        if (distinctId.size() > 0) {
            defaultDistinctId = distinctId;
        }
        else {
            defaultDistinctId = deviceId;
        }
        obj->setDistinctId(defaultDistinctId);


        string accountId = cocos2d::UserDefault::getInstance()->getStringForKey(obj->getAccountIdKey().c_str());
        obj->setAccountId(accountId);


        obj->m_superProperties = obj->getNode();
        
        string disEnableString = cocos2d::UserDefault::getInstance()->getStringForKey(obj->getDisEnableKey().c_str());
        string optoutString = cocos2d::UserDefault::getInstance()->getStringForKey(obj->getoptOutKey().c_str());
        obj->m_enable = (disEnableString == "1") ? false:true;
        obj->m_optOutTracking = (optoutString == "1") ? true:false;

        if (defaultInstanceAppid == "") {
            defaultInstanceAppid = appid;
        }
        
        
        time_t zero = 0;
        const tm* lt = localtime( &zero );
        int unaligned = lt->tm_sec + ( lt->tm_min + ( lt->tm_hour * 60 ) ) * 60;
        double timezone_offset = lt->tm_mon ? unaligned - 24*60*60 : unaligned;
        obj->zoneOffset = timezone_offset/3600;

        mapInstances[appid] = obj;

    }
}

void ThinkingAnalyticsAPI::init(string appId, string server) {
    Config config(appId, server);
    ThinkingAnalyticsAPI::init(config);
}

ThinkingSDKObject::ThinkingSDKObject()
{
}

ThinkingSDKObject::ThinkingSDKObject(string appid, string server)
{
    m_appid = appid;
    m_server = server;
    m_deviceId = getDeviceId();
}

ThinkingSDKObject::ThinkingSDKObject(string appid, string server, string name)
{
    m_appid = appid;
    m_server = server;
    m_name = name;
    m_deviceId = getDeviceId();
}

ThinkingSDKObject::~ThinkingSDKObject()
{
    printf("*** ThinkingSDKObject is being deleted  ****");
}

void ThinkingSDKObject::track(string eventName) {
    TDJSONObject node;
    flush(eventName);
}
void ThinkingSDKObject::track(string eventName,const TDJSONObject& node) {
    flush(eventName, node);
}
void ThinkingSDKObject::trackFirst(TDFirstEvent *event) {
    flush(*event);
}
void ThinkingSDKObject::trackUpdate(TDUpdatableEvent *event) {
    flush(*event);
}
void ThinkingSDKObject::trackOverWrite(TDOverWritableEvent *event) {
    flush(*event);
}

void ThinkingSDKObject::login(string accountID) {
    m_accountId = accountID;
    setAccountId(m_accountId);
}
void ThinkingSDKObject::logout() {
    m_accountId = "";
    setAccountId(m_accountId);
}

void ThinkingSDKObject::user_set(const thinkingdata::TDJSONObject &properties) {
    flushUser(TD_EVENT_TYPE_USER_SET, properties);
}
void ThinkingSDKObject::user_setOnce(const thinkingdata::TDJSONObject &properties) {
    flushUser(TD_EVENT_TYPE_USER_SETONCE, properties);
}
void ThinkingSDKObject::user_append(const thinkingdata::TDJSONObject &properties) {
    flushUser(TD_EVENT_TYPE_USER_APPEND, properties);
}
void ThinkingSDKObject::user_add(const thinkingdata::TDJSONObject &properties) {
    flushUser(TD_EVENT_TYPE_USER_ADD, properties);
}
void ThinkingSDKObject::user_delete() {
    TDJSONObject node;
    flushUser(TD_EVENT_TYPE_USER_DEL, node);
}
void ThinkingSDKObject::user_unset(string propertyName) {
    TDJSONObject obj;
    char *propertyNameString = (char *)propertyName.data();
    obj.setNumber(propertyNameString, 0);
    flushUser(TD_EVENT_TYPE_USER_UNSET, obj);
}


void ThinkingSDKObject::clearSuperProperties() {
   
    cocos2d::UserDefault::getInstance()->setStringForKey(getSuperPropertyKey().c_str(), "");
    cocos2d::UserDefault::getInstance()->flush();
    m_superProperties = TDJSONObject();
}
void ThinkingSDKObject::unsetSuperProperty(string superPropertyName) {
    
    if (superPropertyName.size()>0) {
        TDJSONObject node = getNode();
        node.removeKey(superPropertyName.c_str());
        setSuperProperties(node);
    }
}
void ThinkingSDKObject::setSuperProperties(const thinkingdata::TDJSONObject &properties)
{
    this->m_superProperties = properties;
    saveNode(properties);
}

void td_split(const std::string& src, const std::string& separator, std::vector<std::string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;
    dest.clear();
    index = str.find_first_of(separator,start);
    do
    {
        if (index != string::npos)
        {
            substring = str.substr(start,index-start );
            dest.push_back(substring);
            start =index+separator.size();
            index = str.find(separator,start);
            if (start == string::npos) break;
        }
    }while(index != string::npos);
 
    substring = str.substr(start);
    dest.push_back(substring);
}

TDJSONObject td_parseNode(const rapidjson::Value::MemberIterator &begin ,const rapidjson::Value::MemberIterator &end) {

    TDJSONObject json = TDJSONObject();
    rapidjson::Value jValue;
    string key = "";
    
    for (rapidjson::Value::MemberIterator it = begin; it != end; ++it) {
        
        key = it->name.GetString();
        jValue = it->value;

        if (jValue.IsString()) {
            string value = jValue.GetString();
            json.setString(key.c_str(), value.c_str());
        } else if (jValue.IsBool()) {
            json.setBool(key.c_str(), jValue.IsTrue() ? true:false);
        } else if (jValue.IsInt()) {
            json.setNumber(key.c_str(), jValue.GetInt());
        } else if (jValue.IsInt64()) {
            json.setNumber(key.c_str(), jValue.GetInt64());
        } else if (jValue.IsUint()) {
            json.setNumber(key.c_str(), (int32_t)jValue.GetUint());
        } else if (jValue.IsUint64()) {
            json.setNumber(key.c_str(), (int64_t)jValue.GetUint64());
        } else if (jValue.IsDouble()) {
            json.setNumber(key.c_str(), (int64_t)jValue.GetDouble());
        }  else if (jValue.IsArray()) {
            
            auto rsp_ = jValue.GetArray();
            vector<TDJSONObject> objs_obj;
            vector<string> objs_string;
            TDJSONObject objs_item;

            for (size_t iiii = 0; iiii < rsp_.Size(); iiii++) {

                rapidjson::Value& item = rsp_[iiii];
                if (item.IsString()) {
                    objs_string.push_back(item.GetString());
                } else if (item.IsObject()) {
                    objs_item = td_parseNode(item.GetObject().begin(), item.GetObject().end());
                    objs_obj.push_back(objs_item);
                }
            }

            if (objs_string.size()>0) {
                json.setList(key.c_str(), objs_string);
            } else if (objs_obj.size()>0) {
                json.setList(key.c_str(), objs_obj);
            }
        }
    }
    return json;
}

TDJSONObject ThinkingSDKObject::getNode()
{
    TDJSONObject jsonObjct = TDJSONObject();
    string str = cocos2d::UserDefault::getInstance()->getStringForKey(getSuperPropertyKey().c_str());
    
    rapidjson::Document document;
    document.Parse<0>(str.c_str());
    if (document.HasParseError()) {
        return jsonObjct;
    }
    
    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); itr++)
    {
        rapidjson::Value jKey;
        rapidjson::Value jValue;
        rapidjson::Document::AllocatorType allocator;
        jKey.CopyFrom(itr->name, allocator);
        jValue.CopyFrom(itr->value,allocator);
        if (jKey.IsString())
        {
            if (jValue.IsString()) {
                string value = jValue.GetString();
                jsonObjct.setString(jKey.GetString(), value.c_str());
            } else if (jValue.IsBool()) {
                jsonObjct.setBool(jKey.GetString(), jValue.IsTrue() ? true:false);
            } else if (jValue.IsInt()) {
                jsonObjct.setNumber(jKey.GetString(), jValue.GetInt());
            } else if (jValue.IsInt64()) {
                jsonObjct.setNumber(jKey.GetString(), jValue.GetInt64());
            } else if (jValue.IsUint()) {
                jsonObjct.setNumber(jKey.GetString(), (int32_t)jValue.GetUint());
            } else if (jValue.IsUint64()) {
                jsonObjct.setNumber(jKey.GetString(), (int64_t)jValue.GetUint64());
            } else if (jValue.IsDouble()) {
                jsonObjct.setNumber(jKey.GetString(), (int64_t)jValue.GetDouble());
            } else if (jValue.IsObject()) {
                jsonObjct.setJsonObject(jKey.GetString(), td_parseNode(jValue.GetObject().begin(), jValue.GetObject().end()));
                
            } else if (jValue.IsArray()) {
                
                auto rsp_ = jValue.GetArray();
                vector<TDJSONObject> objs_obj;
                vector<string> objs_string;
                TDJSONObject objs_item;

                for (size_t iiii = 0; iiii < rsp_.Size(); iiii++) {

                    rapidjson::Value& item = rsp_[iiii];
                    if (item.IsString()) {
                        objs_string.push_back(item.GetString());
                    } else if (item.IsObject()) {
                        objs_item = td_parseNode(item.GetObject().begin(), item.GetObject().end());
                        objs_obj.push_back(objs_item);
                    }
                }

                if (objs_string.size()>0) {
                    jsonObjct.setList(jKey.GetString(), objs_string);
                } else if (objs_obj.size()>0) {
                    jsonObjct.setList(jKey.GetString(), objs_obj);
                }
            }
        }
    }
    
    return jsonObjct;
}

void ThinkingSDKObject::saveNode(const TDJSONObject &node)
{
    string jsonString = TDJSONObject::toJson(node);
    cocos2d::UserDefault::getInstance()->setStringForKey(getSuperPropertyKey().c_str(), jsonString.c_str());
    cocos2d::UserDefault::getInstance()->flush();
}

void ThinkingSDKObject::setDynamicSuperProperties(GetDynamicSuperProperties dynamicSuperProperties)
{
}

void ThinkingSDKObject::flush(string eventName)
{
    TDJSONObject node;
    flush(eventName, TD_EVENT_TYPE, node, "", "");
}
void ThinkingSDKObject::flush(string eventName, const TDJSONObject &properties)
{
    flush(eventName, TD_EVENT_TYPE, properties, "", "");
}

void ThinkingSDKObject::flush(const ThinkingAnalyticsEvent &event)
{
    TDJSONObject node;
    if (event.mType == FIRST) {
        flush(event.mEventName, TD_EVENT_TYPE_TRACK_FIRST, event.mProperties, "", event.mExtraId);
    } else if (event.mType == UPDATABLE) {
        flush(event.mEventName, TD_EVENT_TYPE_TRACK_UPDATE, event.mProperties, event.mExtraId, "");
    } else if (event.mType == OVERWRITABLE) {
        flush(event.mEventName, TD_EVENT_TYPE_TRACK_OVERWRITE, event.mProperties, event.mExtraId, "");
    }
}

void ThinkingSDKObject::flushUser(string eventType, const TDJSONObject &properties)
{
    flush("", eventType, properties, "", "");
}

void ThinkingSDKObject::flush(string eventName, string eventType, const TDJSONObject &properties, string eventID, string firstCheckID)
{
    
    if (this->m_enable == false) {
        return;
    }
    
    if (this->m_optOutTracking == true) {
        return;
    }
    
    TDJSONObject finalDic;
    TDJSONObject propertyDic;
    
    bool isTrackEvent = eventType == TD_EVENT_TYPE || eventType == TD_EVENT_TYPE_TRACK_FIRST || eventType == TD_EVENT_TYPE_TRACK_UPDATE || eventType == TD_EVENT_TYPE_TRACK_OVERWRITE;
    

    if (isTrackEvent && eventType == TD_EVENT_TYPE_TRACK_FIRST) {
        eventType = TD_EVENT_TYPE;
        char *firstCheckIDString = (char *)firstCheckID.data();
        if (string(firstCheckIDString).size() == 0) {
            finalDic.setString("#first_check_id", m_deviceId.c_str());
        } else {
            finalDic.setString("#first_check_id", firstCheckIDString);
        }
    } else if (isTrackEvent && (eventType == TD_EVENT_TYPE_TRACK_UPDATE || eventType == TD_EVENT_TYPE_TRACK_OVERWRITE)) {
        char *eventIDString = (char *)eventID.data();
        finalDic.setString("#event_id", eventIDString);
    }

    timeb t;
    ftime(&t);
    finalDic.setDateTime("#time", t.time, t.millitm);
    

    finalDic.setString("#uuid", td_getUUID().c_str());
    
    
    if (m_accountId.size()>0) {
        finalDic.setString("#account_id", (char*)m_accountId.c_str());
    }
    
    
    if (m_distinctId.size()>0) {
        finalDic.setString("#distinct_id", (char*)m_distinctId.c_str());
    }
    
    
    if (eventType.size()>0) {
        finalDic.setString("#type", (char*)eventType.c_str());
    }
    
   
    if (isTrackEvent && eventName.size()>0) {
        finalDic.setString("#event_name", (char*)eventName.c_str());
    }
    
    
    map<string, TD_MAP_TIME_EVENT>::iterator timeMap = maptimeEvents.find(m_appid);
    if(timeMap!=maptimeEvents.end()) {
        TD_MAP_TIME_EVENT map = maptimeEvents[m_appid];
        TD_MAP_TIME_EVENT::iterator key = map.find(eventName);
        if(key!=map.end()) {
            long long timess = map[eventName];
            timeb t;
            ftime(&t);
            if (timess>0) {
                long long offsettime = (long long)(t.time * 1000 + t.millitm - timess);
                if (offsettime > 0) {
                    propertyDic.setNumber("#duration", offsettime/1000.0);
                }
            }
            map.erase(key);
        }
    }
    
 
    
    

    if (isTrackEvent) {

        TDJSONObject *presetJson = ThinkingAnalyticsAPI::getPresetProperties(m_appid)->toEventPresetProperties();
        propertyDic.mergeFrom(*presetJson);

        propertyDic.mergeFrom(m_superProperties);
        
        GetDynamicSuperProperties dynamicSuperProperties = mapDyldProperties[m_appid];
        if (dynamicSuperProperties != NULL) {
            propertyDic.mergeFrom(dynamicSuperProperties());
        }
    }
    
    propertyDic.mergeFrom(properties);
    
    finalDic.setJsonObject("properties", propertyDic);
    
    string json = TDJSONObject::toJson(finalDic);
    string data = "data="+json+"&appid="+m_appid;

    string url = m_server+"/sync_data";
    string postData = data;
    
    if (showLog == true) {
        CCLOG("appid:%s", m_appid.c_str());
        CCLOG("url:%s", url.c_str());
        CCLOG("name:%s", eventName.c_str());
        CCLOG("type:%s", eventType.c_str());
        CCLOG("data:%s", json.c_str());
        CCLOG("");CCLOG("");
    }
    
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(url);
    request->setRequestData(postData.c_str(), strlen(postData.c_str()));
    request->setResponseCallback(CC_CALLBACK_2(ThinkingSDKObject::complete, this));
    HttpClient::getInstance()->send(request);
    request->release();
}

void ThinkingSDKObject::complete(HttpClient *client, HttpResponse *response){
    
    if (showLog == true) {
        
        CCLOG("response code is:%ld",response->getResponseCode());
        if (response->isSucceed()){
            std::vector<char> * data = response->getResponseData();
            std::string str1 ;
            
            CCLOG("response data is:");
            
            for (int i = 0; i < (data->size()); i++){
                str1 = str1 + (*data)[i];
            }
            CCLOG("%s", str1.c_str());
        }
        else{
            CCLOG("error msg is:%s", response->getErrorBuffer());
        }
    }
}



string ThinkingSDKObject::getName() {
    return m_name;
}
string ThinkingSDKObject::getAppId() {
    return m_appid;
}
string ThinkingSDKObject::getServer() {
    return m_server;
}
string ThinkingSDKObject::getDistinctId() {
    return m_distinctId;
}
string ThinkingSDKObject::getAccountId() {
    return m_accountId;
}
string ThinkingSDKObject::getDeviceId() {
    
    if (m_deviceId.size() > 0)
    {
        return m_deviceId;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    m_deviceId = td_getMacUUID();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    m_deviceId = td_getWindowsUUID();
#endif
    return m_deviceId;
}

void ThinkingSDKObject::setName(string name) {
    m_name = name;
}
void ThinkingSDKObject::setAppId(string appId) {
    m_appid = appId;
}
void ThinkingSDKObject::setServer(string server) {
    m_server = server;
}
void ThinkingSDKObject::setDistinctId(string distinctId) {
    m_distinctId = distinctId;
    cocos2d::UserDefault::getInstance()->setStringForKey(getDistinctIdKey().c_str(), distinctId);
    cocos2d::UserDefault::getInstance()->flush();
}
void ThinkingSDKObject::setAccountId(string accountId) {
    m_accountId = accountId;
    cocos2d::UserDefault::getInstance()->setStringForKey(getAccountIdKey().c_str(), m_accountId);
    cocos2d::UserDefault::getInstance()->flush();
}
void ThinkingSDKObject::clearAccountId() {
    m_accountId = "";
    cocos2d::UserDefault::getInstance()->setStringForKey(getAccountIdKey().c_str(), m_accountId);
    cocos2d::UserDefault::getInstance()->flush();
}
void ThinkingSDKObject::enableTracking(bool enabled) {
    m_enable = enabled;
    
    cocos2d::UserDefault::getInstance()->setStringForKey(getDisEnableKey().c_str(), (enabled == false)? "1":"0");
    cocos2d::UserDefault::getInstance()->flush();
}
void ThinkingSDKObject::optOutTrackingAndDeleteUser() {

    TDJSONObject obj = TDJSONObject();
    flushUser("user_del", obj);
    optOutTracking();
}
void ThinkingSDKObject::optOutTracking() {
 
    m_optOutTracking = true;
    m_distinctId = m_deviceId;
    m_accountId = "";
    m_superProperties = TDJSONObject();
    
    cocos2d::UserDefault::getInstance()->setStringForKey(getAccountIdKey().c_str(), "");
    cocos2d::UserDefault::getInstance()->setStringForKey(getDistinctIdKey().c_str(), "");
    cocos2d::UserDefault::getInstance()->setStringForKey(getSuperPropertyKey().c_str(), "");
    cocos2d::UserDefault::getInstance()->setStringForKey(getoptOutKey().c_str(), "1");
    cocos2d::UserDefault::getInstance()->flush();
}
void ThinkingSDKObject::optInTracking() {
    m_optOutTracking = false;
    cocos2d::UserDefault::getInstance()->setStringForKey(getoptOutKey().c_str(), "0");
    cocos2d::UserDefault::getInstance()->flush();
}

string ThinkingSDKObject::getAccountIdKey() {
    return "td"+m_appid+"accountId";;
}

string ThinkingSDKObject::getDistinctIdKey() {
    return "td"+m_appid+"distinctId";
}

string ThinkingSDKObject::getSuperPropertyKey() {
    return "td"+m_appid+"superProperty";
}

string ThinkingSDKObject::getDeviceIdKey() {
    return "td"+m_appid+"deviceId";;
}

string ThinkingSDKObject::getDisEnableKey() {
    return "td"+m_appid+"disenable";
}

string ThinkingSDKObject::getoptOutKey() {
    return "td"+m_appid+"optout";
}

#endif
