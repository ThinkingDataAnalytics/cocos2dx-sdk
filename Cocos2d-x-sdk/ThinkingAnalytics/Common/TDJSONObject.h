#include <stdio.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace thinkingdata {
    namespace analytics {

    enum TDThirdPartyType {
        TDThirdPartyTypeNONE               = 0,
        TDThirdPartyTypeAPPSFLYER          = 1 << 0,
        TDThirdPartyTypeIRONSOURCE         = 1 << 1,
        TDThirdPartyTypeADJUST             = 1 << 2,
        TDThirdPartyTypeBRANCH             = 1 << 3,
        TDThirdPartyTypeTOPON              = 1 << 4,
        TDThirdPartyTypeTRACKING           = 1 << 5,
        TDThirdPartyTypeTRADPLUS           = 1 << 6,
        TDThirdPartyTypeAPPLOVIN           = 1 << 7,
        TDThirdPartyTypeKOCHAVA            = 1 << 8,
        TDThirdPartyTypeTALKINGDATA        = 1 << 9,
        TDThirdPartyTypeFIREBASE           = 1 << 10,
    };

    enum TDTrackStatus {
        TDTrackStatusPause = 0,
        TDTrackStatusStop  = 1,
        TDTrackStatusSaveOnly = 2,
        TDTrackStatusNormal = 3,
    };

    enum TDAutoTrackEventType {
        TDAutoTrackEventTypeNone     = 0,
        TDAutoTrackEventTypeStart    = 1 << 0,
        TDAutoTrackEventTypeEnd      = 1 << 1,
        TDAutoTrackEventTypeCrash    = 1 << 2,
        TDAutoTrackEventTypeInstall  = 1 << 3,
    };

    enum TDSSLPinningMode {
        TDSSLPinningModeNone          = 0,
        TDSSLPinningModePublicKey     = 1 << 0,
        TDSSLPinningModeCertificate   = 1 << 1
    };

    /**
     * @brief JSON Object
     * 
     */
    class TDJSONObject {
    public:
        
        void setNumber(const char *propertyName, int32_t value);
        
        void setNumber(const char *propertyName, int64_t value);
        
        void setNumber(const char *propertyName, double value);
        
        void setString(const char *propertyName, const char *value);
        
        void setBool(const char *propertyName, bool value);
        
        void setList(const char *propertyName, const std::vector<string> &value);
        
        void setDateTime(const char *propertyName, time_t seconds, int milliseconds);
        
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

    /**
     * @brief Custom Event Model
     * 
     */
    class TDEventModel
    {
    public:
        EventType mType;
        string mEventName;
        TDJSONObject mProperties;
        string mExtraId;
        TDEventModel(string eventName,TDJSONObject properties);
    };
    /**
     * @brief First Event Model
     * 
     */
    class TDFirstEventModel: public TDEventModel
    {
    public:
        TDFirstEventModel(string eventName,TDJSONObject properties);
        void setFirstCheckId(string firstCheckId);
        
    };
    /**
     * @brief Updatable Event Mode
     * 
     */
    class TDUpdatableEventModel: public TDEventModel
    {
    public:
        TDUpdatableEventModel(string eventName,TDJSONObject properties,string eventId);
    };
    /**
     * @brief Overwritable Event Model
     * 
     */
    class TDOverwritableEventModel: public TDEventModel
    {
    public:
        TDOverwritableEventModel(string eventName,TDJSONObject properties,string eventId);
    };

    /**
     * @brief Preset Properties
     * 
     */
    class TDPresetProperties
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
        string libName;
        string libVersion;
        
        string appVersion;
        int    fps;
        string disk;
        string ram;
        bool   isSimulator;
        
        TDJSONObject* toEventPresetProperties();
    };

    enum  TDMode{
        TD_NORMAL = 0,
        TD_DEBUG,
        TD_DEBUG_ONLY
    };

    /**
     * @brief Data Encryption Key
     * 
     */
    class TDSecretKey
    {
    public:
        int    version;
        string publicKey;
        TDSecretKey(int version = 0, string publicKey = "");
        ~TDSecretKey();
    };
   
    /**
     * @brief SDK Config Information
     * 
     */
    class TDConfig
    {
    private:
        string appId;
        string server;
        string name;
        TDMode  model;
        TDSecretKey secretKey;
        bool enableEncrypt;
        TDSSLPinningMode pinningMode;
    public:
        TDConfig(string appId,string server);
        void setModel(TDMode model);
        void setName(string name);
        string getName();
        string getAppId();
        string getServer();
        TDMode  getModel();
        void setEnableEncrypt(bool enable);
        void setSecretKey(TDSecretKey secretKey);
        void setPinningMode(TDSSLPinningMode pinningMode);
        bool getEnableEncrypt();
        TDSSLPinningMode getPinningMode();
        TDSecretKey getSecretKey();
        ~TDConfig();
    };
    }
}
