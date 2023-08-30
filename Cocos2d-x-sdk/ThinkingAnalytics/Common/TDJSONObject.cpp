#include "TDJSONObject.h"
#include <sstream>
#include <iostream>

using namespace thinkingdata::analytics;

void TDJSONObject::removeKey(const char *propertyName)
{
    if (!propertyName) return;
    std::map<string, ValueNode>::iterator it;
    it = propertiesMap.find(propertyName);
    if (it != propertiesMap.end()) {
        propertiesMap.erase(it);
    }
}

void TDJSONObject::setJsonObject(const char *propertyName, const TDJSONObject & node) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(node);
}

void TDJSONObject::setNumber(const char *propertyName, double value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(value);
}

void TDJSONObject::setNumber(const char *propertyName, int32_t value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(static_cast<int64_t>(value));
}

void TDJSONObject::setNumber(const char *propertyName, int64_t value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(value);
}

void TDJSONObject::setString(const char *propertyName, const char *value) {
    if (!propertyName || !value) return;
    propertiesMap[string(propertyName)] = ValueNode(string(value));
}

void TDJSONObject::setBool(const char *propertyName, bool value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(value);
}

void TDJSONObject::setList(const char *propertyName, const std::vector<string> &value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(value);
}

void TDJSONObject::setList(const char *propertyName, const std::vector<TDJSONObject> &value) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(value);
}

void TDJSONObject::setDateTime(const char *propertyName, const time_t seconds, int milliseconds) {
    if (!propertyName) return;
    propertiesMap[string(propertyName)] = ValueNode(seconds, milliseconds);
}

void TDJSONObject::setDateTime(const char *propertyName, const char *value) {
    if (!propertyName || !value) return;
    propertiesMap[string(propertyName)] = ValueNode(string(value));
}

void TDJSONObject::clear() {
    propertiesMap.clear();
}

void TDJSONObject::dumpNode(const TDJSONObject &node, string *buffer) {
    *buffer += '{';
    bool first = true;

    for (std::map<string, ValueNode>::const_iterator iterator = node.propertiesMap.begin(); iterator != node.propertiesMap.end(); ++iterator) {
        if (first) {
            first = false;
        } else {
            *buffer += ',';
        }
        *buffer += '"' + iterator->first + "\":";
        ValueNode::toStr(iterator->second, buffer);
    }
    *buffer += '}';
}

void TDJSONObject::ValueNode::dumpString(const string &value, string *buffer) {
    *buffer += '"';
    for (std::string::size_type i = 0; i < value.length(); ++i) {
        char c = value[i];
        switch (c) {
            case '"':
                *buffer += "\\\"";
                break;
            case '\\':
                *buffer += "\\\\";
                break;
            case '\b':
                *buffer += "\\b";
                break;
            case '\f':
                *buffer += "\\f";
                break;
            case '\n':
                *buffer += "\\n";
                break;
            case '\r':
                *buffer += "\\r";
                break;
            case '\t':
                *buffer += "\\t";
                break;
            default:
                *buffer += c;
                break;
        }
    }
    *buffer += '"';
}

void TDJSONObject::ValueNode::dumpList(const std::vector<string> &value, string *buffer) {
    *buffer += '[';
    bool first = true;
    for (std::vector<string>::const_iterator iterator = value.begin(); iterator != value.end(); ++iterator) {
        if (first) {
            first = false;
        } else {
            *buffer += ',';
        }
        dumpString(*iterator, buffer);
    }
    *buffer += ']';
}

void TDJSONObject::ValueNode::dumpList(const std::vector<TDJSONObject> &value, string *buffer) {
    *buffer += '[';
    bool first = true;
    for (std::vector<TDJSONObject>::const_iterator iterator = value.begin(); iterator != value.end(); ++iterator) {
        if (first) {
            first = false;
        } else {
            *buffer += ',';
        }
        dumpNode(*iterator, buffer);
    }
    *buffer += ']';
}


#if defined(__linux__)
#define TD_SDK_LOCALTIME(seconds, now) localtime_r((seconds), (now))
#elif defined(__APPLE__)
#define TD_SDK_LOCALTIME(seconds, now) localtime_r((seconds), (now))
#elif defined(_WIN32)
#define TD_SDK_LOCALTIME(seconds, now) localtime_s((now), (seconds))
#define snprintf sprintf_s
#endif

void TDJSONObject::ValueNode::dumpDateTime(const time_t &seconds, int milliseconds, string *buffer) {
    struct tm tm = {};
    TD_SDK_LOCALTIME(&seconds, &tm);
    char buff[64];
    snprintf(buff, sizeof(buff), "\"%04d-%02d-%02d %02d:%02d:%02d.%03d\"",
             tm.tm_year + 1900,
             tm.tm_mon + 1,
             tm.tm_mday,
             tm.tm_hour,
             tm.tm_min,
             tm.tm_sec,
             milliseconds);
    *buffer += buff;
}

string TDJSONObject::toJson(const TDJSONObject &node) {
    string buffer;
    dumpNode(node, &buffer);
    return buffer;
}

TDJSONObject::ValueNode::ValueNode(double value) : nodeType(NUMBER) {
    valueData.numberValue = value;
}

TDJSONObject::ValueNode::ValueNode(int64_t value) : nodeType(INT) {
    valueData.intValue = value;
}

TDJSONObject::ValueNode::ValueNode(const string &value) : nodeType(STRING), stringData(value) {}

TDJSONObject::ValueNode::ValueNode(bool value) : nodeType(BOOL1) {
    valueData.boolValue = value;
}

TDJSONObject::ValueNode::ValueNode(const std::vector<string> &value) : nodeType(LIST), listData(value) {}

TDJSONObject::ValueNode::ValueNode(time_t seconds, int milliseconds) : nodeType(DATETIME) {
    valueData.datetimeValue.seconds = seconds;
    valueData.datetimeValue.milliseconds = milliseconds;
}

TDJSONObject::ValueNode::ValueNode(const TDJSONObject &value) : nodeType(OBJECT), objectData(value) {}

TDJSONObject::ValueNode::ValueNode(const std::vector<TDJSONObject> &value) : nodeType(OBJECTS), objectDatas(value) {}

void TDJSONObject::ValueNode::toStr(const TDJSONObject::ValueNode &node, string *buffer) {
    switch (node.nodeType) {
        case NUMBER:
            dumpNumber(node.valueData.numberValue, buffer);
            break;
        case INT:
            dumpNumber(node.valueData.intValue, buffer);
            break;
        case STRING:
            dumpString(node.stringData, buffer);
            break;
        case LIST:
            dumpList(node.listData, buffer);
            break;
        case BOOL1:
            *buffer += (node.valueData.boolValue ? "true" : "false");
            break;
        case DATETIME:
            dumpDateTime(node.valueData.datetimeValue.seconds,
                         node.valueData.datetimeValue.milliseconds, buffer);
            break;
        case OBJECT:
            dumpNode(node.objectData, buffer);
            break;
        case OBJECTS:
            dumpList(node.objectDatas, buffer);
            break;
        default:
            break;
    }
}

void TDJSONObject::ValueNode::dumpNumber(double value, string *buffer) {
    std::ostringstream buf;
    buf << value;
    *buffer += buf.str();
}

void TDJSONObject::ValueNode::dumpNumber(int64_t value, string *buffer) {
    std::ostringstream buf;
    buf << value;
    *buffer += buf.str();
}

void TDJSONObject::mergeFrom(const TDJSONObject &anotherNode) {
    for (std::map<string, ValueNode>::const_iterator
                 iterator = anotherNode.propertiesMap.begin();
         iterator != anotherNode.propertiesMap.end(); ++iterator) {
        propertiesMap[iterator->first] = iterator->second;
    }
}

TDEventModel::TDEventModel(string eventName, TDJSONObject properties) {
    this->mEventName = eventName;
    this->mProperties = properties;
}

TDFirstEventModel::TDFirstEventModel(string eventName, TDJSONObject properties):TDEventModel(eventName,properties) {
    this->mType = FIRST;
    this->mExtraId = "";
}

void TDFirstEventModel::setFirstCheckId(string firstCheckId) {
    this->mExtraId = firstCheckId;
}
TDUpdatableEventModel::TDUpdatableEventModel(string eventName, TDJSONObject properties, string eventId):TDEventModel(eventName,properties) {
    this->mExtraId = eventId;
    this->mType = UPDATABLE;
}
TDOverwritableEventModel::TDOverwritableEventModel(string eventName, TDJSONObject properties, string eventId):TDEventModel(eventName,properties) {
    this->mExtraId = eventId;
    this->mType = OVERWRITABLE;
}

//PresetPropewrties
TDJSONObject* TDPresetProperties::toEventPresetProperties()
{

// windows
#if defined(_WIN32) && defined(_WINDOWS)
    TDJSONObject* jsonObject = new TDJSONObject();
    jsonObject->setString("#device_id", this->deviceId.c_str());
    jsonObject->setString("#os", this->os.c_str());
    jsonObject->setNumber("#screen_height", this->screenHeight);
    jsonObject->setNumber("#screen_width", this->screenWidth);
    jsonObject->setString("#system_language", this->systemLanguage.c_str());
    jsonObject->setString("#app_version", this->appVersion.c_str());
    jsonObject->setNumber("#zone_offset", this->zoneOffset);
    jsonObject->setString("#lib_version", this->libVersion.c_str());
    jsonObject->setString("#lib", this->libName.c_str());
    return jsonObject;
    
// Apple: Mac and iOS
#elif defined(__APPLE__) && !defined(ANDROID)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
    TDJSONObject* jsonObject = new TDJSONObject();
    jsonObject->setString("#carrier", this->carrier.c_str());
    jsonObject->setString("#os", this->os.c_str());
    jsonObject->setString("#device_id", this->deviceId.c_str());
    jsonObject->setNumber("#screen_height", this->screenHeight);
    jsonObject->setString("#bundle_id", this->bundleId.c_str());
    jsonObject->setString("#manufacturer", this->manufacturer.c_str());
    jsonObject->setString("#device_model", this->deviceModel.c_str());
    jsonObject->setNumber("#screen_width", this->screenWidth);
    jsonObject->setString("#system_language", this->systemLanguage.c_str());
    jsonObject->setString("#os_version", this->osVersion.c_str());
    jsonObject->setString("#network_type", this->networkType.c_str());
    jsonObject->setNumber("#zone_offset", this->zoneOffset);
    
    jsonObject->setBool("#simulator", this->isSimulator);
    jsonObject->setString("#disk", this->disk.c_str());
    jsonObject->setString("#ram", this->ram.c_str());
    jsonObject->setNumber("#fps", this->fps);
    jsonObject->setString("#app_version", this->appVersion.c_str());
    
    return jsonObject;
    #elif TARGET_OS_MAC
    TDJSONObject* jsonObject = new TDJSONObject();
    jsonObject->setString("#device_id", this->deviceId.c_str());
    jsonObject->setString("#os", this->os.c_str());
    jsonObject->setNumber("#screen_height", this->screenHeight);
    jsonObject->setNumber("#screen_width", this->screenWidth);
    jsonObject->setString("#system_language", this->systemLanguage.c_str());
    jsonObject->setString("#app_version", this->appVersion.c_str());
    jsonObject->setNumber("#zone_offset", this->zoneOffset);
    jsonObject->setString("#lib_version", this->libVersion.c_str());
    jsonObject->setString("#lib", this->libName.c_str());
    return jsonObject;
    #endif
 
#else
    // Android
    TDJSONObject* jsonObject = new TDJSONObject();
    jsonObject->setString("#carrier", this->carrier.c_str());
    jsonObject->setString("#os", this->os.c_str());
    jsonObject->setString("#device_id", this->deviceId.c_str());
    jsonObject->setNumber("#screen_height", this->screenHeight);
    jsonObject->setString("#bundle_id", this->bundleId.c_str());
    jsonObject->setString("#manufacturer", this->manufacturer.c_str());
    jsonObject->setString("#device_model", this->deviceModel.c_str());
    jsonObject->setNumber("#screen_width", this->screenWidth);
    jsonObject->setString("#system_language", this->systemLanguage.c_str());
    jsonObject->setString("#os_version", this->osVersion.c_str());
    jsonObject->setString("#network_type", this->networkType.c_str());
    jsonObject->setNumber("#zone_offset", this->zoneOffset);
    
    jsonObject->setBool("#simulator", this->isSimulator);
    jsonObject->setString("#disk", this->disk.c_str());
    jsonObject->setString("#ram", this->ram.c_str());
    jsonObject->setNumber("#fps", this->fps);
    jsonObject->setString("#app_version", this->appVersion.c_str());
    
    return jsonObject;
#endif
}

// Secret
TDSecretKey::TDSecretKey(int version, string publicKey)
{
    this->version = version;
    this->publicKey = publicKey;
}

TDSecretKey::~TDSecretKey()
{
   
}

//TDConfig
TDConfig::TDConfig(string appId,string server)
{
    this->appId = appId;
    this->server = server;
    this->model = TD_NORMAL;
    this->pinningMode = TDSSLPinningModeNone;
    this->name =  appId;
    this->enableEncrypt = false;
}

void TDConfig::setEnableEncrypt(bool enable)
{
    this->enableEncrypt = enable;
}
void TDConfig::setSecretKey(TDSecretKey secretKey)
{
    this->secretKey = secretKey;
}
bool TDConfig::getEnableEncrypt()
{
    return enableEncrypt;
}


void TDConfig::setPinningMode(TDSSLPinningMode pinningMode)
{
    this->pinningMode = pinningMode;
}

TDSSLPinningMode TDConfig::getPinningMode()
{
    return  pinningMode;
}

TDSecretKey TDConfig::getSecretKey()
{
    return secretKey;
}

void TDConfig::setModel(TDMode model)
{
    this->model = model;
}
void TDConfig::setName(string name) {
    this->name = name;
}
string TDConfig::getName()
{
    return name;
}

string TDConfig::getAppId()
{
    return appId;
}

string TDConfig::getServer()
{
    return server;
}

TDMode TDConfig::getModel()
{
    return  model;
}

TDConfig::~TDConfig()
{
   
}


