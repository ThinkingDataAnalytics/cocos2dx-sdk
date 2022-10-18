/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "./ThinkingAnalytics/Common/ThinkingAnalyticsAPI.h"

#include <thread>

using namespace thinkingdata;
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;


static string platformTag;


const static string APPID_1 = "1b1c1fef65e3482bad5c9d0e6a823356";
const static string APPID_2 = "22e445595b0f42bd8c5fe35bc44b88d6";
int num = 100000000;
int num_1 = 200000000;
double double_num = 12.345;


string getPlatformTag() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    platformTag = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    platformTag = "ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    platformTag = "win";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    platformTag = "mac";
#else
    platformTag = "unknow";
#endif
    return platformTag;
}

TDJSONObject superDylyPropery() {

    vector<TDJSONObject> objs;
    
    vector<string> arr;
    arr.push_back("XX_1");
    arr.push_back("XX_2");
    arr.push_back("XX_3");
    
    TDJSONObject obj1;
    obj1.setNumber("doubleNum_dyld", 3.1415926);
    obj1.setNumber("intNum_dyld", 33333);
    obj1.setNumber("int64Num_dyld", 898989898);
    obj1.setString("string_dyld", "string");
    obj1.setBool("bool1_dyld", false);
    obj1.setBool("bool2_dyld", true);
    obj1.setList("list_dyld", arr);
    obj1.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    TDJSONObject obj2;
    obj2.setNumber("doubleNum_dyld", 3.1415926);
    obj2.setNumber("intNum_dyld", 33333);
    obj2.setNumber("int64Num_dyld", 898989898);
    obj2.setString("string_dyld", "string");
    obj2.setBool("bool1_dyld", false);
    obj2.setBool("bool2_dyld", true);
    obj2.setList("list_dyld", arr);
    obj2.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    TDJSONObject obj3;
    obj3.setNumber("doubleNum_dyld", 3.1415926);
    obj3.setNumber("intNum_dyld", 33333);
    obj3.setNumber("int64Num_dyld", 898989898);
    obj3.setString("string_dyld", "string");
    obj3.setBool("bool1_dyld", false);
    obj3.setBool("bool2_dyld", true);
    obj3.setList("list_dyld", arr);
    obj3.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    objs.push_back(obj1);
    objs.push_back(obj1);
    
    TDJSONObject obj;
    obj.setList("objs_dyld", objs);
    obj.setJsonObject("object_dyld", obj3);

    return obj;
}

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


void showInstanceMessage() {

//    //国家和集群的配置关系
//    map<string, string> countryConfig = {
//            { "IN","IN" },{ "IE","EU" },{ "EE","EU" },{ "AT","EU" },
//            { "BG","EU" },{ "BE","EU" },{ "PL","EU" },{ "DK","EU" },
//            { "DE","EU" },{ "RU","RU" },{ "FR","EU" },{ "FI","EU" },
//            { "NL","EU" },{ "CZ","EU" },{ "HR","EU" },{ "LV","EU" },
//            { "LT","EU" },{ "RO","EU" },{ "LU","EU" },{ "MT","EU" },
//            { "PT","EU" },{ "SE","EU" },{ "CY","EU" },{ "SK","EU" },
//            { "SI","EU" },{ "HU","EU" },{ "ES","EU" },{ "GR","EU" },
//            { "IT","EU" },{ "GB","EU" } };
//
//    // 查找每个区域对应的服务器地址
//    string ls_location = ThinkingAnalyticsAPI::getLocalRegion();
//    std::map<string, string>::iterator clusterIter = countryConfig.find(ls_location);
//
//    // // 默认为新加坡区 ;
//    string ta_cluster_region = "SG";
//    if (clusterIter != countryConfig.end()) {
//        ta_cluster_region = clusterIter->second;
//    }
//
//    //通过集群名称获取集群配置的数据上报地址
//    string ls_ta_server = "https://ta-receiver-sgp.g.mi.com";//新加坡集群上报地址
//    if (ta_cluster_region == "IN")ls_ta_server = "https://ta-receiver-mb.g.mi.com"; //印度集群上报地址
//    if (ta_cluster_region == "EU")ls_ta_server = "https://ta-receiver-eu.g.mi.com"; //欧洲集群上报地址
//    if (ta_cluster_region == "RU")ls_ta_server = "https://ta-receiver-rus.g.mi.com";//俄罗斯集群上报地址
//
//    //开始SDK初始化
//    string ls_ta_appid = "e36b56a0c1f54887b296bfd6736e811c";
//    ThinkingAnalyticsAPI::init(ls_ta_appid, ls_ta_server);
//
//    // 设置好通用参数
//    TDJSONObject superProperties;
//    superProperties.setString("ta_local_region", ls_location.c_str());
//    superProperties.setString("ta_cluster_region", ta_cluster_region.c_str());
//    superProperties.setString("ta_cluster_server", ls_ta_server.c_str());
//    ThinkingAnalyticsAPI::setSuperProperties(superProperties);
//
//    // 开启自动追踪
//    ThinkingAnalyticsAPI::enableAutoTrack();
//
//    ThinkingAnalyticsAPI::enableTrackLog(true);


    ThinkingAnalyticsAPI::enableTrackLog(true);

    Config config1(APPID_1,"https://receiver.ta.thinkingdata.cn");
    ThinkingAnalyticsAPI::init(config1);
    

    Config config2(APPID_2,"https://receiver.ta.thinkingdata.cn");
    ThinkingAnalyticsAPI::init(config2);
    

    CCLOG("########## ins1 ##########");
    CCLOG("appID:%s", APPID_1.c_str());
    CCLOG("deviceID:%s", ThinkingAnalyticsAPI::getDeviceId().c_str());
    CCLOG("distinctID:%s", ThinkingAnalyticsAPI::getDistinctId().c_str());
    CCLOG("superProperty:%s", TDJSONObject::toJson(ThinkingAnalyticsAPI::getSuperProperties()).c_str());
    CCLOG("presentProperty:%s", TDJSONObject::toJson(*ThinkingAnalyticsAPI::getPresetProperties()->toEventPresetProperties()).c_str());

    
    CCLOG("");
    CCLOG("");
    
    CCLOG("########## ins2 ##########");
    CCLOG("appID:%s", APPID_2.c_str());
    CCLOG("deviceID:%s", ThinkingAnalyticsAPI::getDeviceId().c_str());
    CCLOG("distinctID:%s", ThinkingAnalyticsAPI::getDistinctId(APPID_2).c_str());
    CCLOG("superProperty:%s", TDJSONObject::toJson(ThinkingAnalyticsAPI::getSuperProperties(APPID_2)).c_str());
    CCLOG("presentProperty:%s", TDJSONObject::toJson(*ThinkingAnalyticsAPI::getPresetProperties(APPID_2)->toEventPresetProperties()).c_str());

    
    CCLOG("");
    CCLOG("");
    
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    ThinkingAnalyticsAPI::enableAutoTrack();
    ThinkingAnalyticsAPI::enableAutoTrack(APPID_2);
#endif

}

static void problemLoaenableAutoTrackding(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() ) {
        return false;
    }
    
    getPlatformTag();
    showInstanceMessage();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static string lightToken =  ThinkingAnalyticsAPI::createLightInstance();
#endif
    
    auto leftMargin = origin.x + 50;
    auto topMargin = 25;
    auto commonFontSize = 10;
    auto commonMargin = 10;
    auto userLabel = Label::createWithSystemFont("user identiyf", "Arial", 12);
    userLabel->setTextColor(Color4B::ORANGE);
    userLabel->setPosition(Vec2((visibleSize.width -userLabel->getContentSize().width)/2.0 ,
                                origin.y + visibleSize.height - userLabel->getContentSize().height));
    this->addChild(userLabel);


    auto userButton = Button::create();
    userButton->setTitleText("accountID");
    userButton->setTitleFontSize(commonFontSize);
    userButton->setPosition(Vec2(leftMargin,
                                 userLabel->getPositionY() - topMargin));
    userButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::login(("accountid_"+platformTag+"_1").c_str());
            ThinkingAnalyticsAPI::login(("accountid_"+platformTag+"_2").c_str(), APPID_2);
        }
    });
    this->addChild(userButton);


    auto userButton1 = Button::create();
    userButton1->setTitleFontSize(commonFontSize);
    userButton1->setTitleText("distinctID");
    userButton1->setPosition(Vec2(10 + userButton->getPositionX() + userButton->getContentSize().width,
                                  userButton->getPositionY()));
    userButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::identify(("distinct_"+platformTag+"_1").c_str());
            ThinkingAnalyticsAPI::identify(("distinct_"+platformTag+"_2").c_str(), APPID_2);
        }
    });
    this->addChild(userButton1);

    auto userButton2 = Button::create();
    userButton2->setTitleFontSize(commonFontSize);
    userButton2->setTitleText("clearAccID");
    userButton2->setPosition(Vec2(10 + userButton1->getPositionX() + userButton1->getContentSize().width,
                                  userButton->getPositionY()));
    userButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::logout();
            ThinkingAnalyticsAPI::logout(APPID_2);
        }
    });
    this->addChild(userButton2);
    
    auto userButton22 = Button::create();
    userButton22->setTitleFontSize(commonFontSize);
    userButton22->setTitleText("disenable");
    userButton22->setPosition(Vec2(10 + userButton2->getPositionX() + userButton22->getContentSize().width,
                                  userButton->getPositionY()));
    userButton22->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::enableTracking(false);
            ThinkingAnalyticsAPI::enableTracking(false, APPID_2);
        }
    });
    this->addChild(userButton22);
    
    auto userButton33 = Button::create();
    userButton33->setTitleFontSize(commonFontSize);
    userButton33->setTitleText("enable");
    userButton33->setPosition(Vec2(10 + userButton22->getPositionX() + userButton33->getContentSize().width,
                                  userButton->getPositionY()));
    userButton33->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::enableTracking(true);
            ThinkingAnalyticsAPI::enableTracking(true, APPID_2);
        }
    });
    this->addChild(userButton33);
    
    auto userButton44 = Button::create();
    userButton44->setTitleFontSize(commonFontSize);
    userButton44->setTitleText("optOut");
    userButton44->setPosition(Vec2(10 + userButton33->getPositionX() + userButton44->getContentSize().width,
                                  userButton->getPositionY()));
    userButton44->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::optOutTracking();
            ThinkingAnalyticsAPI::optOutTracking(APPID_2);
        }
    });
    this->addChild(userButton44);
    
    auto userButton55 = Button::create();
    userButton55->setTitleFontSize(commonFontSize);
    userButton55->setTitleText("optIn");
    userButton55->setPosition(Vec2(10 + userButton44->getPositionX() + userButton55->getContentSize().width,
                                  userButton->getPositionY()));
    userButton55->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::optInTracking();
            ThinkingAnalyticsAPI::optInTracking(APPID_2);
        }
    });
    this->addChild(userButton55);


    auto eventLabel = Label::createWithSystemFont("eventTrack", "Arial", 12);
    eventLabel->setTextColor(Color4B::ORANGE);
    eventLabel->setPosition(Vec2(userLabel->getPositionX(),
                                 userButton->getPositionY() - topMargin));
    this->addChild(eventLabel);

    auto eventButton = Button::create();
    eventButton->setTitleText("track");
    eventButton->setTitleFontSize(commonFontSize);
    eventButton->setPosition(Vec2(leftMargin,
                                  eventLabel->getPositionY() - topMargin));
    eventButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::track("track");
            ThinkingAnalyticsAPI::track("track", APPID_2);
            
            ThinkingAnalyticsAPI::flush();
            ThinkingAnalyticsAPI::flush(APPID_2);
        }
    });
    this->addChild(eventButton);


    auto eventButton1 = Button::create();
    eventButton1->setTitleText("trackAndProperty");
    eventButton1->setTitleFontSize(commonFontSize);
    eventButton1->setPosition(Vec2(30 + userButton->getPositionX() + userButton->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            vector<TDJSONObject> objs;
            
            vector<string> arr;
            arr.push_back("XX_1");
            arr.push_back("XX_2");
            arr.push_back("XX_3");
            
            TDJSONObject obj1;
            obj1.setNumber("doubleNum", 3.1415926);
            obj1.setNumber("intNum", 33333);
            obj1.setNumber("int64Num", 898989898);
            obj1.setString("string", "string");
            obj1.setBool("bool1", false);
            obj1.setBool("bool2", true);
            obj1.setList("list", arr);
            obj1.setDateTime("time","3033-03-03 10:52:52.290");
            
            TDJSONObject obj2;
            obj2.setNumber("doubleNum", 3.1415926);
            obj2.setNumber("intNum", 33333);
            obj2.setNumber("int64Num", 898989898);
            obj2.setString("string", "string");
            obj2.setBool("bool1", false);
            obj2.setBool("bool2", true);
            obj2.setList("list", arr);
            obj2.setDateTime("time","3033-03-03 10:52:52.290");
            
            TDJSONObject obj3;
            obj3.setNumber("doubleNum", 3.1415926);
            obj3.setNumber("intNum", 33333);
            obj3.setNumber("int64Num", 898989898);
            obj3.setString("string", "string");
            obj3.setBool("bool1", false);
            obj3.setBool("bool2", true);
            obj3.setList("list", arr);
            obj3.setDateTime("time","3033-03-03 10:52:52.290");
            
            objs.push_back(obj1);
            objs.push_back(obj1);
            
            TDJSONObject obj;
            obj.setList("objs", objs);
            obj.setJsonObject("obj", obj3);

            ThinkingAnalyticsAPI::track("track_property",obj);
            ThinkingAnalyticsAPI::track("track_property",obj, APPID_2);
            
            ThinkingAnalyticsAPI::flush();
            ThinkingAnalyticsAPI::flush(APPID_2);
        }
    });
    this->addChild(eventButton1);

    auto eventButton2 = Button::create();
    eventButton2->setTitleFontSize(commonFontSize);
    eventButton2->setTitleText("timeEvent");
    eventButton2->setPosition(Vec2(commonMargin + eventButton1->getPositionX() + eventButton1->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::track("timeEvent");
            ThinkingAnalyticsAPI::track("timeEvent", APPID_2);
        }
    });
    this->addChild(eventButton2);

    auto eventButton3 = Button::create();
    eventButton3->setTitleText("firstEvent");
    eventButton3->setTitleFontSize(commonFontSize);
    eventButton3->setPosition(Vec2(commonMargin + eventButton2->getPositionX() + eventButton2->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDFirstEvent *firstEvent2 = new TDFirstEvent("firstEvent",jsonObject1);
            ThinkingAnalyticsAPI::track(firstEvent2);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_2);
            
            firstEvent2->setFirstCheckId("firstCheckID");
            ThinkingAnalyticsAPI::track(firstEvent2);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_2);
        }
    });
    this->addChild(eventButton3);

    auto eventButton4 = Button::create();
    eventButton4->setTitleText("updateEvent");
    eventButton4->setTitleFontSize(commonFontSize);
    eventButton4->setPosition(Vec2(commonMargin + eventButton3->getPositionX() + eventButton3->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            jsonObject1.setNumber("status", 3);
            jsonObject1.setNumber("price", 5);
            TDUpdatableEvent *updatableEvent1 = new TDUpdatableEvent("updateEvent",jsonObject1,"12345");
            ThinkingAnalyticsAPI::track(updatableEvent1);
            ThinkingAnalyticsAPI::track(updatableEvent1,APPID_2);
        }
    });
    this->addChild(eventButton4);



    auto eventButton5 = Button::create();
    eventButton5->setTitleText("overWriteEvent");
    eventButton5->setTitleFontSize(commonFontSize);
    eventButton5->setPosition(Vec2(commonMargin + eventButton4->getPositionX() + eventButton4->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDOverWritableEvent *overWritableEvent1 = new TDOverWritableEvent("overWriteEvent",jsonObject1,"12345");
            ThinkingAnalyticsAPI::track(overWritableEvent1);
            ThinkingAnalyticsAPI::track(overWritableEvent1,APPID_2);
        }
    });
    this->addChild(eventButton5);



    auto userPropertyLabel = Label::createWithSystemFont("userEvent", "Arial", 12);
    userPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                        eventButton->getPositionY() - topMargin));
    this->addChild(userPropertyLabel);
    userPropertyLabel->setTextColor(Color4B::ORANGE);
    auto userPropertyButton = Button::create();
    userPropertyButton->setTitleFontSize(10);
    userPropertyButton->setTitleText("UserSet");
    userPropertyButton->setPosition(Vec2(leftMargin,
                                         userPropertyLabel->getPositionY() - topMargin));
    userPropertyButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setString("userSet_key", "userSet_value");
            ThinkingAnalyticsAPI::user_set(userProperties1);
            ThinkingAnalyticsAPI::user_set(userProperties1,APPID_2);
        }
    });
    this->addChild(userPropertyButton);


    auto userPropertyButton1 = Button::create();
    userPropertyButton1->setTitleFontSize(10);
    userPropertyButton1->setTitleText("UserSetOnce");
    userPropertyButton1->setPosition(Vec2(25 + userPropertyButton->getPositionX() + userPropertyButton->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setString("userSetOnce_key", "userSetOnce_value");
            userProperties1.setNumber("userSetOnce_int",1);
            ThinkingAnalyticsAPI::user_setOnce(userProperties1);
            ThinkingAnalyticsAPI::user_setOnce(userProperties1, APPID_2);
        }
    });
    this->addChild(userPropertyButton1);


    auto userPropertyButton2 = Button::create();
    userPropertyButton2->setTitleFontSize(10);
    userPropertyButton2->setTitleText("UserAdd");
    userPropertyButton2->setPosition(Vec2(25 + userPropertyButton1->getPositionX() + userPropertyButton1->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setNumber("userAdd_int",1);
            ThinkingAnalyticsAPI::user_add(userProperties1);
            ThinkingAnalyticsAPI::user_add(userProperties1, APPID_2);
        }
    });
    this->addChild(userPropertyButton2);


    auto userPropertyButton3 = Button::create();
    userPropertyButton3->setTitleFontSize(10);
    userPropertyButton3->setTitleText("UserUnset");
    userPropertyButton3->setPosition(Vec2(25 + userPropertyButton2->getPositionX() + userPropertyButton2->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::user_unset("userUnset_key");
            ThinkingAnalyticsAPI::user_unset("userUnset_key",APPID_2);
        }
    });
    this->addChild(userPropertyButton3);


    auto userPropertyButton4 = Button::create();
    userPropertyButton4->setTitleFontSize(10);
    userPropertyButton4->setTitleText("UserDelete");
    userPropertyButton4->setPosition(Vec2(25 + userPropertyButton3->getPositionX() + userPropertyButton3->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::user_delete();
            ThinkingAnalyticsAPI::user_delete(APPID_2);
        }
    });
    this->addChild(userPropertyButton4);

    auto userPropertyButton5 = Button::create();
    userPropertyButton5->setTitleFontSize(10);
    userPropertyButton5->setTitleText("UserAppend");
    userPropertyButton5->setPosition(Vec2(25 + userPropertyButton4->getPositionX() + userPropertyButton4->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            vector<string> listValue1;
            listValue1.push_back("XX");
            userProperties1.setList("userAppend_key",listValue1);
            ThinkingAnalyticsAPI::user_append(userProperties1);
            ThinkingAnalyticsAPI::user_append(userProperties1, APPID_2);
        }
    });
    this->addChild(userPropertyButton5);
    

    auto publicPropertyLabel = Label::createWithSystemFont("commonProperty", "Arial", 12);
    publicPropertyLabel->setTextColor(Color4B::ORANGE);
    publicPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                          userPropertyButton->getPositionY() - topMargin));
    this->addChild(publicPropertyLabel);

    auto publicPropertyButton = Button::create();
    publicPropertyButton->setTitleFontSize(10);
    publicPropertyButton->setTitleText("setStaicProperty");
    publicPropertyButton->setPosition(Vec2(leftMargin,
                                           publicPropertyLabel->getPositionY() - topMargin));
    publicPropertyButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            vector<TDJSONObject> objs;
            
            vector<string> arr;
            arr.push_back("XX_1");
            arr.push_back("XX_2");
            arr.push_back("XX_3");
            
            TDJSONObject obj1;
            obj1.setNumber("doubleNum_static", 3.1415926);
            obj1.setNumber("intNum_static", 33333);
            obj1.setNumber("int64Num_static", 898989898);
            obj1.setString("string_static", "string");
            obj1.setBool("bool1_static", false);
            obj1.setBool("bool2_static", true);
            obj1.setList("list_static", arr);
            obj1.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            TDJSONObject obj2;
            obj2.setNumber("doubleNum_static", 3.1415926);
            obj2.setNumber("intNum_static", 33333);
            obj2.setNumber("int64Num_static", 898989898);
            obj2.setString("string_static", "string");
            obj2.setBool("bool1_static", false);
            obj2.setBool("bool2_static", true);
            obj2.setList("list_static", arr);
            obj2.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            TDJSONObject obj3;
            obj3.setNumber("doubleNum_static", 3.1415926);
            obj3.setNumber("intNum_static", 33333);
            obj3.setNumber("int64Num_static", 898989898);
            obj3.setString("string_static", "string");
            obj3.setBool("bool1_static", false);
            obj3.setBool("bool2_static", true);
            obj3.setList("list_static", arr);
            obj3.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            objs.push_back(obj1);
            objs.push_back(obj1);
            
            TDJSONObject obj;
            obj.setList("objs_static", objs);
            obj.setJsonObject("obj_static", obj3);
            obj.setList("list_static", arr);
            
            ThinkingAnalyticsAPI::setSuperProperties(obj);
            ThinkingAnalyticsAPI::setSuperProperties(obj, APPID_2);
        }
    });
    this->addChild(publicPropertyButton);


    auto publicPropertyButton1 = Button::create();
    publicPropertyButton1->setTitleFontSize(commonFontSize);
    publicPropertyButton1->setTitleText("setDyldProperty");
    publicPropertyButton1->setPosition(Vec2(25 + publicPropertyButton->getPositionX() + publicPropertyButton->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::setDynamicSuperProperties(superDylyPropery);
            ThinkingAnalyticsAPI::setDynamicSuperProperties(superDylyPropery, APPID_2);
        }
    });
    this->addChild(publicPropertyButton1);


    auto publicPropertyButton2 = Button::create();
    publicPropertyButton2->setTitleFontSize(10);
    publicPropertyButton2->setTitleText("clearOneProperty");
    publicPropertyButton2->setPosition(Vec2(25 + publicPropertyButton1->getPositionX() + publicPropertyButton1->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::unsetSuperProperty("obj_static");
            ThinkingAnalyticsAPI::unsetSuperProperty("obj_static",APPID_2);
        }
    });
    this->addChild(publicPropertyButton2);


    auto publicPropertyButton3 = Button::create();
    publicPropertyButton3->setTitleFontSize(10);
    publicPropertyButton3->setTitleText("ClearAllProperty");
    publicPropertyButton3->setPosition(Vec2(25 + publicPropertyButton2->getPositionX() + publicPropertyButton2->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::clearSuperProperties();
            ThinkingAnalyticsAPI::clearSuperProperties(APPID_2);
        }
    });
    this->addChild(publicPropertyButton3);

//    auto flushButton = Button::create();
//    flushButton->setTitleFontSize(10);
//    flushButton->setTitleText("Flush");
//    flushButton->setPosition(Vec2(25 + publicPropertyButton3->getPositionX() + publicPropertyButton3->getContentSize().width,
//                                            publicPropertyButton->getPositionY()));
//    flushButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
//        if(type == ui::Widget::TouchEventType::ENDED)
//        {
////            ThinkingAnalyticsAPI::clearSuperProperties();
//            ThinkingAnalyticsAPI::flush();
//        }
//    });
//    this->addChild(flushButton);


//    PresetProperties* presetProperties =  ThinkingAnalyticsAPI::getPresetProperties(APPID_1);
//    string distintID =  ThinkingAnalyticsAPI::getDistinctId();

//    // CCLog();
//     string properties = TDJSONObject::toJson(*(presetProperties->toEventPresetProperties()));
//     printf("%s\n", properties.c_str());
//     printf("BundleID:%s\n",presetProperties->bundleId.c_str());
//     printf("Carrier:%s\n",presetProperties->carrier.c_str());
//     printf("DeviceID:%s\n",presetProperties->deviceId.c_str());
//     printf("DeviceModel:%s\n",presetProperties->deviceModel.c_str());
//     printf("Manufacturer:%s\n",presetProperties->manufacturer.c_str());
//     printf("NetworkType:%s\n",presetProperties->networkType.c_str());
//     printf("ScreenWidth:%i\n",presetProperties->screenWidth);
//     printf("ScreenHeight:%i\n",presetProperties->screenHeight);
//     printf("OS:%s\n",presetProperties->os.c_str());
//     printf("OSVersion:%s\n",presetProperties->osVersion.c_str());
//     printf("ZoneOffset:%f\n",presetProperties->zoneOffset);
//     printf("SystemLanguage:%s\n",presetProperties->systemLanguage.c_str());

    return true;
}




void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

