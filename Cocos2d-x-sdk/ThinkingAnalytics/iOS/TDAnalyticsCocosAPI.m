//
//  ThinkingAnalyticsCocoAPI.m
//  ThinkingEngine
//
//  Created by Hale Wang on 2021/4/1.
//

#import "TDAnalyticsCocosAPI.h"
#import "ThinkingAnalyticsSDK.h"
#import <ThinkingSDK/TDConfig.h>
#import <ThinkingSDK/TDPresetProperties.h>

static ThinkingAnalyticsSDK* instance;

static NSMutableDictionary* sInstances;
static NSMutableArray*      sAppIds;
@implementation TDAnalyticsCocosAPI
+ (NSString*)currentAppId:(NSString*)appId
{
    NSString *token = @"";
    if((appId == nil || appId.length == 0) && sAppIds.count > 0)
    {
        token = self.appIds[0];
    }else if(appId.length > 0){
        token = appId;
    }
    return token;
}
+(ThinkingAnalyticsSDK *)currentInstance:(NSString *)appid
{
    ThinkingAnalyticsSDK *instance;
    NSString *token = [self currentAppId:appid];
    if(token.length > 0)
    {
        instance = [self.instances objectForKey:token];
    }
    if(instance == nil)
    {
//        TDLogInfo(@"Instance does not exist");
    }
    return  instance;
}
+ (BOOL)isInit
{
    return self.appIds.count > 0;
}
+ (NSMutableArray* )appIds
{
    if(sAppIds == nil)
    {
        sAppIds = [NSMutableArray new];
    }
    return  sAppIds;
}
+ (NSMutableDictionary *)instances
{
    if(sInstances == nil)
    {
        sInstances = [NSMutableDictionary new];
    }
    return sInstances;
}
+ (void)setCustomerLibInfoWithLibName:(NSString *)libName libVersion:(nonnull NSString *)libVersion
{
    [ThinkingAnalyticsSDK setCustomerLibInfoWithLibName:libName libVersion:libVersion];
}
+ (ThinkingAnalyticsSDK *)sharedInstance:(NSString *)appid server:(NSString *)server
{
    NSMutableDictionary *config = [NSMutableDictionary dictionary];
    [config setObject:appid forKey:@"appId"];
    [config setObject:server forKey:@"serverUrl"];
    return [self sharedInstance:config];
}
+ (ThinkingAnalyticsSDK *)sharedInstance:(NSDictionary *)config
{
    NSString* appid = config[@"appId"];
    NSString* server = config[@"serverUrl"];
    TDConfig *tdConfig = [[TDConfig alloc] initWithAppId:appid serverUrl:server];
    NSString* _mode = config[@"mode"];
    if([_mode isEqualToString:@"debug"])
    {
        tdConfig.debugMode = ThinkingAnalyticsDebug;
    }
    else if([_mode isEqualToString:@"debugOnly"])
    {
        tdConfig.debugMode = ThinkingAnalyticsDebugOnly;
    }
    bool enableEncrypt = [config[@"enableEncrypt"] boolValue];
    if (enableEncrypt == true) {
        NSString* version = [config[@"version"] unsignedIntegerValue];
        NSString* publicKey = config[@"publicKey"];
        TDSecretKey *tdSecretKey = [[TDSecretKey alloc] initWithVersion:version
                                                          publicKey:publicKey];
        tdConfig.secretKey = tdSecretKey;
        tdConfig.enableEncrypt = enableEncrypt;
    }
    NSInteger pinningMode = [config[@"pinningMode"] integerValue];
    if (pinningMode != TDSSLPinningModeNone) {
        TDSecurityPolicy *securityPolicy = [TDSecurityPolicy policyWithPinningMode:pinningMode];
        tdConfig.securityPolicy = securityPolicy;
        [tdConfig.securityPolicy setSessionDidReceiveAuthenticationChallenge:^NSURLSessionAuthChallengeDisposition(NSURLSession * _Nullable session, NSURLAuthenticationChallenge * _Nullable challenge, NSURLCredential * _Nullable * _Nullable credential) {
            return NSURLSessionAuthChallengePerformDefaultHandling;
        }];
    }
    tdConfig.name = config[@"name"];

    NSString* name = tdConfig.name.length == 0 ? tdConfig.appid : tdConfig.name;
    ThinkingAnalyticsSDK *instance = self.instances[name];
    if(instance == nil)
    {
        instance = [ThinkingAnalyticsSDK startWithConfig:tdConfig];
        [self.instances setValue:instance forKey:name];
        [self.appIds addObject:name];
    }
    return instance;
}
+ (NSString*)createLightInstance:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        ThinkingAnalyticsSDK *lightInstance =  [[self currentInstance:appid] createLightInstance];
        NSString *uuid = [NSUUID UUID].UUIDString;
        self.instances[uuid] = lightInstance;
        return uuid;
    }else
    {
        return @"";
    }
}
+ (NSDictionary *)getPresetProperties:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        return [[[self currentInstance:appid] getPresetProperties] toEventPresetProperties];
    }else
    {
        return [NSDictionary new];
    }
}

+ (void)track:(NSString*)eventName appid:(NSString*)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] track:eventName];
    }
}

+ (void)track:(NSString *)eventName properties:(nullable NSDictionary *)properties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] track:eventName properties:properties];
    }
    
}

+ (void)track:(NSString *)eventName properties:(nullable NSDictionary *)properties extraId:(NSString *)extraId type:(int)type appid:(NSString *)appid
{
    
    TDEventModel *eventModel = nil;
    switch (type) {
        case 1:
        {
            if(extraId != nil && extraId.length > 0)
            {
                eventModel = [[TDFirstEventModel alloc] initWithEventName:eventName firstCheckID:extraId];
            }else
            {
                eventModel = [[TDFirstEventModel alloc] initWithEventName:eventName];
            }
        }
            break;
        case 2:
        {
            eventModel = [[TDUpdateEventModel alloc] initWithEventName:eventName eventID:extraId];
        }
            break;
        case  3:
        {
            eventModel = [[TDOverwriteEventModel alloc] initWithEventName:eventName eventID:extraId];
        }
            break;
        default:
            break;
    }
    if(eventModel != nil)
    {
        eventModel.properties = properties;
        if([self currentInstance:appid] != nil)
        {
           [[self currentInstance:appid] trackWithEventModel:eventModel];
        }
    }
}

+ (void)timeEvent:(NSString *)eventName appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] timeEvent:eventName];
    }
}

+ (void)login:(NSString *)accountID appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] login:accountID];
    }
}

+ (void)logout:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] logout];
    }
}

+ (void)setDistinctId:(NSString *)distinctId appid:(NSString *)appid
{
   if([self currentInstance:appid] != nil)
   {
       [[self currentInstance:appid] identify:distinctId];
   }
}

+ (NSString *)getDistinctId:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        return  [[self currentInstance:appid] getDistinctId];
    }
    return @"";
}

+ (void)user_set:(NSDictionary *)userProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
       [[self currentInstance:appid] user_set:userProperties];
    }
}

+ (void)user_setOnce:(NSDictionary *)userProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] user_setOnce:userProperties];
    }
}

+ (void)user_add:(NSDictionary *)userProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] user_add:userProperties];
    }
}

+ (void)user_append:(NSDictionary *)userProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid]user_append:userProperties];
    }
}

+ (void)user_uniqAppend:(NSDictionary *)userProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid]user_uniqAppend:userProperties];
    }
}

+ (void)user_delete:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] user_delete];
    }
}

+ (void)user_unset:(NSString *)propertyName appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] user_unset:propertyName];
    }
}

+ (void)setSuperProperties:(NSDictionary *)supperProperties appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] setSuperProperties:supperProperties];
    }
}

+ (NSDictionary *)getSuperProperties:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        return [[self currentInstance:appid] currentSuperProperties];
    }
    
    return @{};
    
}
+ (void)clearSuperProperties:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] clearSuperProperties];
    }
}

+ (void)unsetSuperProperty:(NSString *)supperPropertyName appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] unsetSuperProperty:supperPropertyName];
    }
}

+ (void)enableAutoTrack:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] enableAutoTrack:ThinkingAnalyticsEventTypeAppStart | ThinkingAnalyticsEventTypeAppInstall |
         ThinkingAnalyticsEventTypeAppEnd];
    }
}

+ (void)enableAutoTrack:(NSString *)appid eventType:(int)eventType customMap:(NSDictionary *)customMap {
    [[self currentInstance:appid] enableAutoTrack:eventType properties:customMap];
}

+ (void)flush:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] flush];
    }
}

+ (void)enableTracking:(BOOL)enabled appid:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] enableTracking:enabled];
    }
}

+ (void)optOutTrackingAndDeleteUser:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
       [[self currentInstance:appid] optOutTrackingAndDeleteUser];
    }
}

+ (void)optOutTracking:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] optOutTracking];
    }
    
}

+ (void)optInTracking:(NSString *)appid
{
    if([self currentInstance:appid] != nil)
    {
        [[self currentInstance:appid] optInTracking];
    }
    
}

+ (void)enableTrackLog:(BOOL)enableLog
{
    [ThinkingAnalyticsSDK setLogLevel:TDLoggingLevelDebug];
}
+ (void)calibrateTime:(long)timestamp
{
    [ThinkingAnalyticsSDK calibrateTime:timestamp];
}
+ (void)calibrateTimeWithNtp:(NSString *)ntpServer
{
    [ThinkingAnalyticsSDK calibrateTimeWithNtp:ntpServer];
}
+ (NSString *)getDeviceId
{
    if([self currentInstance:@""] != nil)
    {
        return [self currentInstance:@""].getDeviceId;
    }
    return @"";
}
+ (NSString *)getLocalRegion
{
    return [ThinkingAnalyticsSDK getLocalRegion];
}

+ (void)enableThirdPartySharing:(int)type customMap:(NSDictionary *)properties appid:(NSString *)appId {
    if([self currentInstance:appId] != nil)
    {
        [[self currentInstance:appId] enableThirdPartySharing:type customMap:properties];
    }
}

+ (void)setTrackStatus:(int)status appid:(NSString *)appId {
    if([self currentInstance:appId] != nil)
    {
        [[self currentInstance:appId] setTrackStatus:status];
    }
}

+ (void)setDynamicSuperProperties:(GetDynamicSuperPropertiesIOS)dynamicSuperProperties appid:(NSString *)appId {
    if([self currentInstance:appId] != nil)
    {
        [[self currentInstance:appId] registerDynamicSuperProperties:^NSDictionary<NSString *,id> * _Nonnull{
            return dynamicSuperProperties(appId);
        }];
    }
}

@end
