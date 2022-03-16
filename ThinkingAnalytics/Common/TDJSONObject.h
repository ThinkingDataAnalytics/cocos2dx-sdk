#include <stdio.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace thinkingdata {

    class TDJSONObject {
    public:
        
        void setNumber(const char *propertyName, int32_t value);

        void setNumber(const char *propertyName, int64_t value);

        void setNumber(const char *propertyName, double value);

        void setString(const char *propertyName, const char *value);

        void setBool(const char *propertyName, bool value);

        void setList(const char *propertyName, const std::vector<string> &value);
        
        void setDateTime(const char *propertyName, time_t seconds, int milliseconds);

        /**
         * 设置时间为事件属性
         * @param property_name 属性名
         * @param value 时间字符串，格式需要是：2020-12-31 16:30:27.567
         */
        void setDateTime(const char *propertyName, const char *value);

        void clear();

        static string toJson(const TDJSONObject &node);
        
        static TDJSONObject toObject(const char *string);

        void mergeFrom(const TDJSONObject &anotherNode);
        
        void removeKey(const char *propertyName);

        void setList(const char *propertyName, const std::vector<TDJSONObject> &value);
        
        void setJsonObject(const char *propertyName, const TDJSONObject &node);

        class ValueNode;

        std::map<string, ValueNode> propertiesMap;
        
        enum ValueNodeType {
            NUMBER,
            INT,
            STRING,
            LIST,
            DATETIME,
            BOOL1,
            OBJECT,
            OBJECTS,
            UNKNOWN,
        };

    private:
        static void dumpNode(const TDJSONObject &node, string *buffer);
    
    };

    class TDJSONObject::ValueNode {
    public:
        ValueNode() : nodeType(UNKNOWN) {}

        explicit ValueNode(double value);

        explicit ValueNode(int64_t value);

        explicit ValueNode(const string &value);

        explicit ValueNode(bool value);

        explicit ValueNode(const TDJSONObject &value);
        
        explicit ValueNode(const std::vector<TDJSONObject> &value);

        explicit ValueNode(const std::vector<string> &value);

        ValueNode(time_t seconds, int milliseconds);

        static void toStr(const ValueNode &node, string *buffer);

    private:
        
        static void dumpString(const string &value, string *buffer);

        static void dumpList(const std::vector<string> &value, string *buffer);

        static void dumpDateTime(const time_t &seconds, int milliseconds, string *buffer);

        static void dumpNumber(double value, string *buffer);

        static void dumpNumber(int64_t value, string *buffer);
        
        static void dumpList(const std::vector<TDJSONObject> &value, string *buffer);
        
        static void dumpObject(TDJSONObject value, string *buffer);

        ValueNodeType nodeType;

        union UnionValue {
            double numberValue;
            bool boolValue;
            struct {
                time_t seconds;
                int milliseconds;
            } datetimeValue;
            int64_t intValue;

            UnionValue() { memset(this, 0, sizeof(UnionValue)); }
        } valueData;

        string stringData;
        std::vector<string> listData;
        std::vector<TDJSONObject> objectDatas;
        TDJSONObject objectData;
    };
    enum EventType {
        FIRST=1,
        UPDATABLE=2,
        OVERWRITABLE=3
    };


    class ThinkingAnalyticsEvent
    {
    public:
        EventType mType;
        string mEventName;
        TDJSONObject mProperties;
        string mExtraId;
        ThinkingAnalyticsEvent(string eventName,TDJSONObject properties);
    };
    class TDFirstEvent: public ThinkingAnalyticsEvent
    {
    public:
        TDFirstEvent(string eventName,TDJSONObject properties);
        void setFirstCheckId(string firstCheckId);

    };
    class TDUpdatableEvent: public ThinkingAnalyticsEvent
    {
    public:
        TDUpdatableEvent(string eventName,TDJSONObject properties,string eventId);
    };
    class TDOverWritableEvent: public ThinkingAnalyticsEvent
    {
    public:
        TDOverWritableEvent(string eventName,TDJSONObject properties,string eventId);
    };
    
    class PresetProperties
    {
    public:
    
        string bundleId;
        string carrier;
        string deviceId;
        string deviceModel;
        string manufacturer;
        string networkType;
        string os;
        string osVersion;
        int    screenHeight;
        int    screenWidth;
        string systemLanguage;
        double zoneOffset;
        string appVersion;
        string libName;
        string libVersion;

        TDJSONObject* toEventPresetProperties();
    };

   enum  Model{
       TD_NORMAL = 0,
       TD_DEBUG,
       TD_DEBUG_ONLY
   };

   class Config
   {
   private:
       string appId;
       string server;
       string name;
       Model  model;
   public:
       Config(string appId,string server);
       void setModel(Model model);
       void setName(string name);
       string getName();
       string getAppId();
       string getServer();
       Model  getModel();
       ~Config();
   };
}
