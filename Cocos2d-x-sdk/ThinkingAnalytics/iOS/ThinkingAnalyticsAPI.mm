#import <ThinkingSDK/ThinkingSDK.h>
#import <ThinkingSDK/ThinkingAnalyticsCocosAPI.h>
#import <Foundation/Foundation.h>
#include "../Common/ThinkingAnalyticsAPI.h"
using  namespace thinkingdata;
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
void ThinkingAnalyticsAPI::init(string appid, string server)
{
    Config config(appid,server);
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


void ThinkingAnalyticsAPI::init(Config config)
{
    NSString* _appid = NSStringFromCString(config.getAppId().c_str());
    NSString* _server = NSStringFromCString(config.getServer().c_str());
    TDConfig *_config = [[TDConfig alloc] initWithAppId:_appid serverUrl:_server];
    if(config.getModel() == TD_DEBUG)
    {
        _config.debugMode = ThinkingAnalyticsDebug;
    }else if(config.getModel() == TD_DEBUG_ONLY)
    {
        _config.debugMode = ThinkingAnalyticsDebugOnly;
    }
    if (config.getEnableEncrypt() == true) {
        TDSecretKey *_secretKey = [[TDSecretKey alloc] initWithVersion:config.getSecretKey().version
                                                          publicKey:NSStringFromCString(config.getSecretKey().publicKey.c_str())];
        _config.secretKey = _secretKey;
        _config.enableEncrypt = true;
    }
    if (config.getPinningMode() != TASSLPinningNone) {
        TDSecurityPolicy *securityPolicy = [TDSecurityPolicy policyWithPinningMode:config.getPinningMode() withPinnedCertificates:certificatesInBundle([NSBundle mainBundle])];
        _config.securityPolicy = securityPolicy;
        [_config.securityPolicy setSessionDidReceiveAuthenticationChallenge:^NSURLSessionAuthChallengeDisposition(NSURLSession * _Nullable session, NSURLAuthenticationChallenge * _Nullable challenge, NSURLCredential * _Nullable * _Nullable credential) {
            return NSURLSessionAuthChallengePerformDefaultHandling;
        }];
    }
    _config.name = NSStringFromCString(config.getName().c_str());
    [ThinkingAnalyticsCocosAPI setCustomerLibInfoWithLibName:NSStringFromCString(LIB_NAME.c_str()) libVersion:NSStringFromCString(LIB_VERSION.c_str())];
    [ThinkingAnalyticsCocosAPI sharedInstance:_config];
    
}
TDJSONObject getDynamicProperties(string appid)
{
    TDJSONObject jsonObject;
    if(dynamicPropertiesMap.size() > 0 && dynamicPropertiesMap.count(ThinkingAnalyticsAPI::currentAppId(appid)) > 0)
    {
        GetDynamicSuperProperties  getDynamicSuperProperties = dynamicPropertiesMap[ThinkingAnalyticsAPI::currentAppId(appid)];
        
        jsonObject = getDynamicSuperProperties();
    }
    return jsonObject;
}

static void trackEvent(ThinkingAnalyticsEvent* event,int type, string appid)
{
    NSString* eventName = NSStringFromCString((*event).mEventName.c_str());
    NSString* extraId = NSStringFromCString((*event).mExtraId.c_str());
    TDJSONObject _properties = getDynamicProperties(appid);
    _properties.mergeFrom((*event).mProperties);
    NSDictionary* properties = NSDictionaryFromJSONObject(_properties);
    [ThinkingAnalyticsCocosAPI track:eventName properties:properties extraId:extraId type:type appid:NSStringFromCString(appid.c_str())];
}

//ThinkingSDK content
void ThinkingAnalyticsAPI::track(string eventName,string appid)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appid);
    [ThinkingAnalyticsCocosAPI track:NSStringFromCString(eventName.c_str()) properties:NSDictionaryFromJSONObject(dynamicProperties) appid:NSStringFromCString(appid.c_str())];
    
}
void ThinkingAnalyticsAPI::track(string eventName,const TDJSONObject& properties,string appid)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appid);
    dynamicProperties.mergeFrom(properties);
    [ThinkingAnalyticsCocosAPI track:NSStringFromCString(eventName.c_str()) properties:NSDictionaryFromJSONObject(dynamicProperties) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::track(TDFirstEvent* firstEvent,string appid) {
    trackEvent(firstEvent, 1, appid);
}
void ThinkingAnalyticsAPI::track(TDUpdatableEvent* updatableEvent,string appid)
{
    trackEvent(updatableEvent, 2, appid);
}
void ThinkingAnalyticsAPI::track(TDOverWritableEvent* overWritableEvent,string appid) {
    trackEvent(overWritableEvent, 3, appid);
}

void ThinkingAnalyticsAPI::timeEvent(string eventName,string appid)
{
    [ThinkingAnalyticsCocosAPI timeEvent:NSStringFromCString(eventName.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::login(string accountID,string appid)
{
    [ThinkingAnalyticsCocosAPI login:NSStringFromCString(accountID.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::logout(string appid)
{
    [ThinkingAnalyticsCocosAPI logout:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::identify(string distinctId,string appid)
{
    [ThinkingAnalyticsCocosAPI identify:NSStringFromCString(distinctId.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::user_set(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI user_set:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::user_setOnce(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI user_setOnce:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::user_append(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI user_append:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::user_uniqAppend(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI user_uniqAppend:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::user_add(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI user_add:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::user_delete(string appid)
{
    [ThinkingAnalyticsCocosAPI user_delete:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::user_unset(string propertyName,string appid)
{
    [ThinkingAnalyticsCocosAPI user_unset:NSStringFromCString(propertyName.c_str()) appid:NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::enableAutoTrack(string appid)
{
    [ThinkingAnalyticsCocosAPI enableAutoTrack:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::enableAutoTrack(const TDJSONObject &properties, TAAutoTrackType eventType, string appId) {
    [ThinkingAnalyticsCocosAPI enableAutoTrack:NSStringFromCString(appId.c_str()) eventType:eventType customMap:NSDictionaryFromJSONObject(properties)];
}

void ThinkingAnalyticsAPI::setSuperProperties(const TDJSONObject &properties,string appid)
{
    [ThinkingAnalyticsCocosAPI setSuperProperties:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::clearSuperProperties(string appid)
{
    [ThinkingAnalyticsCocosAPI clearSuperProperties:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::unsetSuperProperty(string superPropertyName,string appid)
{
    [ThinkingAnalyticsCocosAPI unsetSuperProperty:NSStringFromCString(superPropertyName.c_str()) appid :NSStringFromCString(appid.c_str())];
}

void ThinkingAnalyticsAPI::flush(string appid)
{
    [ThinkingAnalyticsCocosAPI flush:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::optOutTrackingAndDeleteUser(string appid)
{
    [ThinkingAnalyticsCocosAPI optOutTrackingAndDeleteUser:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::optOutTracking(string appid)
{
    [ThinkingAnalyticsCocosAPI optOutTracking:NSStringFromCString(appid.c_str())];
}
void ThinkingAnalyticsAPI::optInTracking(string appid)
{
    [ThinkingAnalyticsCocosAPI optInTracking:NSStringFromCString(appid.c_str())];
}
 void ThinkingAnalyticsAPI::enableTracking(bool enabled,string appid)
 {
     [ThinkingAnalyticsCocosAPI enableTracking:enabled appid:NSStringFromCString(appid.c_str())];
 }
void ThinkingAnalyticsAPI::enableTrackLog(bool enabled)
 {
     [ThinkingAnalyticsCocosAPI enableTrackLog:enabled];
 }
string ThinkingAnalyticsAPI::getDeviceId()
{
    NSString* deviceId = [ThinkingAnalyticsCocosAPI getDeviceId];
    return CStringFromNSString(deviceId);
}
string ThinkingAnalyticsAPI::getDistinctId(string appid)
{
    NSString* distinctId = [ThinkingAnalyticsCocosAPI getDistinctId:NSStringFromCString(appid.c_str())];
    return CStringFromNSString(distinctId);
}
string ThinkingAnalyticsAPI::createLightInstance(string appid)
{
    NSString *_appid = [ThinkingAnalyticsCocosAPI createLightInstance:NSStringFromCString(appid.c_str())];
    string instance = CStringFromNSString(_appid);
    return  instance;
}
void ThinkingAnalyticsAPI::calibrateTime(long long timestamp)
{
    [ThinkingAnalyticsCocosAPI calibrateTime:timestamp];
}
void ThinkingAnalyticsAPI::calibrateTimeWithNtp(string ntpServer)
{
    [ThinkingAnalyticsCocosAPI calibrateTimeWithNtp:NSStringFromCString(ntpServer.c_str())];
}
void ThinkingAnalyticsAPI::setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties,string appid)
{
    if(currentAppId(appid) == "")
    {
        printf("%s","Please initialize the SDK first");
    }else
    {
        dynamicPropertiesMap[currentAppId(appid)] = getDynamicSuperProperties;
    }
}

string ThinkingAnalyticsAPI::getLocalRegion() {
    return "";
}

string ThinkingAnalyticsAPI::currentAppId(string appId)
{
    NSString* _appId  = [ThinkingAnalyticsCocosAPI currentAppId:NSStringFromCString(appId.c_str())];
    return CStringFromNSString(_appId);
}
TDJSONObject ThinkingAnalyticsAPI::getSuperProperties(string appid)
{
    NSDictionary* superPropertiesDic = [ThinkingAnalyticsCocosAPI getSuperProperties:NSStringFromCString(appid.c_str())];
    return JSONObjectFromNSDictionary(superPropertiesDic);
}

PresetProperties* ThinkingAnalyticsAPI::getPresetProperties(string appid)
{
    TDPresetProperties* properties = [ThinkingAnalyticsCocosAPI getPresetProperties:NSStringFromCString(appid.c_str())];
    NSDictionary *pppppp = [properties toEventPresetProperties];
    PresetProperties* presetProperties = new PresetProperties();
    presetProperties->bundleId = CStringFromNSString(properties.bundle_id);;
    presetProperties->carrier  = CStringFromNSString(properties.carrier);
    presetProperties->deviceId = CStringFromNSString(properties.device_id);
    presetProperties->deviceModel = CStringFromNSString(properties.device_model);
    presetProperties->manufacturer = CStringFromNSString(properties.manufacturer);
    presetProperties->networkType = CStringFromNSString(properties.network_type);
    presetProperties->os = CStringFromNSString(properties.os);
    presetProperties->osVersion = CStringFromNSString(properties.os_version);
    presetProperties->screenHeight = properties.screen_height.intValue;
    presetProperties->screenWidth = properties.screen_width.intValue;
    presetProperties->systemLanguage = CStringFromNSString(properties.system_language);
    presetProperties->zoneOffset = properties.zone_offset.doubleValue;
    presetProperties->appVersion = CStringFromNSString(pppppp[@"#app_version"]);
    presetProperties->fps = ((NSNumber *)pppppp[@"#fps"]).intValue;
    presetProperties->disk = CStringFromNSString(pppppp[@"#disk"]);
    presetProperties->ram = CStringFromNSString(pppppp[@"#ram"]);
    presetProperties->isSimulator = ((NSNumber *)pppppp[@"#simulator"]).intValue;
    
    return  presetProperties;
}


void ThinkingAnalyticsAPI::enableThirdPartySharing(TAThirdPartyType type, string appId)
{
    ThinkingAnalyticsAPI::enableThirdPartySharing(type, TDJSONObject(), appId);
}

void ThinkingAnalyticsAPI::enableThirdPartySharing(TAThirdPartyType type, const TDJSONObject &properties, string appId)
{
    [ThinkingAnalyticsCocosAPI enableThirdPartySharing:type customMap:NSDictionaryFromJSONObject(properties) appid:NSStringFromCString(appId.c_str())];
}

void ThinkingAnalyticsAPI::setTrackStatus(TATrackType status, string appId)
{
    [ThinkingAnalyticsCocosAPI setTrackStatus:status appid:NSStringFromCString(appId.c_str())];
}
