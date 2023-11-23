#include "../Common/TDAnalytics.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"
#include "cn_thinkingdata_analytics_TDAnalyticsCocosAPI.h"

using namespace thinkingdata::analytics;
USING_NS_CC;
#define THINKING_JAVA_CLASS "cn/thinkingdata/analytics/TDAnalyticsCocosAPI"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C"{
//
//static JniMethodInfo sMethodInfo;
//static bool isMethodExist(const char *methodName, const char *methodDescription) {
//    if (JniHelper::getStaticMethodInfo(sMethodInfo,THINKING_JAVA_CLASS,methodName,methodDescription)) {
//        return true;
//    }
//    return false;
//}

static string jStringToString(JNIEnv *env, jstring jString) {
    if (jString == NULL || env == NULL) {
        return "";
    }
    const char *chars = env->GetStringUTFChars(jString, NULL);
    string ret(chars);
    env->ReleaseStringUTFChars(jString, chars);
    return ret;
}

// java jsonobject cover to C++ TDJSONObject
static TDJSONObject TDJSONObjectFromJSONObject(JNIEnv *env,jobject object)
{

    TDJSONObject _object;
    jclass _class = env->GetObjectClass(object);
    jmethodID _keysMethod = env->GetMethodID(_class,"keys","()Ljava/util/Iterator;");
    jobject  _iterator = env->CallObjectMethod(object,_keysMethod);
    jclass  _iteratorClass = env->GetObjectClass(_iterator);
    jmethodID _hasNext = env->GetMethodID(_iteratorClass,"hasNext","()Z");
    jclass _stringClass = env->FindClass("java/lang/String");
    jclass _intClass = env->FindClass("java/lang/Integer");
    jclass _longClass = env->FindClass("java/lang/Long");
    jclass _boolClass = env->FindClass("java/lang/Boolean");
    jclass _jsonArrayClass = env->FindClass("org/json/JSONArray");
    jclass _doubleClass = env->FindClass("java/lang/Double");
    jclass _objectClass = env->FindClass("org/json/JSONObject");
    jmethodID  _getDoubleValue = env->GetMethodID(_doubleClass,"doubleValue","()D");
    jmethodID  _getBoolValue = env->GetMethodID(_boolClass,"booleanValue","()Z");
    jmethodID  _getIntValue  = env->GetMethodID(_intClass,"intValue","()I");
    jmethodID  _getLongValue = env->GetMethodID(_longClass,"longValue","()J");

    while (env->CallBooleanMethod(_iterator,_hasNext))
    {
        jmethodID  _next = env->GetMethodID(_iteratorClass,"next","()Ljava/lang/Object;");
        jstring  _key = (jstring)env->CallObjectMethod(_iterator,_next);
        jmethodID _get = env->GetMethodID(_class,"get","(Ljava/lang/String;)Ljava/lang/Object;");
        jobject  _value = env->CallObjectMethod(object,_get,_key);
        if(env->IsInstanceOf(_value,_stringClass))
        {
            _object.setString(jStringToString(env,((jstring)_key)).c_str(),jStringToString(env,((jstring)_value)).c_str());
        } else if(env->IsInstanceOf(_value,_intClass))
        {
            _object.setNumber(jStringToString(env,((jstring)_key)).c_str(),env->CallIntMethod(_value,_getIntValue));
        } else if(env->IsInstanceOf(_value,_boolClass))
        {
            _object.setBool(jStringToString(env,((jstring)_key)).c_str(),env->CallBooleanMethod(_value,_getBoolValue));
        } else if(env->IsInstanceOf(_value,_doubleClass))
        {
            _object.setNumber(jStringToString(env,((jstring)_key)).c_str(),env->CallDoubleMethod(_value,_getDoubleValue));
        } else if(env->IsInstanceOf(_value,_longClass))
        {
            _object.setNumber(jStringToString(env,((jstring)_key)).c_str(),env->CallLongMethod(_value,_getLongValue));
        }else if (env->IsInstanceOf(_value,_objectClass)) {
            _object.setJsonObject(jStringToString(env,((jstring)_key)).c_str(),TDJSONObjectFromJSONObject(env, _value));
        } else{

            jmethodID  _size = env->GetMethodID(_jsonArrayClass,"length","()I");
            vector<string> list;
            vector<TDJSONObject> listobjs;
            int  i = 0;
            jmethodID  _getString = env->GetMethodID(_jsonArrayClass,"getString","(I)Ljava/lang/String;");
            jmethodID  _getoptobj = env->GetMethodID(_jsonArrayClass,"optJSONObject","(I)Lorg/json/JSONObject;");

            for(;i<env->CallIntMethod(_value,_size);i++)
            {

                jobject json = (jobject)env->CallObjectMethod(_value,_getoptobj, i);
                if (json != NULL) {
                    listobjs.push_back(TDJSONObjectFromJSONObject(env, json));
                    env->DeleteLocalRef(json);
                } else {
                    jstring content = (jstring)env->CallObjectMethod(_value,_getString,i);
                    list.push_back(jStringToString(env,content));
                    env->DeleteLocalRef(content);
                }
            }
            if (listobjs.size() > 0) {
                _object.setList(jStringToString(env,((jstring)_key)).c_str(),listobjs);
            } else {
                _object.setList(jStringToString(env,((jstring)_key)).c_str(),list);
            }
        }
        env->DeleteLocalRef(_key);
        env->DeleteLocalRef(_value);
    }
    env->DeleteLocalRef(_class);
    env->DeleteLocalRef(_iterator);
    env->DeleteLocalRef(_iteratorClass);
    env->DeleteLocalRef(_stringClass);
    env->DeleteLocalRef(_intClass);
    env->DeleteLocalRef(_boolClass);
    env->DeleteLocalRef(_longClass);
    env->DeleteLocalRef(_doubleClass);
    env->DeleteLocalRef(_jsonArrayClass);
    return _object;

}
//static void releaseMethod() {
//    if (sMethodInfo.classID != NULL)
//        sMethodInfo.env->DeleteLocalRef(sMethodInfo.classID);
//}
static void releaseMethod(JniMethodInfo &method)
{
    if(method.methodID != NULL)
    {
        method.env->DeleteLocalRef(method.classID);
    }
}


static jobject createJavaJsonObject(JNIEnv *env, const TDJSONObject *properties) {
    jclass classJSONObject = env->FindClass("org/json/JSONObject");
    jmethodID constructMethod = env->GetMethodID(classJSONObject,
                                                 "<init>",
                                                 "(Ljava/lang/String;)V");
    string js = TDJSONObject::toJson(*properties);
    jobject objJSON = env->NewObject(classJSONObject, constructMethod,
                                     env->NewStringUTF(js.c_str()));
    env->DeleteLocalRef(classJSONObject);
    return objJSON;
}


static TDJSONObject getDynamicProperties(string appId)
{
    TDJSONObject jsonObject;
    if(!dynamicPropertiesMap.empty())
    {
        string currentAppId = TDAnalytics::currentAppId(appId);
        int count = dynamicPropertiesMap.count(currentAppId);
        if(count > 0)
        {
            GetDynamicSuperProperties  getDynamicSuperProperties = dynamicPropertiesMap[currentAppId];
            jsonObject = getDynamicSuperProperties();
        }
    }
    return jsonObject;
}

static string getDynamicPropertiesJson(string appId) {
    TDJSONObject jsonObject = getDynamicProperties(appId);
    string js = TDJSONObject::toJson(jsonObject);
    return js;
}

/*
 * Class:     cn_thinkingdata_analytics_TDAnalyticsCocosAPI
 * Method:    getDynamicPropertiesJson
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cn_thinkingdata_analytics_TDAnalyticsCocosAPI_getDynamicPropertiesJson
        (JNIEnv *env, jclass, jstring appId) {
    string _appId = jStringToString(env,appId);
    string json = getDynamicPropertiesJson(_appId);
    return env->NewStringUTF(json.c_str());
}

static void trackEvent(TDEventModel* event,int type,string appId)
{

    TDJSONObject dynamicProperties = getDynamicProperties(appId);
    dynamicProperties.mergeFrom((*event).mProperties);
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"track","(Ljava/lang/String;Lorg/json/JSONObject;Ljava/lang/String;ILjava/lang/String;)V"))
    {
        jstring jEventName = methodInfo.env->NewStringUTF((*event).mEventName.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());


        jobject jParam = createJavaJsonObject(methodInfo.env, &dynamicProperties);
        jstring jExtraID;
        string defaultId = "";
        if(!(*event).mExtraId.empty())
        {
            jExtraID = methodInfo.env->NewStringUTF((*event).mExtraId.c_str());
        }else
        {
            jExtraID = methodInfo.env->NewStringUTF(defaultId.c_str());
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jEventName,jParam,jExtraID,type,jAppId);
        methodInfo.env->DeleteLocalRef(jEventName);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jExtraID);
        methodInfo.env->DeleteLocalRef(jAppId);
    }
    releaseMethod(methodInfo);
}
}

void TDAnalytics::track(string eventName,string appId)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appId);
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"track","(Ljava/lang/String;Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jobject jParam = createJavaJsonObject(methodInfo.env,&dynamicProperties);
        jstring jEventName = methodInfo.env->NewStringUTF(eventName.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jEventName,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jEventName);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::track(string eventName,const TDJSONObject& node,string appId)
{
    TDJSONObject dynamicProperties = getDynamicProperties(appId);
    dynamicProperties.mergeFrom(node);
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"track","(Ljava/lang/String;Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jstring jEventName = methodInfo.env->NewStringUTF(eventName.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &dynamicProperties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jEventName,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jEventName);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::track(TDFirstEventModel* firstEvent,string appId) {

    trackEvent(firstEvent,1,appId);

}
void TDAnalytics::track(TDUpdatableEventModel* updatableEvent,string appId)
{
    trackEvent(updatableEvent,2,appId);

}
void TDAnalytics::track(TDOverwritableEventModel* overWritableEvent,string appId) {
    trackEvent(overWritableEvent,3,appId);
}

void TDAnalytics::timeEvent(string eventName,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"timeEvent","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jEventName = methodInfo.env->NewStringUTF(eventName.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jEventName,jAppId);
        methodInfo.env->DeleteLocalRef(jEventName);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}







void TDAnalytics::login(string accountID,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"login","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring account = methodInfo.env->NewStringUTF(accountID.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,account,jAppId);
        methodInfo.env->DeleteLocalRef(account);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::logout(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"logout","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::setDistinctId(string distinctId,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"setDistinctId","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring distinct = methodInfo.env->NewStringUTF(distinctId.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,distinct,jAppId);
        methodInfo.env->DeleteLocalRef(distinct);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::userSet(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_set","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::userSetOnce(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_setOnce","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::userAppend(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_append","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::userUniqAppend(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_uniqAppend","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::userAdd(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_add","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::userDelete(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_delete","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::userUnset(string propertyName,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"user_unset","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jstring name = methodInfo.env->NewStringUTF(propertyName.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,name,jAppId);
        methodInfo.env->DeleteLocalRef(name);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}


void TDAnalytics::enableAutoTrack(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableAutoTrack","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::enableAutoTrack(const TDJSONObject &properties, TDAutoTrackEventType eventType, string appid)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableAutoTrack","(Ljava/lang/String;ILorg/json/JSONObject;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appid.c_str());
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId, eventType, jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::setSuperProperties(const TDJSONObject &properties,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"setSuperProperties","(Lorg/json/JSONObject;Ljava/lang/String;)V"))
    {
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jParam,jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::clearSuperProperties(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"clearSuperProperties","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::unsetSuperProperty(string superPropertyName,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"unsetSuperProperty","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring name = methodInfo.env->NewStringUTF(superPropertyName.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,name,jAppId);
        methodInfo.env->DeleteLocalRef(name);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::flush(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"flush","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::optOutTrackingAndDeleteUser(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"optOutTrackingAndDeleteUser","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::optOutTracking(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"optOutTracking","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::optInTracking(string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"optInTracking","(Ljava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::enableTracking(bool enabled,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableTracking","(ZLjava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,enabled,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}
void TDAnalytics::enableTrackLog(bool enabled)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableTrackLog","(Z)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,enabled);
        releaseMethod(methodInfo);
    }
}

string TDAnalytics::getDeviceId()
{
    string deviceID;
    JniMethodInfo methodInfo;
    if(cocos2d::JniHelper::getStaticMethodInfo(methodInfo,"cn/thinkingdata/analytics/TDAnalyticsCocosAPI","getDeviceId","()Ljava/lang/String;"))
    {
        jstring result = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        deviceID = jStringToString(methodInfo.env,result);
        methodInfo.env->DeleteLocalRef(result);
        releaseMethod(methodInfo);
    }
    return deviceID;
}
string TDAnalytics::getDistinctId(string appId)
{
    string distinctID;
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"getDistinctId","(Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jstring result = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        distinctID = jStringToString(methodInfo.env,result);
        methodInfo.env->DeleteLocalRef(result);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
    return distinctID;
}
TDPresetProperties* TDAnalytics::getPresetProperties(string appId)
{
    TDPresetProperties *presetProperties = new TDPresetProperties();
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"getPresetProperties","(Ljava/lang/String;)Lcn/thinkingdata/analytics/TDPresetProperties;"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());

        jobject obj = (jobject)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        jclass clazz = methodInfo.env->GetObjectClass(obj);

        jfieldID screenWidthField = methodInfo.env->GetFieldID(clazz, "screenWidth", "I");
        jint screenWidth = methodInfo.env->GetIntField(obj,screenWidthField);
        presetProperties->screenWidth = screenWidth;

        jfieldID fpsField = methodInfo.env->GetFieldID(clazz, "fps", "I");
        jint fps = methodInfo.env->GetIntField(obj,fpsField);
        presetProperties->fps = fps;

        jfieldID ramField = methodInfo.env->GetFieldID(clazz, "ram", "Ljava/lang/String;");
        jstring ram = (jstring)methodInfo.env->GetObjectField(obj,ramField);
        presetProperties->ram = jStringToString(methodInfo.env,ram);;

        jfieldID diskField = methodInfo.env->GetFieldID(clazz, "disk", "Ljava/lang/String;");
        jstring disk = (jstring)methodInfo.env->GetObjectField(obj,diskField);
        presetProperties->disk = jStringToString(methodInfo.env,disk);;

        jfieldID isSimulatorField = methodInfo.env->GetFieldID(clazz, "isSimulator", "Z");
        jboolean isSimulator = methodInfo.env->GetBooleanField(obj,isSimulatorField);
        presetProperties->isSimulator = isSimulator;

        jfieldID appVersionField = methodInfo.env->GetFieldID(clazz, "appVersion", "Ljava/lang/String;");
        jstring appVersion = (jstring)methodInfo.env->GetObjectField(obj,appVersionField);
        presetProperties->appVersion = jStringToString(methodInfo.env,appVersion);;

        jfieldID screenHeightField = methodInfo.env->GetFieldID(clazz, "screenHeight", "I");
        jint screenHeight = methodInfo.env->GetIntField(obj,screenHeightField);
        presetProperties->screenHeight = screenHeight;

        jfieldID bundleIdField = methodInfo.env->GetFieldID(clazz, "bundleId", "Ljava/lang/String;");
        jstring bundleId = (jstring)methodInfo.env->GetObjectField(obj,bundleIdField);
        presetProperties->bundleId = jStringToString(methodInfo.env,bundleId);;

        jfieldID carrierField = methodInfo.env->GetFieldID(clazz, "carrier", "Ljava/lang/String;");
        jstring carrier = (jstring)methodInfo.env->GetObjectField(obj,carrierField);
        presetProperties->carrier = jStringToString(methodInfo.env,carrier);

        jfieldID deviceIdField = methodInfo.env->GetFieldID(clazz, "deviceId", "Ljava/lang/String;");
        jstring deviceId = (jstring)methodInfo.env->GetObjectField(obj,deviceIdField);
        presetProperties->deviceId = jStringToString(methodInfo.env,deviceId);

        jfieldID deviceModelField = methodInfo.env->GetFieldID(clazz, "deviceModel", "Ljava/lang/String;");
        jstring deviceModel = (jstring)methodInfo.env->GetObjectField(obj,deviceModelField);
        presetProperties->deviceModel = jStringToString(methodInfo.env,deviceModel);

        jfieldID manufactureField = methodInfo.env->GetFieldID(clazz, "manufacture", "Ljava/lang/String;");
        jstring manufacture = (jstring)methodInfo.env->GetObjectField(obj,manufactureField);
        presetProperties->manufacturer = jStringToString(methodInfo.env,manufacture);

        jfieldID networkTypeField = methodInfo.env->GetFieldID(clazz, "networkType", "Ljava/lang/String;");
        jstring networkType = (jstring)methodInfo.env->GetObjectField(obj,networkTypeField);
        presetProperties->networkType = jStringToString(methodInfo.env,networkType);

        jfieldID osField = methodInfo.env->GetFieldID(clazz, "os", "Ljava/lang/String;");
        jstring os = (jstring)methodInfo.env->GetObjectField(obj,osField);
        presetProperties->os = jStringToString(methodInfo.env,os);


        jfieldID osVersionField = methodInfo.env->GetFieldID(clazz, "osVersion", "Ljava/lang/String;");
        jstring osVersion = (jstring)methodInfo.env->GetObjectField(obj,osVersionField);
        presetProperties->osVersion = jStringToString(methodInfo.env,osVersion);


        jfieldID systemLanguageField = methodInfo.env->GetFieldID(clazz, "systemLanguage", "Ljava/lang/String;");
        jstring systemLanguage = (jstring)methodInfo.env->GetObjectField(obj,systemLanguageField);
        presetProperties->systemLanguage = jStringToString(methodInfo.env,systemLanguage);


        jfieldID zoneOffsetField = methodInfo.env->GetFieldID(clazz, "zoneOffset", "D");
        jdouble zoneOffset = methodInfo.env->GetDoubleField(obj,zoneOffsetField);
        presetProperties->zoneOffset = zoneOffset;

        methodInfo.env->DeleteLocalRef(obj);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
    return presetProperties;
}
void TDAnalytics::calibrateTime(long long timestamp) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"calibrateTime","(J)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,timestamp);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::calibrateTimeWithNtp(string ntpServer) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"calibrateTimeWithNtp","(Ljava/lang/String;)V"))
    {
        jstring _server = methodInfo.env->NewStringUTF(ntpServer.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,_server);
        methodInfo.env->DeleteLocalRef(_server);
        releaseMethod(methodInfo);
    }
}


TDJSONObject TDAnalytics::getSuperProperties(string appId) {
    TDJSONObject jsonObject;
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"getSuperProperties","(Ljava/lang/String;)Lorg/json/JSONObject;"))
    {
        jstring _appId = methodInfo.env->NewStringUTF(appId.c_str());
        jobject  _object = (jobject)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,_appId);
        jsonObject = TDJSONObjectFromJSONObject(methodInfo.env,_object);
        methodInfo.env->DeleteLocalRef(_appId);
        releaseMethod(methodInfo);
    }
    return jsonObject;
}

string TDAnalytics::currentAppId(string appId) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"currentToken","(Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        jstring currentAppId = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        string _appId = jStringToString(methodInfo.env,currentAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
        return _appId;
    }
    return "";
}

void TDAnalytics::setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties, string appId) {

    if(TDAnalytics::currentAppId(appId).size() != 0)
    {
        string currentAppId = TDAnalytics::currentAppId(appId);
        dynamicPropertiesMap[currentAppId] = getDynamicSuperProperties;

        JniMethodInfo methodInfo;
        if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"setDynamicSuperProperties","(Ljava/lang/String;)V"))
        {
            jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jAppId);
            methodInfo.env->DeleteLocalRef(jAppId);
            releaseMethod(methodInfo);
        }
    }

}
string TDAnalytics::createLightInstance(string appId) {
    string lightToken = "";
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"createLightInstance","(Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring _appId = methodInfo.env->NewStringUTF(appId.c_str());
        jstring _token = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,_appId);
        lightToken = jStringToString(methodInfo.env,_token);
        methodInfo.env->DeleteLocalRef(_appId);
        methodInfo.env->DeleteLocalRef(_token);
        releaseMethod(methodInfo);
    }
    return lightToken;
}

void TDAnalytics::init(TDConfig config) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"setCustomerLibInfo","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring libName = methodInfo.env->NewStringUTF(LIB_NAME.c_str());
        jstring libVersion = methodInfo.env->NewStringUTF(LIB_VERSION.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,libName,libVersion);
        methodInfo.env->DeleteLocalRef(libName);
        methodInfo.env->DeleteLocalRef(libVersion);
        releaseMethod(methodInfo);
    }

    cocos2d::JniMethodInfo jniGetContext;
    if (!cocos2d::JniHelper::getStaticMethodInfo(jniGetContext,
                                                 "org/cocos2dx/lib/Cocos2dxActivity",
                                                 "getContext",
                                                 "()Landroid/content/Context;")) {
        return;
    }
    jobject _context = (jobject) jniGetContext.env->CallStaticObjectMethod(
            jniGetContext.classID, jniGetContext.methodID);
    releaseMethod(jniGetContext);

    cocos2d::JniMethodInfo jniGetConfig;
    if (!cocos2d::JniHelper::getStaticMethodInfo(jniGetConfig,
                                                 "cn/thinkingdata/analytics/TDConfig",
                                                 "getInstance",
                                                 "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcn/thinkingdata/analytics/TDConfig;")) {
        return;
    }
    jstring _appId = jniGetConfig.env->NewStringUTF(config.getAppId().c_str());
    jstring _server = jniGetConfig.env->NewStringUTF(config.getServer().c_str());
    jstring _name = jniGetConfig.env->NewStringUTF(config.getName().c_str());
    jobject _config = (jobject) jniGetConfig.env->CallStaticObjectMethod(
            jniGetConfig.classID, jniGetConfig.methodID,_context,_appId,_server,_name);
    releaseMethod(jniGetConfig);

    JniMethodInfo methodInfo1;
    if(JniHelper::getStaticMethodInfo(methodInfo1,THINKING_JAVA_CLASS,"sharedInstance","(Lcn/thinkingdata/analytics/TDConfig;IIILjava/lang/String;)Lcn/thinkingdata/analytics/ThinkingAnalyticsSDK;"))
    {
        int mode = 0;
        if(config.getModel() == TD_DEBUG) {
            mode = 1;
        }
        else if(config.getModel() == TD_DEBUG_ONLY) {
            mode = 2;
        }
        int enableEncrypt = config.getEnableEncrypt();

        int version = config.getSecretKey().version;
        jstring keyy = methodInfo.env->NewStringUTF(config.getSecretKey().publicKey.c_str());
        methodInfo1.env->CallStaticObjectMethod(methodInfo1.classID, methodInfo1.methodID,_config, mode, enableEncrypt, version, keyy);
        methodInfo.env->DeleteLocalRef(keyy);
    }
    methodInfo1.env->DeleteLocalRef(_context);
    methodInfo1.env->DeleteLocalRef(_appId);
    methodInfo1.env->DeleteLocalRef(_config);
    methodInfo1.env->DeleteLocalRef(_name);
    releaseMethod(methodInfo1);
}

void TDAnalytics::init(string appId, string server) {
    TDConfig config = TDConfig(appId,server);
    init(config);
}

void TDAnalytics::setTrackStatus(TDTrackStatus status,string appid)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"setTrackStatus","(ILjava/lang/String;)V"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appid.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)status,jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void TDAnalytics::enableThirdPartySharing(TDThirdPartyType type, string appId)
{
    TDAnalytics::enableThirdPartySharing(type, TDJSONObject(), appId);
}

void TDAnalytics::enableThirdPartySharing(TDThirdPartyType type, const TDJSONObject &properties, string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableThirdPartySharing","(Lorg/json/JSONObject;ILjava/lang/String;)V"))
    {
        jobject jParam = createJavaJsonObject(methodInfo.env, &properties);
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jParam, type, jAppId);
        methodInfo.env->DeleteLocalRef(jAppId);
        methodInfo.env->DeleteLocalRef(jParam);
        releaseMethod(methodInfo);
    }
}





#endif
