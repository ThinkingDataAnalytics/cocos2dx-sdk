#import "TDAnalyticsCocosAPI.h"
#import <Foundation/Foundation.h>
#include "../Common/TDAnalytics.h"
using  namespace thinkingdata::analytics;
using  namespace std;

static NSString *NSStringFromCString(const char* cstr){
    return cstr ? [NSString stringWithUTF8String:cstr] : nil;
}

static char *CStringFromNSString(NSString *string) {
    return (char *)[string UTF8String];
}
static NSDictionary *NSDictionaryFromJSONObject(const TDJSONObject &jsonObject) {
    string json = TDJSONObject::toJson(jsonObject);
    NSString *string = NSStringFromCString(json.c_str());
    NSData *data = [string dataUsingEncoding:NSUTF8StringEncoding];
    
    if (!data) return nil;
    return [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
}
static std::vector<string> ListFromNSArray(NSArray* array)
{
    vector<string> list;
    for(NSString* value in array)
    {
        list.push_back(CStringFromNSString(value));
    }
    return list;
}


static TDJSONObject JSONObjectFromNSDictionary(NSDictionary* dic)
{
    TDJSONObject object;
    for(int i = 0 ; i <dic.allKeys.count ; i++)
    {
        NSString* key = dic.allKeys[i];
        NSObject* value = dic[key];
        if([value isKindOfClass:[NSString class]])
        {
            object.setString(CStringFromNSString(key), CStringFromNSString((NSString*)value));
        }else if([value isKindOfClass:[NSNumber class]])
        {
            NSNumber *number = (NSNumber*)value;
            if (strcmp([number objCType], @encode(long)) == 0)
            {
                int64_t num = [number longValue];
                object.setNumber(CStringFromNSString(key),num);
            }else if (strcmp([number objCType], @encode(long long)) == 0)
            {
                int64_t num = [number longLongValue];
                object.setNumber(CStringFromNSString(key), num);
            }
            else if (strcmp([number objCType], @encode(int)) == 0)
            {
                int32_t num = [number intValue];
                object.setNumber(CStringFromNSString(key), num);
            }
            else if (strcmp([number objCType], @encode(BOOL)) == 0)
            {
                object.setNumber(CStringFromNSString(key), number.boolValue);
            }else if (strcmp([number objCType], @encode(double)) == 0)
            {
                object.setNumber(CStringFromNSString(key), number.doubleValue);
            }
            else if (strcmp([number objCType], @encode(float)) == 0)
            {
                object.setNumber(CStringFromNSString(key), number.floatValue);
            }
        }else if([value isKindOfClass:[NSArray class]])
        {
            if ([((NSArray *)value).firstObject isKindOfClass:[NSDictionary class]]) {
                NSArray<NSDictionary *> *tmpValue = (NSArray<NSDictionary *> *)value;
                vector<TDJSONObject> list;
                for(NSDictionary* item in tmpValue) {
                    TDJSONObject obj;
                    obj = JSONObjectFromNSDictionary(item);
                    list.push_back(obj);
                }
                object.setList(CStringFromNSString(key), list);
            } else {
                object.setList(CStringFromNSString(key), ListFromNSArray((NSArray*)value));
            }
            
        }else if([value isKindOfClass:[NSDictionary class]])
        {
            TDJSONObject obj;
            obj = JSONObjectFromNSDictionary((NSDictionary *)value);
            object.setJsonObject(CStringFromNSString(key), obj);
            
        }
    }
    return object;
}

static char *CStringFromNSDictionary(NSDictionary *dic) {
    
    if (!dic) return nil;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
    
    if (!jsonData) return nil;
    NSString *jsonStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    return (char *)[jsonStr UTF8String];
}
void TDAnalytics::init(string appid, string server)
{
    TDConfig config(appid,server);
    init(config);
}

NSSet *certificatesInBundle(NSBundle *bundle) {
    NSArray *paths = [bundle pathsForResourcesOfType:@"cer" inDirectory:@"."];

    NSMutableSet *certificates = [NSMutableSet setWithCapacity:[paths count]];
    for (NSString *path in paths) {
        NSData *certificateData = [NSData dataWithContentsOfFile:path];
        [certificates addObject:certificateData];
    }

    return [NSSet setWithSet:certificates];
}


void TDAnalytics::init(TDConfig config)
{
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    [params setObject:NSStringFromCString(config.getAppId().c_str()) forKey:@"appId"];
    [params setObject:NSStringFromCString(config.getServer().c_str()) forKey:@"serverUrl"];
    [params setObject:@"" forKey:@""];
    if(config.getModel() == TD_DEBUG)
    {
        [params setObject:@"mode" forKey:@"debug"];
    }
    else if(config.getModel() == TD_DEBUG_ONLY)
    {
        [params setObject:@"mode" forKey:@"debugOnly"];
    }
    if (config.getEnableEncrypt() == true) {
        [params setObject:@"enableEncrypt" forKey:@(YES)];
        [params setObject:@"version" forKey:@(config.getSecretKey().version)];
        [params setObject:@"publicKey" forKey:NSStringFromCString(config.getSecretKey().publicKey.c_str())];
    }
    if (config.getPinningMode() != TDSSLPinningModeNone) {
        [params setObject:@"pinningMode" forKey:@(config.getPinningMode())];
    }
    [params setObject:@"name" forKey:NSStringFromCString(config.getName().c_str())];

    [TDAnalyticsCocosAPI setCustomerLibInfoWithLibName:NSStringFromCString(LIB_NAME.c_str()) libVersion:NSStringFromCString(LIB_VERSION.c_str())];
    
    [TDAnalyticsCocosAPI sharedInstance:params];
}

static TDJSONObject getDynamicProperties(string appid)
{
    TDJSONObject jsonObject;
    if(dynamicPropertiesMap.size() > 0 && dynamicPropertiesMap.count(TDAnalytics::currentAppId(appid)) > 0)
    {
        GetDynamicSuperProperties  getDynamicSuperProperties = dynamicPropertiesMap[TDAnalytics::currentAppId(appid)];
        
        jsonObject = getDynamicSuperProperties();
    }
    return jsonObject;
}

static void trackEvent(TDEventModel* event,int type, string appid)
{
    NSString* eventName = NSStringFromCString((*event).mEventName.c_str());
    NSString* extraId = NSStringFromCString((*event).mExtraId.c_str());
    TDJSONObject _properties = getDynamicProperties(appid);
    _properties.mergeFrom((*event).mProperties);
    NSDictionary* properties = NSDictionaryFromJSONObject(_properties);
    [TDAnalyticsCocosAPI track:eventName properties:properties extraId:extraId type:type appid:NSStringFromCString(appid.c_str())];
}

//ThinkingSDK content
void TDAnalytics::track(string eventName,string appid)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appid);
    [TDAnalyticsCocosAPI track:NSStringFromCString(eventName.c_str()) properties:NSDictionaryFromJSONObject(dynamicProperties) appid:NSStringFromCString(appid.c_str())];
    
}
void TDAnalytics::track(string eventName,const TDJSONObject& properties,string appid)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appid);
    dynamicProperties.mergeFrom(properties);
    [TDAnalyticsCocosAPI track:NSStringFromCString(eventName.c_str()) properties:NSDictionaryFromJSONObject(dynamicProperties) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::track(TDFirstEventModel* firstEvent,string appid) {
    trackEvent(firstEvent, 1, appid);
}
void TDAnalytics::track(TDUpdatableEventModel* updatableEvent,string appid)
{
    trackEvent(updatableEvent, 2, appid);
}
void TDAnalytics::track(TDOverwritableEventModel* overWritableEvent,string appid) {
    trackEvent(overWritableEvent, 3, appid);
}

void TDAnalytics::timeEvent(string eventName,string appid)
{
    [TDAnalyticsCocosAPI timeEvent:NSStringFromCString(eventName.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::login(string accountID,string appid)
{
    [TDAnalyticsCocosAPI login:NSStringFromCString(accountID.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::logout(string appid)
{
    [TDAnalyticsCocosAPI logout:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::setDistinctId(string distinctId,string appid)
{
    [TDAnalyticsCocosAPI setDistinctId:NSStringFromCString(distinctId.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::userSet(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI user_set:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::userSetOnce(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI user_setOnce:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::userAppend(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI user_append:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::userUniqAppend(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI user_uniqAppend:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::userAdd(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI user_add:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::userDelete(string appid)
{
    [TDAnalyticsCocosAPI user_delete:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::userUnset(string propertyName,string appid)
{
    [TDAnalyticsCocosAPI user_unset:NSStringFromCString(propertyName.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void TDAnalytics::enableAutoTrack(string appid)
{
    [TDAnalyticsCocosAPI enableAutoTrack:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::enableAutoTrack(const TDJSONObject &properties, TDAutoTrackEventType eventType, string appId) {
    [TDAnalyticsCocosAPI enableAutoTrack:NSStringFromCString(appId.c_str()) eventType:eventType customMap:NSDictionaryFromJSONObject(properties)];
}

void TDAnalytics::setSuperProperties(const TDJSONObject &properties,string appid)
{
    [TDAnalyticsCocosAPI setSuperProperties:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::clearSuperProperties(string appid)
{
    [TDAnalyticsCocosAPI clearSuperProperties:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::unsetSuperProperty(string superPropertyName,string appid)
{
    [TDAnalyticsCocosAPI unsetSuperProperty:NSStringFromCString(superPropertyName.c_str()) appid :NSStringFromCString(appid.c_str())];
}

void TDAnalytics::flush(string appid)
{
    [TDAnalyticsCocosAPI flush:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::optOutTrackingAndDeleteUser(string appid)
{
    [TDAnalyticsCocosAPI optOutTrackingAndDeleteUser:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::optOutTracking(string appid)
{
    [TDAnalyticsCocosAPI optOutTracking:NSStringFromCString(appid.c_str())];
}
void TDAnalytics::optInTracking(string appid)
{
    [TDAnalyticsCocosAPI optInTracking:NSStringFromCString(appid.c_str())];
}
 void TDAnalytics::enableTracking(bool enabled,string appid)
 {
     [TDAnalyticsCocosAPI enableTracking:enabled appid:NSStringFromCString(appid.c_str())];
 }
void TDAnalytics::enableTrackLog(bool enabled)
 {
     [TDAnalyticsCocosAPI enableTrackLog:enabled];
 }
string TDAnalytics::getDeviceId()
{
    NSString* deviceId = [TDAnalyticsCocosAPI getDeviceId];
    return CStringFromNSString(deviceId);
}
string TDAnalytics::getDistinctId(string appid)
{
    NSString* distinctId = [TDAnalyticsCocosAPI getDistinctId:NSStringFromCString(appid.c_str())];
    return CStringFromNSString(distinctId);
}
string TDAnalytics::createLightInstance(string appid)
{
    NSString *_appid = [TDAnalyticsCocosAPI createLightInstance:NSStringFromCString(appid.c_str())];
    string instance = CStringFromNSString(_appid);
    return  instance;
}
void TDAnalytics::calibrateTime(long long timestamp)
{
    [TDAnalyticsCocosAPI calibrateTime:timestamp];
}
void TDAnalytics::calibrateTimeWithNtp(string ntpServer)
{
    [TDAnalyticsCocosAPI calibrateTimeWithNtp:NSStringFromCString(ntpServer.c_str())];
}
void TDAnalytics::setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties,string appid)
{
    if(currentAppId(appid) == "")
    {
        printf("[ThinkingEngine] Info: %s","Please initialize the SDK first");
    }else
    {
        dynamicPropertiesMap[currentAppId(appid)] = getDynamicSuperProperties;
        [TDAnalyticsCocosAPI setDynamicSuperProperties:^NSDictionary * _Nullable(NSString * _Nonnull appId) {
            TDJSONObject _properties = getDynamicProperties(appid);
            NSDictionary* properties = NSDictionaryFromJSONObject(_properties);
            return properties;
        } appid:NSStringFromCString(appid.c_str())];
    }
}

string TDAnalytics::getLocalRegion() {
    return "";
}

string TDAnalytics::currentAppId(string appId)
{
    NSString* _appId  = [TDAnalyticsCocosAPI currentAppId:NSStringFromCString(appId.c_str())];
    return CStringFromNSString(_appId);
}
TDJSONObject TDAnalytics::getSuperProperties(string appid)
{
    NSDictionary* superPropertiesDic = [TDAnalyticsCocosAPI getSuperProperties:NSStringFromCString(appid.c_str())];
    return JSONObjectFromNSDictionary(superPropertiesDic);
}

TDPresetProperties* TDAnalytics::getPresetProperties(string appid)
{
    NSDictionary* properties = [TDAnalyticsCocosAPI getPresetProperties:NSStringFromCString(appid.c_str())];
    TDPresetProperties* presetProperties = new TDPresetProperties();
    presetProperties->bundleId = CStringFromNSString(properties[@"#bundle_id"]);
    presetProperties->carrier  = CStringFromNSString(properties[@"#carrier"]);
    presetProperties->deviceId = CStringFromNSString(properties[@"#device_id"]);
    presetProperties->deviceModel = CStringFromNSString(properties[@"#device_model"]);
    presetProperties->manufacturer = CStringFromNSString(properties[@"#manufacturer"]);
    presetProperties->networkType = CStringFromNSString(properties[@"#network_type"]);
    presetProperties->os = CStringFromNSString(properties[@"#os"]);
    presetProperties->osVersion = CStringFromNSString(properties[@"#os_version"]);
    presetProperties->screenHeight = [properties[@"#screen_height"] intValue];
    presetProperties->screenWidth = [properties[@"#screen_width"] intValue];
    presetProperties->systemLanguage = CStringFromNSString(properties[@"#system_language"]);
    presetProperties->zoneOffset = [properties[@"#zone_offset"] doubleValue];
    presetProperties->appVersion = CStringFromNSString(properties[@"#app_version"]);
    presetProperties->fps = [properties[@"#fps"] intValue];
    presetProperties->disk = CStringFromNSString(properties[@"#disk"]);
    presetProperties->ram = CStringFromNSString(properties[@"#ram"]);
    presetProperties->isSimulator = [properties[@"#simulator"] intValue];
    
    return  presetProperties;
}


void TDAnalytics::enableThirdPartySharing(TDThirdPartyType type, string appId)
{
    TDAnalytics::enableThirdPartySharing(type, TDJSONObject(), appId);
}

void TDAnalytics::enableThirdPartySharing(TDThirdPartyType type, const TDJSONObject &properties, string appId)
{
    [TDAnalyticsCocosAPI enableThirdPartySharing:type customMap:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appId.c_str())];
}

void TDAnalytics::setTrackStatus(TDTrackStatus status, string appId)
{
    [TDAnalyticsCocosAPI setTrackStatus:status appid:NSStringFromCString(appId.c_str())];
}
