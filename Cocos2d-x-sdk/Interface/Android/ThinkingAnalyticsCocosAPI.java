package cn.thinkingdata.android;

import android.app.Activity;
import android.app.Dialog;
import android.app.Fragment;
import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TimeZone;
import java.util.UUID;

import cn.thinkingdata.android.encrypt.TDSecreteKey;
import cn.thinkingdata.android.utils.TDLog;

public class ThinkingAnalyticsCocosAPI {
    private static final String TAG = "ThinkingAnalyticsAPI";
    static Map<String, ThinkingAnalyticsSDK> sInstances = new HashMap<>();
    static List<String> sAppIds = new ArrayList<>();

    public static void setCustomerLibInfo(String libName, String libVersion) {
        ThinkingAnalyticsSDK.setCustomerLibInfo(libName, libVersion);
    }

    public static ThinkingAnalyticsSDK sharedInstance(Context context, String appId, String url) {
        TDConfig config = TDConfig.getInstance(context, appId, url);
        return ThinkingAnalyticsSDK.sharedInstance(config);
    }

    public static ThinkingAnalyticsSDK sharedInstance(TDConfig config) {
        String instanceName = TextUtils.isEmpty(config.getName()) ? config.mToken : config.getName();
        ThinkingAnalyticsSDK instance = sInstances.get(instanceName);
        if (instance == null) {
            instance = ThinkingAnalyticsSDK.sharedInstance(config);
            sInstances.put(instanceName, instance);
            sAppIds.add(instanceName);
        }
        return ThinkingAnalyticsSDK.sharedInstance(config);
    }

    public static ThinkingAnalyticsSDK sharedInstance(TDConfig config, int enableEncrypt, int version, String publicKey) {
        if (enableEncrypt == 1) {
            config.enableEncrypt(true);
            TDSecreteKey secreteKey = new TDSecreteKey();
            secreteKey.publicKey = publicKey;
            secreteKey.version = version;
            secreteKey.symmetricEncryption = "AES";
            secreteKey.asymmetricEncryption = "RSA";
            config.setSecretKey(secreteKey);
        }

        String instanceName = TextUtils.isEmpty(config.getName()) ? config.mToken : config.getName();
        ThinkingAnalyticsSDK instance = (ThinkingAnalyticsSDK)sInstances.get(instanceName);
        if (instance == null) {
            instance = ThinkingAnalyticsSDK.sharedInstance(config);
            sInstances.put(instanceName, instance);
            sAppIds.add(instanceName);
        }

        return ThinkingAnalyticsSDK.sharedInstance(config);
    }

    public static String createLightInstance(String appId)
    {
       ThinkingAnalyticsSDK instance =  currentInstance(appId).createLightInstance();
       String token =  UUID.randomUUID().toString();
       sInstances.put(token,instance);
       return token;
    }
    public static String currentToken(String appId)
    {
        String token;
        if(TextUtils.isEmpty(appId) && sAppIds.size() > 0)
        {
            token = sAppIds.get(0);
        }else
        {
            token = appId;
        }
        return token;
    }
    public static  ThinkingAnalyticsSDK currentInstance(String appId)
    {
       String token = currentToken(appId);
       if(sInstances.containsKey(token))
       {
           return sInstances.get(token);
       }
       TDLog.d(TAG,"Instance does not exist");
       return null;
    }

    public static  void track(String eventName,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).track(eventName);
        }
    }


    public static void track(String eventName, JSONObject properties,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).track(eventName,properties);
        }
    }


    public static  void track(String eventName, JSONObject properties, Date time,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).track(eventName,properties,time);
        }
    }


    public static  void track(String eventName, JSONObject properties, Date time, TimeZone timeZone,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).track(eventName, properties, time,timeZone);
        }
    }


    public static  void track(ThinkingAnalyticsEvent event,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).track(event);
        }
    }

    public static  void track(String eventName,JSONObject properties,String extraId,int type,String appId)
    {
        ThinkingAnalyticsEvent event = null;
        switch (type)
        {
            case 1:
            {
                event = new TDFirstEvent(eventName,properties);
                if(extraId != null && extraId.length() != 0)
                {
                    ((TDFirstEvent)event).setFirstCheckId(extraId);
                }
            }
            break;
            case 2:
            {
                event = new TDUpdatableEvent(eventName,properties,extraId);
            }
            break;
            case 3:
            {
                event = new TDOverWritableEvent(eventName,properties,extraId);
            }
            break;
        }
        if(event != null)
        {
            track(event,appId);
        }
    }


    public static  void timeEvent(String eventName,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).timeEvent(eventName);
        }
    }


    public static  void login(String loginId,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).login(loginId);
        }
    }


    public static  void logout(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).logout();
        }
    }


    public static  void identify(String identify,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).identify(identify);
        }
    }


    public static  void user_set(JSONObject property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_set(property);
        }
    }


    public static  void user_setOnce(JSONObject property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_setOnce(property);
        }

    }


    public static  void user_add(JSONObject property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_add(property);
        }
    }


    public static  void user_append(JSONObject property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_append(property);
        }
    }

    public static  void user_uniqAppend(JSONObject property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_uniqAppend(property);
        }
    }

    public static  void user_add(String propertyName, Number propertyValue,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_add(propertyName,propertyValue);
        }
    }


    public static  void user_delete(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_delete();
        }
    }

    public static  void user_unset(String property,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).user_unset(property);
        }
    }


    public static  void setSuperProperties(JSONObject superProperties,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).setSuperProperties(superProperties);
        }
    }


    public static  void unsetSuperProperty(String superPropertyName,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).unsetSuperProperty(superPropertyName);
        }
    }


    public static  void clearSuperProperties(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).clearSuperProperties();
        }
    }


    public static String getDistinctId(String appId) {
        if(currentInstance(appId) != null)
        {
            return currentInstance(appId).getDistinctId();
        }
        return  "";
    }


    public static JSONObject getSuperProperties(String appId) {
        if(currentInstance(appId) != null)
        {
            return currentInstance(appId).getSuperProperties();
        }
        return new JSONObject();
    }

    public static TDPresetProperties getPresetProperties(String appId){
        if(currentInstance(appId) != null)
        {
            return  currentInstance(appId).getPresetProperties();
        }
        return  new TDPresetProperties();
    }

    public static  void enableAutoTrack(String appId) {
        List<ThinkingAnalyticsSDK.AutoTrackEventType> typeList = new ArrayList<>();
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_START);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_INSTALL);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_END);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_CRASH);
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).enableAutoTrack(typeList);
        }
    }

    public static void enableAutoTrack(String appId, JSONObject properties) {
        List<ThinkingAnalyticsSDK.AutoTrackEventType> typeList = new ArrayList();
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_START);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_INSTALL);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_END);
        typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_CRASH);
        if (currentInstance(appId) != null) {
            currentInstance(appId).enableAutoTrack(typeList, properties);
        }

    }

    public static void enableAutoTrack(String appId, int type, JSONObject properties) {
        List<ThinkingAnalyticsSDK.AutoTrackEventType> typeList = new ArrayList();
        if ((type & 1) > 0) {
            typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_START);
        }

        if ((type & 2) > 0) {
            typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_END);
        }

        if ((type & 4) > 0) {
            typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_CRASH);
        }

        if ((type & 8) > 0) {
            typeList.add(ThinkingAnalyticsSDK.AutoTrackEventType.APP_INSTALL);
        }

        if (currentInstance(appId) != null) {
            currentInstance(appId).enableAutoTrack(typeList, properties);
        }

    }

    public static  void flush(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).flush();
        }
    }

    public static  String getDeviceId() {
        if(currentInstance("") != null)
        {
            return currentInstance("").getDeviceId();
        }
        return "";
    }

    public static  void enableTracking(boolean enabled,String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).enableTracking(enabled);
        }
    }
    public static  void enableTrackLog(boolean enabled)
    {
        ThinkingAnalyticsSDK.enableTrackLog(enabled);
    }


    public static  void optOutTrackingAndDeleteUser(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).optOutTrackingAndDeleteUser();
        }
    }

    public static  void optOutTracking(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).optOutTracking();
        }
    }


    public static  void optInTracking(String appId) {
        if(currentInstance(appId) != null)
        {
            currentInstance(appId).optInTracking();
        }

    }

    public static void calibrateTimeWithNtp(String ntpServer)
    {
        ThinkingAnalyticsSDK.calibrateTimeWithNtp(ntpServer);
    }

    public static void calibrateTime(long timestamp)
    {
        ThinkingAnalyticsSDK.calibrateTime(timestamp);
    }

    public static void setTrackStatus(int status, String appId) {
        if (currentInstance(appId) != null) {
            if (status == 0) {
                currentInstance(appId).setTrackStatus(ThinkingAnalyticsSDK.TATrackStatus.PAUSE);
            } else if (status == 1) {
                currentInstance(appId).setTrackStatus(ThinkingAnalyticsSDK.TATrackStatus.STOP);
            } else if (status == 2) {
                currentInstance(appId).setTrackStatus(ThinkingAnalyticsSDK.TATrackStatus.SAVE_ONLY);
            } else if (status == 3) {
                currentInstance(appId).setTrackStatus(ThinkingAnalyticsSDK.TATrackStatus.NORMAL);
            }
        }

    }

    public static List<Map<String, Object>> jsonArr2Map(JSONArray jsonArr) throws Exception {
        List<Map<String, Object>> list = new ArrayList();

        for(int j = 0; j < jsonArr.length(); ++j) {
            if (jsonArr.get(j) instanceof JSONObject) {
                list.add(jsonObject2Map((JSONObject)jsonArr.get(j)));
            }
        }

        return list;
    }

    public static Map<String, Object> jsonObject2Map(JSONObject jsonObject) throws Exception {
        Map<String, Object> map = new HashMap();
        Iterator i = jsonObject.keys();

        while(i.hasNext()) {
            String key = (String)i.next();
            Object value = jsonObject.getString(key);
            if (value instanceof JSONObject) {
                map.put(key, jsonObject2Map((JSONObject)value));
            } else if (value instanceof JSONArray) {
                map.put(key, jsonArr2Map((JSONArray)value));
            } else {
                map.put(key, value);
            }
        }

        return map;
    }

    public static void enableThirdPartySharing(JSONObject properties, int type, String appId) {
        if (currentInstance(appId) != null) {
            try {
                currentInstance(appId).enableThirdPartySharing(type, jsonObject2Map(properties));
            } catch (Exception var4) {
                var4.printStackTrace();
            }
        }

    }
}