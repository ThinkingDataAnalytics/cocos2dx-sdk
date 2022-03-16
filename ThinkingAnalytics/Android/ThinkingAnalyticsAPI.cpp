#include "../Common/ThinkingAnalyticsAPI.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"


using namespace thinkingdata;
USING_NS_CC;
#define THINKING_JAVA_CLASS "cn/thinkingdata/android/ThinkingAnalyticsCocosAPI"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C"{
// Jni 全局实例
//static JniMethodInfo sMethodInfo;
//static bool isMethodExist(const char *methodName, const char *methodDescription) {
//    if (JniHelper::getStaticMethodInfo(sMethodInfo,THINKING_JAVA_CLASS,methodName,methodDescription)) {
//        return true;
//    }
//    return false;
//}
/**
 * 将 jstring 转化为 string
 * @param env env
 * @param jString jString
 * @return
 */
static string jStringToString(JNIEnv *env, jstring jString) {
    if (jString == NULL || env == NULL) {
        return "";
    }
    const char *chars = env->GetStringUTFChars(jString, NULL);
    string ret(chars);
    env->ReleaseStringUTFChars(jString, chars);
    return ret;
}

// java jsonobject 转化为C++ TDJSONObject
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
            jmethodID  _getobj = env->GetMethodID(_jsonArrayClass,"getJSONObject","(I)Lorg/json/JSONObject;");

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



/**
 * TDJSONObject 转化为 JSONObjec 对象
 * @param env env
 * @param properties TDJSONObject
 * @return 返回 JSONObject 对象
 */
static jobject createJavaJsonObject(JNIEnv *env, const thinkingdata::TDJSONObject *properties) {
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


TDJSONObject getDynamicProperties(string appId)
{
    TDJSONObject jsonObject;
    if(!dynamicPropertiesMap.empty())
    {
        string currentAppId = ThinkingAnalyticsAPI::currentAppId(appId);
        int count = dynamicPropertiesMap.count(currentAppId);
        if(count > 0)
        {
            GetDynamicSuperProperties  getDynamicSuperProperties = dynamicPropertiesMap[currentAppId];
            jsonObject = getDynamicSuperProperties();
        }
    }
    return jsonObject;
}


static void trackEvent(ThinkingAnalyticsEvent* event,int type,string appId)
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



/**
 *事件上报接口
 */
void ThinkingAnalyticsAPI::track(string eventName,string appId)
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
void ThinkingAnalyticsAPI::track(string eventName,const TDJSONObject& node,string appId)
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

void ThinkingAnalyticsAPI::track(TDFirstEvent* firstEvent,string appId) {

    trackEvent(firstEvent,1,appId);

}
void ThinkingAnalyticsAPI::track(TDUpdatableEvent* updatableEvent,string appId)
{
    trackEvent(updatableEvent,2,appId);

}
void ThinkingAnalyticsAPI::track(TDOverWritableEvent* overWritableEvent,string appId) {
    trackEvent(overWritableEvent,3,appId);
}

void ThinkingAnalyticsAPI::timeEvent(string eventName,string appId)
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







void ThinkingAnalyticsAPI::login(string accountID,string appId)
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

void ThinkingAnalyticsAPI::logout(string appId)
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
void ThinkingAnalyticsAPI::identify(string distinctId,string appId)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"identify","(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring distinct = methodInfo.env->NewStringUTF(distinctId.c_str());
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,distinct,jAppId);
        methodInfo.env->DeleteLocalRef(distinct);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
}

void ThinkingAnalyticsAPI::user_set(const TDJSONObject &properties,string appId)
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
void ThinkingAnalyticsAPI::user_setOnce(const TDJSONObject &properties,string appId)
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
void ThinkingAnalyticsAPI::user_append(const TDJSONObject &properties,string appId)
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
void ThinkingAnalyticsAPI::user_add(const TDJSONObject &properties,string appId)
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
void ThinkingAnalyticsAPI::user_delete(string appId)
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
void ThinkingAnalyticsAPI::user_unset(string propertyName,string appId)
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


void ThinkingAnalyticsAPI::enableAutoTrack(string appId)
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
void ThinkingAnalyticsAPI::setSuperProperties(const TDJSONObject &properties,string appId)
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
void ThinkingAnalyticsAPI::clearSuperProperties(string appId)
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
void ThinkingAnalyticsAPI::unsetSuperProperty(string superPropertyName,string appId)
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

void ThinkingAnalyticsAPI::flush(string appId)
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
void ThinkingAnalyticsAPI::optOutTrackingAndDeleteUser(string appId)
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
void ThinkingAnalyticsAPI::optOutTracking(string appId)
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
void ThinkingAnalyticsAPI::optInTracking(string appId)
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
void ThinkingAnalyticsAPI::enableTracking(bool enabled,string appId)
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
void ThinkingAnalyticsAPI::enableTrackLog(bool enabled)
{
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"enableTrackLog","(Z)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,enabled);
        releaseMethod(methodInfo);
    }
}
string ThinkingAnalyticsAPI::getDeviceId()
{
    string deviceID;
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"getDeviceId","()Ljava/lang/String;"))
    {
        jstring result = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        deviceID = jStringToString(methodInfo.env,result);
        methodInfo.env->DeleteLocalRef(result);
        releaseMethod(methodInfo);
    }
    return deviceID;
}
string ThinkingAnalyticsAPI::getDistinctId(string appId)
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
PresetProperties* ThinkingAnalyticsAPI::getPresetProperties(string appId)
{
    PresetProperties *presetProperties = new PresetProperties();
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"getPresetProperties","(Ljava/lang/String;)Lcn/thinkingdata/android/TDPresetProperties;"))
    {
        jstring jAppId = methodInfo.env->NewStringUTF(appId.c_str());

        jobject obj = (jobject)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,jAppId);
        jclass clazz = methodInfo.env->GetObjectClass(obj);

        jfieldID screenWidthField = methodInfo.env->GetFieldID(clazz, "screen_width", "I");
        jint screenWidth = methodInfo.env->GetIntField(obj,screenWidthField);
        presetProperties->screenWidth = screenWidth;

        jfieldID screenHeightField = methodInfo.env->GetFieldID(clazz, "screen_height", "I");
        jint screenHeight = methodInfo.env->GetIntField(obj,screenHeightField);
        presetProperties->screenHeight = screenHeight;

        jfieldID bundleIdField = methodInfo.env->GetFieldID(clazz, "bundle_id", "Ljava/lang/String;");
        jstring bundleId = (jstring)methodInfo.env->GetObjectField(obj,bundleIdField);
        presetProperties->bundleId = jStringToString(methodInfo.env,bundleId);;

        jfieldID carrierField = methodInfo.env->GetFieldID(clazz, "carrier", "Ljava/lang/String;");
        jstring carrier = (jstring)methodInfo.env->GetObjectField(obj,carrierField);
        presetProperties->carrier = jStringToString(methodInfo.env,carrier);

        jfieldID deviceIdField = methodInfo.env->GetFieldID(clazz, "device_id", "Ljava/lang/String;");
        jstring deviceId = (jstring)methodInfo.env->GetObjectField(obj,deviceIdField);
        presetProperties->deviceId = jStringToString(methodInfo.env,deviceId);

        jfieldID deviceModelField = methodInfo.env->GetFieldID(clazz, "device_model", "Ljava/lang/String;");
        jstring deviceModel = (jstring)methodInfo.env->GetObjectField(obj,deviceModelField);
        presetProperties->deviceModel = jStringToString(methodInfo.env,deviceModel);

        jfieldID manufactureField = methodInfo.env->GetFieldID(clazz, "manufacture", "Ljava/lang/String;");
        jstring manufacture = (jstring)methodInfo.env->GetObjectField(obj,manufactureField);
        presetProperties->manufacturer = jStringToString(methodInfo.env,manufacture);

        jfieldID networkTypeField = methodInfo.env->GetFieldID(clazz, "network_type", "Ljava/lang/String;");
        jstring networkType = (jstring)methodInfo.env->GetObjectField(obj,networkTypeField);
        presetProperties->networkType = jStringToString(methodInfo.env,networkType);

        jfieldID osField = methodInfo.env->GetFieldID(clazz, "os", "Ljava/lang/String;");
        jstring os = (jstring)methodInfo.env->GetObjectField(obj,osField);
        presetProperties->os = jStringToString(methodInfo.env,os);


        jfieldID osVersionField = methodInfo.env->GetFieldID(clazz, "os_version", "Ljava/lang/String;");
        jstring osVersion = (jstring)methodInfo.env->GetObjectField(obj,osVersionField);
        presetProperties->osVersion = jStringToString(methodInfo.env,osVersion);


        jfieldID systemLanguageField = methodInfo.env->GetFieldID(clazz, "system_language", "Ljava/lang/String;");
        jstring systemLanguage = (jstring)methodInfo.env->GetObjectField(obj,systemLanguageField);
        presetProperties->systemLanguage = jStringToString(methodInfo.env,systemLanguage);


        jfieldID zoneOffsetField = methodInfo.env->GetFieldID(clazz, "zone_offset", "D");
        jdouble zoneOffset = methodInfo.env->GetDoubleField(obj,zoneOffsetField);
        presetProperties->zoneOffset = zoneOffset;

        methodInfo.env->DeleteLocalRef(obj);
        methodInfo.env->DeleteLocalRef(jAppId);
        releaseMethod(methodInfo);
    }
    return presetProperties;
}
void ThinkingAnalyticsAPI::calibrateTime(long long timestamp) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"calibrateTime","(J)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,timestamp);
        releaseMethod(methodInfo);
    }
}

void ThinkingAnalyticsAPI::calibrateTimeWithNtp(string ntpServer) {
    JniMethodInfo methodInfo;
    if(JniHelper::getStaticMethodInfo(methodInfo,THINKING_JAVA_CLASS,"calibrateTimeWithNtp","(Ljava/lang/String;)V"))
    {
        jstring _server = methodInfo.env->NewStringUTF(ntpServer.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,_server);
        methodInfo.env->DeleteLocalRef(_server);
        releaseMethod(methodInfo);
    }
}


TDJSONObject ThinkingAnalyticsAPI::getSuperProperties(string appId) {
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

string ThinkingAnalyticsAPI::currentAppId(string appId) {
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

void ThinkingAnalyticsAPI::setDynamicSuperProperties(GetDynamicSuperProperties getDynamicSuperProperties, string appId) {

    if(ThinkingAnalyticsAPI::currentAppId(appId).size() != 0)
    {
        string currentAppId = ThinkingAnalyticsAPI::currentAppId(appId);
        dynamicPropertiesMap[currentAppId] = getDynamicSuperProperties;
    }

}
string ThinkingAnalyticsAPI::createLightInstance(string appId) {
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

void ThinkingAnalyticsAPI::init(Config config) {
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
                                                 "cn/thinkingdata/android/TDConfig",
                                                 "getInstance",
                                                 "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcn/thinkingdata/android/TDConfig;")) {
        return;
    }
    jstring _appId = jniGetConfig.env->NewStringUTF(config.getAppId().c_str());
    jstring _server = jniGetConfig.env->NewStringUTF(config.getServer().c_str());
    jstring _name = jniGetConfig.env->NewStringUTF(config.getName().c_str());
    jobject _config = (jobject) jniGetConfig.env->CallStaticObjectMethod(
            jniGetConfig.classID, jniGetConfig.methodID,_context,_appId,_server,_name);
    if(config.getModel() != TD_NORMAL)
    {
        jclass clazz = jniGetConfig.env->GetObjectClass(_config);
        jmethodID _setDebugModel = jniGetConfig.env->GetMethodID(clazz,"setModeInt","(I)V");
        if(config.getModel() == TD_DEBUG)
        {
            jniGetConfig.env->CallVoidMethod(_config,_setDebugModel,1);
        }else if(config.getModel() == TD_DEBUG_ONLY)
        {
            jniGetConfig.env->CallVoidMethod(_config,_setDebugModel,2);
        }
        jniGetConfig.env->DeleteLocalRef(clazz);
    }
    releaseMethod(jniGetConfig);

    JniMethodInfo methodInfo1;
    if(JniHelper::getStaticMethodInfo(methodInfo1,THINKING_JAVA_CLASS,"sharedInstance","(Lcn/thinkingdata/android/TDConfig;)Lcn/thinkingdata/android/ThinkingAnalyticsSDK;"))
    {
        methodInfo1.env->CallStaticObjectMethod(methodInfo1.classID, methodInfo1.methodID,_config);
    }
    methodInfo1.env->DeleteLocalRef(_context);
    methodInfo1.env->DeleteLocalRef(_appId);
    methodInfo1.env->DeleteLocalRef(_config);
    methodInfo1.env->DeleteLocalRef(_name);
    releaseMethod(methodInfo1);
}

void ThinkingAnalyticsAPI::init(string appId, string server) {
    Config config = Config(appId,server);
    init(config);
}

#endif
